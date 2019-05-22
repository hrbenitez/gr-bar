/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "packet_checker_impl.h"

#define DEBUG 0
#define DEBUG1 0
#define dout_1 DEBUG1 && std::cout
#define dout DEBUG && dout_1

#define _chrono std::chrono
#define hrc std::chrono::high_resolution_clock

namespace gr {
  namespace bar {

    packet_checker::sptr
    packet_checker::make(unsigned long int samp_rate, unsigned int threshold, unsigned int choice)
    {
      return gnuradio::get_initial_sptr
        (new packet_checker_impl(samp_rate, threshold, choice));
    }

    /*
     * The private constructor
     */
    packet_checker_impl::packet_checker_impl(unsigned long int samp_rate, unsigned int threshold, unsigned int choice)
      : gr::block("packet_checker",
              gr::io_signature::make(4, 4, sizeof(char)),
              gr::io_signature::make(3, 3, sizeof(float))),
              d_samp_rate(samp_rate),
              d_threshold(threshold),
              d_lat_choice(choice),
              d_received_packets(0),
              d_sent_packets(0),
              d_total_bits(0),
              d_have_sync(false)
    {
      d_start_clock = hrc::now();
      d_proc_overhead = _chrono::duration_cast<_chrono::microseconds>(d_start_clock - d_start_clock);
    }

    /*
     * Our virtual destructor.
     */
    packet_checker_impl::~packet_checker_impl()
    {
    }

    void
    packet_checker_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }
    
    namespace pc {
      int find_sync_start (const char *sync, int n) {
        int i = 0;
        while(i < n) {
          if (sync[i] > 0) {
            return i;
          }
          i++;
        }
        return -1;
      }
      int find_sync_end (const char *sync, int n) {
        int i = 0;
        while(i < n) {
          if (sync[i] == 0) {
            return i;
          }
          i++;
        }
        return i;
      }
    }
    
    int
    packet_checker_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      using namespace pc;
      const char *in1 = (const char *) input_items[0];
      const char *sync1 = (const char *) input_items[1];
      const char *in2 = (const char *) input_items[2];
      const char *sync2 = (const char *) input_items[3];
      float *err = (float *) output_items[0];
      float *thr = (float *) output_items[1];
      float *lat = (float *) output_items[2];
      
      int ninput1 = std::min(ninput_items[0], ninput_items[1]);
      int ninput2 = std::min(ninput_items[2], ninput_items[3]);
      
      int n = 0;  
      int count1 = find_sync_start(sync1, ninput1);
      int end1 = ninput1;
      if (count1 >= 0) {
        end1 = find_sync_end(sync1+count1, ninput1-count1);
        if (end1 > 256) {
          if (!d_have_sync) {
            d_sync_clock = hrc::now();
            d_have_sync = true;
            d_sent_packets++;
            dout << "Sync 1 at " << count1 << " of " << ninput1 << '\n';
            dout << "Sync 1 end at " << count1 + end1 << ", total: " << end1 << '\n';
            if (d_proc_overhead.count() == 0) {
              d_proc_overhead = _chrono::duration_cast<_chrono::microseconds>(d_sync_clock - d_start_clock);
              std::cout << "Processing overhead estimated at " << d_proc_overhead.count()/1000.0 - (double) d_samp_rate << "ms.\n";
            }
          }
        } else {
          dout << "Sync 1 too short.\n";
          return 0;
        }
      } else {
        consume(0, ninput1);
        consume(1, ninput1);
      }
      
