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


#ifndef INCLUDED_BAR_PACKET_CHECKER_H
#define INCLUDED_BAR_PACKET_CHECKER_H

#include <bar/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace bar {

    class BAR_API packet_checker : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<packet_checker> sptr;

      static sptr make(unsigned long int samp_rate, unsigned int threshold, unsigned int choice);
    };

  } // namespace bar
} // namespace gr

#endif /* INCLUDED_BAR_PACKET_CHECKER_H */

