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
#include "symbol_sync_impl.h"

#define DEBUG 0
#define dout DEBUG && std::cout

namespace gr {
  namespace bar {

    symbol_sync::sptr
    symbol_sync::make(int threshold, int packet_length)
    {
      return gnuradio::get_initial_sptr
        (new symbol_sync_impl(threshold, packet_length));
    }

    /*
     * The private constructor
     */
    symbol_sync_impl::symbol_sync_impl(int threshold, int packet_length)
      : gr::block("symbol_sync",
              gr::io_signature::make(2, 2, sizeof(char)),
              gr::io_signature::make(2, 2, sizeof(char))),
              d_packet_length(packet_length),
              d_threshold(threshold),
              d_have_sync(false)
    {}

    /*
     * Our virtual destructor.
     */
    symbol_sync_impl::~symbol_sync_impl()
    {
    }

    void
    symbol_sync_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    symbol_sync_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const char *sym0 = (const char *) input_items[0];
      const char *sym1 = (const char *) input_items[1];
      char *out0 = (char *) output_items[0];
      char *out1 = (char *) output_items[1];
      
      int n_sym0 = ninput_items[0];
      int n_sym1 = ninput_items[1];
      
      if (n_sym0 > 0 && n_sym1 > 0) {
        int sn0 = sym0[4]*16 + sym0[5], sn1 = sym1[4]*16 + sym1[5];
        dout << "SN: " << sn0 << ", " << sn1 << "\n";
        if (sn0 == sn1) {
          std::memset(out0, 0, d_packet_length * sizeof(char));
          std::memset(out1, 0, d_packet_length * sizeof(char));
          
          std::memcpy(out0, sym0, std::min(d_packet_length, n_sym0) * sizeof(char));
          std::memcpy(out1, sym1, std::min(d_packet_length, n_sym1) * sizeof(char));
          
          consume(0, std::min(d_packet_length, n_sym0));
          consume(1, std::min(d_packet_length, n_sym1));
          return d_packet_length;
        } else if (sn1 - sn0 < d_threshold && sn1 > sn0) {
          dout << "SN2 greater than SN1, changing reference.\n";
          consume(0, d_packet_length);
          return 0;
        } else {
          dout << "SN2 corrupted, dropping packet.\n";
          consume(1, n_sym1);
          return 0;
        }
      }
      // Tell runtime system how many output items we produced.
      return 0;
    }

  } /* namespace bar */
} /* namespace gr */

