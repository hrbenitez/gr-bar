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
#include "packet_former_cc_impl.h"

#define DEBUG 1
#define dout DEBUG && std::cout

namespace gr {
  namespace bar {

    packet_former_cc::sptr
    packet_former_cc::make(int scale)
    {
      return gnuradio::get_initial_sptr
        (new packet_former_cc_impl(scale));
    }

    /*
     * The private constructor
     */
    packet_former_cc_impl::packet_former_cc_impl(int scale)
      : gr::block("packet_former_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_scale(scale),
              d_waiting(false),
              d_ninput(0),
              d_val(0)
    {}

    /*
     * Our virtual destructor.
     */
    packet_former_cc_impl::~packet_former_cc_impl()
    {
    }

    void
    packet_former_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
      
    }

    int
    packet_former_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      
      int ninput = ninput_items[0];
	    int noutput = noutput_items;
      
      const uint64_t nread = this->nitems_read(0);
	    std::vector<gr::tag_t> tags;
	    get_tags_in_range(tags, 0, nread, nread + ninput);
	    std::sort(tags.begin(), tags.end(), tag_t::offset_compare);
      
      if(tags.size()) {
        tag_t t = tags[0];
        std::string n = pmt::symbol_to_string(t.key);
        d_val = d_scale * pmt::to_uint64(t.value);
        
        if (!d_waiting) {
          dout << "found tag: " << n << " = "<< d_val << std::endl;
          if (n == "pdu_length") {
            if (ninput < d_val) {
              if (!d_waiting) {
                dout << "Input too few, waiting...\n";
                d_ninput = ninput;
                
                dout << "Creating temporary buffer...\n";
                temp = new gr_complex[d_val];
                memcpy(temp, in, ninput*sizeof(gr_complex));
                dout << "Added " << ninput << " to buffer.\n";
                
                d_waiting = true;
              }
              consume_each (ninput);
              return 0;
            } else {
                dout << "Tagged stream complete, output " << d_val << "\n";
                d_waiting = false;
                d_ninput = 0;
                memcpy(out, in, d_val*sizeof(gr_complex));
                consume_each (d_val);
                return d_val;
            }
          }
        } else {
          if (d_ninput + ninput >= d_val) {
            int diff = d_val - d_ninput;
            memcpy(temp + d_ninput, in, diff*sizeof(gr_complex));
            dout << "Added " << diff << " to buffer, total: " << d_val << "\n";
            
            if (0) for (int i = 0; i < d_val; i++) {
              dout << "temp[" << i << "] = " << temp[i] << "\n";
            }
            
            d_waiting = false;
            d_ninput = 0;
            dout << "Copying temp to output stream\n";
            memcpy(out, temp, noutput*sizeof(gr_complex));
            dout << "Tagged stream complete, output " << d_val << "\n";
            delete[] temp;
            dout << "Temporary buffer deleted \n";
            consume_each (diff);
            return noutput;
            
          } else {
            memcpy(temp + d_ninput, in, ninput*sizeof(gr_complex));
            d_ninput += ninput;
            dout << "Added " << ninput << " to buffer, total: " << d_ninput << "\n";
            consume_each (ninput);
            return 0;
          }
        }  
      }
      
      consume_each (noutput);
      return noutput;
    }

  } /* namespace bar */
} /* namespace gr */

