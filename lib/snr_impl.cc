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
#include "snr_impl.h"

#define DEBUG 0
#define VERBOSE 0
#define dout DEBUG && std::cout
#define vout VERBOSE && DEBUG && std::cout

namespace gr {
  namespace bar {

    snr::sptr
    snr::make(float threshold)
    {
      return gnuradio::get_initial_sptr
        (new snr_impl(threshold));
    }

    /*
     * The private constructor
     */
    snr_impl::snr_impl(float threshold)
      : gr::block("snr",
              gr::io_signature::make(2, 2, sizeof(float)),
              gr::io_signature::make(2, 2, sizeof(float))),
              d_n_sync(0),
              d_n_s_re(0),
              d_noise_threshold(threshold)
    {}

    /*
     * Our virtual destructor.
     */
    snr_impl::~snr_impl()
    {
    }

    void
    snr_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }
    
namespace signal_part {
    template <typename T>
    int find_sync_start (const T *sync, int n) {
      int i = 0;
      while(i < n) {
        if (sync[i] > 0) {
          dout << std::endl;
          return i;
        }
        i++;
      }
      return -1;
    }

    int find_sync_end (const float *sync, int n) {
      int i = 0;
      while(i < n) {
        if (sync[i] == 0) {
          dout << std::endl;
          return i;
        }
        i++;
      }
      return i;
    }
}
using namespace signal_part;
    int
    snr_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const float *in1 = (const float *) input_items[0]; //real
      const float *sync = (const float *) input_items[1];
      //const float *in2 = (const float *) input_items[2]; //imaginary
      
      float *s_re = (float *) output_items[0];
      float *n_re = (float *) output_items[1];
      //float *s_im = (float *) output_items[2];
      //float *n_im = (float *) output_items[3];
      
      int n_s_re = ninput_items[0];
      int n_sync = ninput_items[1];
      //int n_s_im = ninput_items[2];
      int ninput = std::min(n_s_re, n_sync);
      
      int n = 0;
      if (n_s_re != d_n_s_re || n_sync != d_n_sync) {
        vout << "\t\tn_s_re = " << n_s_re << ", n_sync = " << n_sync << "\n";
        d_n_s_re = n_s_re;
        d_n_sync = n_sync;
      }
      int count = find_sync_start(sync, ninput);
      int end = ninput;
      if (count >= 0) {
        dout << "Sync 1 at " << count << " of " << n_sync << "\n";
        end = find_sync_end(sync+count, ninput-count);
        //dout << "Sync 1 end at " << count + end << ", total: " << end << ", need: " << noutput_items << '\n';
        if (end > 512) {
          n = std::min(end, noutput_items)/2;
          //dout << "Estimate signal at " << count << " of " << n_s_re << "\n";
          
          //const float *pckt1 = in1+(count*2);
          
          /*
          std::memcpy(s_re, in1+(count), n * sizeof(float));
          std::memcpy(n_re, in1+((count+n)*2)+n, n * sizeof(float)); //noise real
          //*/
          auto min_add = s_re - n_re;
          dout << "add_dif = " << min_add << ", sending = " << n << "\n";
          for (int i = 0; i < n; i++) {
            s_re[i] = in1[(count + i) % n_s_re];
            n_re[i] = in1[(count + end + i + 6144) % n_s_re];
            if (n_re[i]*(d_noise_threshold)  > s_re[i]) n = i;
          }
          dout << "sending sig = " << s_re[n/2]*1000.0 << ", noise = " << n_re[n/2]*1000.0 << "\n";
          
          consume(0, ninput);
          consume(1, ninput);
          vout << "\t\t\t\t consume n_s_re = " << ninput << ", consume n_sync = " << ninput << "\n";
        } else {
          dout << "Sync too short, dropping packet.\n";
          vout << "\t\t\t\t consume n_s_re = " << ninput << ", consume n_sync = " << ninput << "\n";
          consume(0, ninput);
          consume(1, ninput);
          return 0;
        }
      } else if (n_sync > 0) {
        vout << "No sync found\n";
        vout << "\t\t\t\t consume n_s_re = " << ninput << ", consume n_sync = " << ninput << "\n";
        consume(0, ninput);
        consume(1, ninput);
        return 0;
      }
      
      return n;
    }

  } /* namespace bar */
} /* namespace gr */
