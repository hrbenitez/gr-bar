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
#include "out_sync_impl.h"

#define DEBUG 0
#define dout DEBUG && std::cout

namespace gr {
  namespace bar {

    out_sync::sptr
    out_sync::make(unsigned int threshold)
    {
      return gnuradio::get_initial_sptr
        (new out_sync_impl(threshold));
    }

    /*
     * The private constructor
     */
    out_sync_impl::out_sync_impl(unsigned int threshold)
      : gr::block("out_sync",
              gr::io_signature::make(6, 6, sizeof(char)),
              gr::io_signature::make(4, 4, sizeof(char))),
              d_threshold(threshold),
              d_have_sync(false)
    {}

    /*
     * Our virtual destructor.
     */
    out_sync_impl::~out_sync_impl()
    {
      
    }

    namespace os {
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
      
      void clean_symb (char* in, int n) {
        char tmp = 0;
        for (int i = 0; i+32 <= n; i += 32) {
          tmp = *std::max_element(in + i, in + i + 31);
          for (int j = 0; j < 32; j++) in[i+j] = tmp;
        }
      }
    }

    void
    out_sync_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    out_sync_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      using namespace os;
      const char *bit1 = (const char *) input_items[0];
      const char *sym1 = (const char *) input_items[1];
      const char *sync1 = (const char *) input_items[2];
      const char *bit2 = (const char *) input_items[3];
      const char *sym2 = (const char *) input_items[4];
      const char *sync2 = (const char *) input_items[5];
      char *o_bit1 = (char *) output_items[0];
      char *o_sym1 = (char *) output_items[1];
      char *o_bit2 = (char *) output_items[2];
      char *o_sym2 = (char *) output_items[3];
      
      int n_in1 = std::min(ninput_items[2], std::min(ninput_items[1], ninput_items[0]));
      int n_in2 = std::min(ninput_items[5], std::min(ninput_items[4], ninput_items[3]));
      
      int n = 0;  
      int count1 = find_sync_start(sync1, n_in1);
      int end1 = n_in1;
      if (count1 >= 0) {
        end1 = find_sync_end(sync1+count1, n_in1-count1);
        if (!d_have_sync) {
          d_have_sync = true;
          
          dout << "Sync 1 at " << count1 << " of " << n_in1 << '\n';
          dout << "Sync 1 end at " << count1 + n_in1 << ", total: " << end1 << '\n';
        }
      } else {
        consume(0, n_in1);
        consume(1, n_in1);
        consume(2, n_in1);
      }
      
      int count2 = find_sync_start(sync2, n_in2);
      int end2 = n_in2;
      if (count2 >= 0) {
        if (d_have_sync) {
          end2 = find_sync_end(sync2+count2, n_in2-count2);
          if (count2+end2 == n_in2) {
            dout << "Sync at end of buffer, wait for more\n";
            return 0;
          }
          if (end2 > 256) {
            dout << "Sync 2 at " << count2 << " of " << n_in2 << '\n';
            dout << "Sync 2 end at " << count2 + end2 << ", total: " << end2 << '\n';
            
            n = std::max(end1, end2);
            
            const char *pckt1 = sym1+count1, *pckt2 = sym2+count2;
            
            int sn1 = (*std::max_element(pckt1 + 224, pckt1 + 255)) * 16 + (*std::max_element(pckt1 + 192, pckt1 + 223));
            int sn2 = (*std::max_element(pckt2 + 224, pckt2 + 255)) * 16 + (*std::max_element(pckt2 + 192, pckt2 + 223));
            
            if (sn1 == sn2) {
              d_have_sync = false;
              dout << "SN 1 = " << sn1 << ", SN 2 = " << sn2 << "\n";
              
              std::memcpy(o_sym1, pckt1, (n-32) * sizeof(char));
              std::memcpy(o_sym2, pckt2, (n-32) * sizeof(char));
              std::memcpy(o_bit1, bit1+count1, (n-32) * sizeof(char));
              std::memcpy(o_bit2, bit2+count2, (n-32) * sizeof(char));
              clean_symb(o_sym1, n);
              clean_symb(o_sym2, n);
               
              consume(0, count1+end1);
              consume(1, count1+end1);
              consume(2, count1+end1);
              consume(3, count2+end2);
              consume(4, count2+end2);
              consume(5, count2+end2);
                            
              if (DEBUG) {
                dout << "Payload 1: ";
                for (int i=0; i+32 < n; i+=64) {
                  dout << (char) (((pckt1[i+32] < 10)? 48: 55) + pckt1[i+32]) <<(char) (((pckt1[i] < 10)? 48: 55) + pckt1[i]) << ' ';
                }
                dout << '\n';
                dout << "Payload 2: ";
                for (int i=0; i+32 < n; i+=64) {
                  dout << (char) (((pckt2[i+32] < 10)? 48: 55) + pckt2[i+32]) <<(char) (((pckt2[i] < 10)? 48: 55) + pckt2[i]) << ' ';
                }
                dout << "\n\n";
              }
              
            } else if (sn2 - sn1 < d_threshold && sn2 > sn1) {
              dout << "SN2 greater than SN1, changing reference.\n";
              consume(0, count1+end1);
              consume(1, count1+end1);
              consume(2, count1+end1);
              d_have_sync = false;
              return 0;
            } else {
              dout << "SN2 corrupted, dropping packet.\n";
              consume(3, count2+end2);
              consume(4, count2+end2);
              consume(5, count2+end2);
              return 0;
            }
              
          } else {
            dout << "Sync too short, dropping packet.\n";
            consume(3, count2+end2);
            consume(4, count2+end2);
            consume(5, count2+end2);
            return 0;
          }
        }
      } else {
        consume(3, n_in2);
        consume(4, n_in2);
        consume(5, n_in2);
        return 0;
      }
      // Tell runtime system how many output items we produced.
      return n;
    }

  } /* namespace bar */
} /* namespace gr */

