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
#include "try_ff_impl.h"

#define DEBUG 0
#define dout DEBUG && std::cout

namespace gr {
  namespace bar {

    try_ff::sptr
    try_ff::make()
    {
      return gnuradio::get_initial_sptr
        (new try_ff_impl());
    }

    /*
     * The private constructor
     */
    try_ff_impl::try_ff_impl()
      : gr::block("try_ff",
              gr::io_signature::make(4, 4, sizeof(char)),
              gr::io_signature::make(2, 2, sizeof(char)))
    {}

    /*
     * Our virtual destructor.
     */
    try_ff_impl::~try_ff_impl()
    {
    }

    void
    try_ff_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }
  
    int find_sync_start (const char *sync, int n) {
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
    int find_sync_end (const char *sync, int n) {
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
    
    int
    try_ff_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *in1 = (const char *) input_items[0];
      const char *sync1 = (const char *) input_items[1];
      const char *in2 = (const char *) input_items[2];
      const char *sync2 = (const char *) input_items[3];
      char *out1 = (char *) output_items[0];
      char *out2 = (char *) output_items[1];
      
      int ninput1 = std::min(ninput_items[0], ninput_items[1]);
      int ninput2 = std::min(ninput_items[2], ninput_items[3]);
      
      int n = 0;  
      int count2 = find_sync_start(sync2, ninput2);
      int end2 = ninput2;
      if (count2 >= 0) {
        dout << "Sync 2 at with sync2[" << count2 << "] = " << (int) sync2[count2] << '\n';
        end2 = find_sync_end(sync2+count2, ninput2-count2);
        dout << "Sync 2 end at " << count2 + end2 << ", total: " << end2 << '\n';
      } else {
        consume(2, ninput2);
        consume(3, ninput2);
      }
      
      int count1 = find_sync_start(sync1, ninput1);
      int end1 = ninput1;
      if (count1 >= 0) {
        dout << "Sync 1 at with sync1[" << count1 << "] = " << (int) sync1[count1] << '\n';
        end1 = find_sync_end(sync1+count1, ninput1-count1);
        dout << "Sync 1 end at " << count1 + end1 << ", total: " << end1 << '\n';
        if (count2 >= 0) {
          n = std::max(end1, end2);
          dout << "Have both sync, output: " << n << '\n';
          
          std::memcpy(out1, in1+count1, n * sizeof(char));
          std::memcpy(out2, in2+count2, n * sizeof(char));
          
          consume(0, ninput1);
          consume(1, ninput1);
          consume(2, ninput2);
          consume(3, ninput2);
        } 
      } else {
        consume(0, ninput1);
        consume(1, ninput1);
      }
      
      
      
     
      // Tell runtime system how many output items we produced.
      return n;
    }

  } /* namespace bar */
} /* namespace gr */