      int count2 = find_sync_start(sync2, ninput2);
      int end2 = ninput2;
      if (count2 >= 0) {
        if (d_have_sync) {
          end2 = find_sync_end(sync2+count2, ninput2-count2);
          if (count2+end2 == ninput2) {
            dout << "Sync at end of buffer, wait for more\n";
            return 0;
          }
          if (end2 > 512) {
            dout << "Sync 2 at " << count2 << " of " << ninput2 << '\n';
            dout << "Sync 2 end at " << count2 + end2 << ", total: " << end2 << '\n';
            
            n = std::min(end1, end2);
            
            const char *pckt1 = in1+count1, *pckt2 = in2+count2;
            int sn1 = (*std::max_element(pckt1 + 224, pckt1 + 255)) * 16 + (*std::max_element(pckt1 + 192, pckt1 + 223));
            int sn2 = (*std::max_element(pckt2 + 224, pckt2 + 255)) * 16 + (*std::max_element(pckt2 + 192, pckt2 + 223));
            //if (((d_sent_packets)%256) != (sn1+1))
              //d_sent_packets =  d_sent_packets - (d_sent_packets%256) + sn1 + 1; // re-sync
                       
            if (sn1 == sn2) {
              double err_rate = 1.0 - (double) (++d_received_packets)/d_sent_packets;
              int p_length = end2/8;
              err_rate = (err_rate < 0)? 0 : err_rate;
              d_have_sync = false;
              d_total_bits += p_length;
              //d_total_bits = d_received_packets*p_length + (sn1+1-((d_sent_packets)%256))*p_length;
              
              d_recv_clock = hrc::now();
              _chrono::duration<double> total_time = _chrono::duration_cast<_chrono::duration<double>>(d_recv_clock - d_start_clock);
              _chrono::microseconds travel_time = _chrono::duration_cast<_chrono::microseconds>(d_recv_clock - d_sync_clock);
              
              float tot_lat = (d_lat_choice == 1)? (travel_time.count() + d_proc_overhead.count())/1000.0 - (double) d_samp_rate : travel_time.count()/1000;
              float throughput = ((double)(d_total_bits))/(total_time.count());
              
              dout_1 << "\nPacket # " << d_sent_packets-1 << " received after " << total_time.count() <<  "s\n";
              dout_1 << "SN 1 = " << sn1 << ", SN 2 = " << sn2 << "\n";
              dout_1 << "Travel time estimated at " << (travel_time.count()/1000.0) << "ms\n";
              dout_1 << "Received " << d_received_packets << " of " << d_sent_packets << "\n";
              dout_1 << "Error rate = " <<  err_rate*100.0 << "%\n";
              dout_1 << "Received " <<  4*end2/32 << " bits, total = " << d_total_bits << "\n";
              dout_1 << "Throughput = " <<  throughput << "bits/sec\n";
 
              for (int i = 0; i < noutput_items; i++) {
                err[i] = err_rate;
                thr[i] = throughput;
                lat[i] = tot_lat;
              }
               
              consume(0, count1+end1);
              consume(1, count1+end1);
              consume(2, count2+end2);
              consume(3, count2+end2);
              
              if (DEBUG1) {
                dout_1 << "Payload 1: ";
                for (int i=0; i+32 < n; i+=64) {
                  dout_1 << (char) (((pckt1[i+32] < 10)? 48: 55) + pckt1[i+32]) <<(char) (((pckt1[i] < 10)? 48: 55) + pckt1[i]) << ' ';
                }
                dout_1 << '\n';
                dout_1 << "Payload 2: ";
                for (int i=0; i+32 < n; i+=64) {
                  dout_1 << (char) (((pckt2[i+32] < 10)? 48: 55) + pckt2[i+32]) <<(char) (((pckt2[i] < 10)? 48: 55) + pckt2[i]) << ' ';
                }
                dout_1 << "\n\n";
              }
              
            } else if (sn2 - sn1 < d_threshold && sn2 > sn1) {
              dout << "SN2 greater than SN1, changing reference.\n";
              consume(0, count1+end1);
              consume(1, count1+end1);
              d_have_sync = false;
            } else {
              dout << "SN2 corrupted, dropping packet.\n";
              consume(2, count2+end2);
              consume(3, count2+end2);
            }
              
          } else {
            dout << "Sync too short, dropping packet.\n";
            consume(2, count2+end2);
            consume(3, count2+end2);
          }
        }
      } else {
        consume(2, ninput2);
        consume(3, ninput2);
      }
      // Tell runtime system how many output items we produced.
      return n;
    }

  } /* namespace bar */
} /* namespace gr */

