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

#ifndef INCLUDED_BAR_PACKET_CHECKER_IMPL_H
#define INCLUDED_BAR_PACKET_CHECKER_IMPL_H

#include <bar/packet_checker.h>
#include <chrono>

namespace gr {
  namespace bar {

    class packet_checker_impl : public packet_checker
    {
     private:
      std::chrono::microseconds d_proc_overhead;
      int d_received_packets;
      int d_sent_packets;
      int d_samp_rate;
      int d_threshold;
      int d_lat_choice;
      bool d_have_sync;
      std::chrono::high_resolution_clock::time_point d_sync_clock;
      std::chrono::high_resolution_clock::time_point d_recv_clock;
      std::chrono::high_resolution_clock::time_point d_start_clock;
      unsigned long long d_total_bits;

     public:
      packet_checker_impl(unsigned long int samp_rate, unsigned int threshold, unsigned int choice);
      ~packet_checker_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace bar
} // namespace gr

#endif /* INCLUDED_BAR_PACKET_CHECKER_IMPL_H */

