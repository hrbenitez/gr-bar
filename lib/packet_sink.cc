/*
 * Copyright 2004,2013 Free Software Foundation, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITH
 OUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <ieee802_15_4/packet_sink.h>
#include <gnuradio/io_signature.h>
#include <cstdio>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include <cstring>
#include <gnuradio/blocks/count_bits.h>
#include <iostream>

using namespace gr::ieee802_15_4;

// very verbose output for almost each sample
#define VERBOSE 0
// less verbose output for higher level debugging
#define VERBOSE2 0

// this is the mapping between chips and symbols if we do
// a fm demodulation of the O-QPSK signal. Note that this
// is different than the O-QPSK chip sequence from the
// 802.15.4 standard since there there is a translation
// happening.
// See "CMOS RFIC Architectures for IEEE 802.15.4 Networks",
// John Notor, Anthony Caviglia, Gary Levy, for more details.
static unsigned int CHIP_MAPPING[] = {
					1618456172,
					1309113062,
					1826650030,
					1724778362,
					778887287,
					2061946375,
					2007919840,
					125494990,
					529027475,
					838370585,
					320833617,
					422705285,
					1368596360,
					85537272,
					139563807,
					2021988657};

static const int MAX_PKT_LEN    = 128 -  1; // remove header and CRC
static const int MAX_LQI_SAMPLES = 8; // Number of chip correlation samples to take

class packet_sink_impl : public packet_sink {
public:

void enter_search()
{
	if (VERBOSE)
		fprintf(stderr, "@ enter_search\n");

	d_state = STATE_SYNC_SEARCH;
	d_shift_reg = 0;
	d_preamble_cnt = 0;
	d_chip_cnt = 0;
	d_packet_byte = 0;
}
    
void enter_have_sync()
{
	if (VERBOSE)
		fprintf(stderr, "@ enter_have_sync\n");

	d_state = STATE_HAVE_SYNC;
	d_packetlen_cnt = 0;
	d_packet_byte = 0;
	d_packet_byte_index = 0;

	// Link Quality Information
	d_lqi = 0;
	d_lqi_sample_count = 0;
}

void enter_have_header(int payload_len)
{
	if (VERBOSE)
		fprintf(stderr, "@ enter_have_header (payload_len = %d)\n", payload_len);

	d_state = STATE_HAVE_HEADER;
	d_packetlen  = payload_len;
	d_payload_cnt = 0;
	d_packet_byte = 0;
	d_packet_byte_index = 0;
}


unsigned char decode_chips(unsigned int chips){
	int i;
	int best_match = 0xFF;
	int min_threshold = 33; // Matching to 32 chips, could never have a error of 33 chips

	for(i=0; i<16; i++) {
		// FIXME: we can store the last chip
		// ignore the first and last chip since it depends on the last chip.
		unsigned int threshold = gr::blocks::count_bits32((chips & 0x7FFFFFFE) ^ (CHIP_MAPPING[i] & 0x7FFFFFFE));

		if (threshold < min_threshold) {
			best_match = i;
			min_threshold = threshold;
		}
	}

	if (min_threshold < d_threshold) {
		if (VERBOSE)
			fprintf(stderr, "Found sequence with %d errors at 0x%x\n", min_threshold, (chips & 0x7FFFFFFE) ^ (CHIP_MAPPING[best_match] & 0x7FFFFFFE)), fflush(stderr);
		// LQI: Average number of chips correct * MAX_LQI_SAMPLES
		//
		
		if (d_lqi_sample_count < MAX_LQI_SAMPLES) {
			d_lqi += 32 - min_threshold;
			d_lqi_sample_count++;
		}
    
		return (char)best_match & 0xF;
	}

	return 0xFF;
}

int slice(float x) {
	return x > 0 ? 1 : 0;
}

packet_sink_impl(int threshold, int choice)
  : block ("packet_sink",
		   gr::io_signature::make(1, 1, sizeof(float)),
		   gr::io_signature::make(3, 3, sizeof(char))),
    d_threshold(threshold),
    d_choice(choice)
    
{
std::cout << choice;
if (d_choice == 1){
			CHIP_MAPPING[0] = 1618456172;
			CHIP_MAPPING[1] = 1309113062;
			CHIP_MAPPING[2] = 1826650030;
			CHIP_MAPPING[3] = 1724778362;
			CHIP_MAPPING[4] = 778887287;
			CHIP_MAPPING[5] = 2061946375;
			CHIP_MAPPING[6] = 2007919840;
			CHIP_MAPPING[7] = 125494990;
			CHIP_MAPPING[8] = 529027475;
			CHIP_MAPPING[9] = 838370585;
			CHIP_MAPPING[10] = 320833617;
			CHIP_MAPPING[11] = 422705285;
			CHIP_MAPPING[12] = 1368596360;
			CHIP_MAPPING[13] = 85537272;
			CHIP_MAPPING[14] = 139563807;
			CHIP_MAPPING[15] = 2021988657;
					}
		else {
			CHIP_MAPPING[0] = 3653456430;
			CHIP_MAPPING[1] = 3986437410;
			CHIP_MAPPING[2] = 786023250;
			CHIP_MAPPING[3] = 585997365;
			CHIP_MAPPING[4] = 1378802115;
			CHIP_MAPPING[5] = 891481500;
			CHIP_MAPPING[6] = 3276943065;
			CHIP_MAPPING[7] = 2620728045;
			CHIP_MAPPING[8] = 2358642555;
			CHIP_MAPPING[9] = 3100205175;
			CHIP_MAPPING[10] = 2072811015;
			CHIP_MAPPING[11] = 2008598880;
			CHIP_MAPPING[12] = 125537430;
			CHIP_MAPPING[13] = 1618458825;
			CHIP_MAPPING[14] = 2517072780;
			CHIP_MAPPING[15] = 3378542520;	
					}
	d_sync_vector = 0xA7;

	// Link Quality Information
	d_lqi = 0;
	d_lqi_sample_count = 0;

	if ( VERBOSE )
		fprintf(stderr, "syncvec: %x, threshold: %d\n", d_sync_vector, d_threshold),fflush(stderr);
	enter_search();

	message_port_register_out(pmt::mp("out"));
}

~packet_sink_impl()
{
}

int general_work(int noutput, gr_vector_int& ninput_items,
			gr_vector_const_void_star& input_items,
			gr_vector_void_star& output_items ) {

	const float *inbuf = (const float*)input_items[0];
        int ninput = ninput_items[0];
	int count=0, count_b=0, count_c=0;
	
	int i = 0;
  unsigned char *sync = (unsigned char *) output_items[0];
  unsigned char *bits = (unsigned char *) output_items[1];
  unsigned char *symb = (unsigned char *) output_items[2];
  memset(sync, 0, sizeof(unsigned char)*ninput);
  memset(bits, 0, sizeof(unsigned char)*ninput);
  memset(symb, 0, sizeof(unsigned char)*ninput);
  
  ///*
	if (VERBOSE)
		fprintf(stderr,">>> Entering state machine\n"),fflush(stderr);

	while(count < ninput) {
		switch(d_state) {

		case STATE_SYNC_SEARCH:    // Look for sync vector
			if (VERBOSE)
				fprintf(stderr,"SYNC Search, ninput=%d syncvec=%x\n", ninput, d_sync_vector),fflush(stderr);

			while (count < ninput) {
				if(slice(inbuf[count++]))
					d_shift_reg = (d_shift_reg << 1) | 1;
				else
					d_shift_reg = d_shift_reg << 1;

				if(d_preamble_cnt > 0){
					d_chip_cnt = d_chip_cnt+1;
				}
        *(sync + count - 1) = 0;
				// The first if block syncronizes to chip sequences.
				if(d_preamble_cnt == 0){
					unsigned int threshold;
					threshold = gr::blocks::count_bits32((d_shift_reg & 0x7FFFFFFE) ^ (CHIP_MAPPING[0] & 0x7FFFFFFE));
					if(threshold < d_threshold) {
						//  fprintf(stderr, "Threshold %d d_preamble_cnt: %d\n", threshold, d_preamble_cnt);
						//if ((d_shift_reg&0xFFFFFE) == (CHIP_MAPPING[0]&0xFFFFFE)) {
						if (VERBOSE2)
							fprintf(stderr,"Found 0 in chip sequence\n"),fflush(stderr);
						// we found a 0 in the chip sequence
						d_preamble_cnt+=1;
						//fprintf(stderr, "Threshold %d d_preamble_cnt: %d\n", threshold, d_preamble_cnt);
					}
				} else {
					// we found the first 0, thus we only have to do the calculation every 32 chips
					if(d_chip_cnt == 32){
						d_chip_cnt = 0;

						if(d_packet_byte == 0) {
							if (gr::blocks::count_bits32((d_shift_reg & 0x7FFFFFFE) ^ (CHIP_MAPPING[0] & 0xFFFFFFFE)) <= d_threshold) {
								if (VERBOSE2)
									fprintf(stderr,"Found %d 0 in chip sequence\n", d_preamble_cnt),fflush(stderr);
								// we found an other 0 in the chip sequence
								d_packet_byte = 0;
								d_preamble_cnt ++;
							} else if (gr::blocks::count_bits32((d_shift_reg & 0x7FFFFFFE) ^ (CHIP_MAPPING[7] & 0xFFFFFFFE)) <= d_threshold) {
								if (VERBOSE2)
									fprintf(stderr,"Found first SFD\n"),fflush(stderr);
								d_packet_byte = 7 << 4;
							} else {
								// we are not in the synchronization header
								if (VERBOSE2)
									fprintf(stderr, "Wrong first byte of SFD. %u\n", d_shift_reg), fflush(stderr);
								enter_search();
								break;
							}

						} else {
							if (gr::blocks::count_bits32((d_shift_reg & 0x7FFFFFFE) ^ (CHIP_MAPPING[10] & 0xFFFFFFFE)) <= d_threshold) {
								d_packet_byte |= 0xA;
								if (VERBOSE2)
									fprintf(stderr,"Found sync, 0x%x\n", d_packet_byte),fflush(stderr);
								// found SDF
								// setup for header decode
								count_b = count-1;
								enter_have_sync();
								break;
							} else {
								if (VERBOSE)
									fprintf(stderr, "Wrong second byte of SFD. %u\n", d_shift_reg), fflush(stderr);
								enter_search();
								break;
							}
						}
					}
				}
			}
			break;

		case STATE_HAVE_SYNC:
			if (VERBOSE2)
				fprintf(stderr,"Header Search bitcnt=%d, header=0x%08x\n", d_headerbitlen_cnt, d_header),
				fflush(stderr);

			while (count < ninput) {		// Decode the bytes one after another.
			  
				if(slice(inbuf[count++]))
					d_shift_reg = (d_shift_reg << 1) | 1;
				else
					d_shift_reg = d_shift_reg << 1;

				d_chip_cnt = d_chip_cnt+1;
        *(sync + count - 1) = 1;
        *(bits + count - 1) = d_shift_reg & 1;
				if(d_chip_cnt == 32){
					d_chip_cnt = 0;
					unsigned char c = decode_chips(d_shift_reg);
					/*
					if(c == 0xFF){
						// something is wrong. restart the search for a sync
						if(VERBOSE2)
							fprintf(stderr, "Found a not valid chip sequence! %u\n", d_shift_reg), fflush(stderr);
            count_c = count-1;
						enter_search();
						break;
					}
					//*/
          //*(symb + count - 1) = c;
					int temp = std::max(count - 33, 0);
					std::memset(symb + temp, c, 32);
					
					if(d_packet_byte_index == 0){
						d_packet_byte = c;
					} else {
						// c is always < 15
						d_packet_byte |= c << 4;
					}
					d_packet_byte_index = d_packet_byte_index + 1;
					if(d_packet_byte_index%2 == 0){
						// we have a complete byte which represents the frame length.
						int frame_len = d_packet_byte;
						if(frame_len <= MAX_PKT_LEN){
							enter_have_header(frame_len);
						} else {
						  enter_search();
						}
						break;
					}
				}
			}
			break;

		case STATE_HAVE_HEADER:
			if (VERBOSE2)
				fprintf(stderr,"Packet Build count=%d, ninput=%d, packet_len=%d\n", count, ninput, d_packetlen),fflush(stderr);

			while (count < ninput) {   // shift bits into bytes of packet one at a time
				if(slice(inbuf[count++]))
					d_shift_reg = (d_shift_reg << 1) | 1;
				else
					d_shift_reg = d_shift_reg << 1;

				d_chip_cnt = (d_chip_cnt+1)%32;
        *(sync + count - 1) = 1;
        *(bits + count - 1) = d_shift_reg & 1;
				if(d_chip_cnt == 0){
					unsigned char c = decode_chips(d_shift_reg);
					/*
					if(c == 0xff){
						// something is wrong. restart the search for a sync
						if(VERBOSE2)
							fprintf(stderr, "Found a not valid chip sequence! %u\n", d_shift_reg), fflush(stderr);

						enter_search();
						break;
					}
					//*/
					int temp = std::max(count - 33, 0);
					std::memset(symb + temp, c, 32);
					
					// the first symbol represents the first part of the byte.
					if(d_packet_byte_index == 0){
						d_packet_byte = c;
					} else {
						// c is always < 15
						d_packet_byte |= c << 4;
					}
					//fprintf(stderr, "%d: 0x%x\n", d_packet_byte_index, c);
					d_packet_byte_index = d_packet_byte_index + 1;
					if(d_packet_byte_index%2 == 0){
						// we have a complete byte
						if (VERBOSE2)
							fprintf(stderr, "packetcnt: %d, payloadcnt: %d, payload 0x%x, d_packet_byte_index: %d\n", d_packetlen_cnt, d_payload_cnt, d_packet_byte, d_packet_byte_index), fflush(stderr);

						d_packet[d_packetlen_cnt++] = d_packet_byte;
						d_payload_cnt++;
						d_packet_byte_index = 0;

						if (d_payload_cnt >= d_packetlen){	// packet is filled, including CRC. might do check later in here
							unsigned int scaled_lqi = (d_lqi / MAX_LQI_SAMPLES) << 3;
							unsigned char lqi = (scaled_lqi >= 256? 255 : scaled_lqi);

							pmt::pmt_t meta = pmt::make_dict();
							meta = pmt::dict_add(meta, pmt::mp("lqi"), pmt::from_long(lqi));

							std::memcpy(buf, d_packet, d_packetlen_cnt);
							pmt::pmt_t payload = pmt::make_blob(buf, d_packetlen_cnt);

							message_port_pub(pmt::mp("out"), pmt::cons(meta, payload));

							if(VERBOSE2)
								fprintf(stderr, "Adding message of size %d to queue\n", d_packetlen_cnt);
							count_c = count-1;
							enter_search();
							break;
						}
					}
				}
			}
			break;

		default:
			assert(0);
			break;

		}
	}
  
	if(VERBOSE2)
		fprintf(stderr, "Samples Processed: %d\n", ninput_items[0]), fflush(stderr);

  //memset(sync+(count_b*4), 1, sizeof(unsigned char)*(count_c-count_b)*4);
  //fprintf(stderr, "count_c: %d\ncount_b: %d\n", count_c, count_b), fflush(stderr);
  
  consume(0, ninput);
  return ninput;
}

private:
	enum {STATE_SYNC_SEARCH, STATE_HAVE_SYNC, STATE_HAVE_HEADER} d_state;

	unsigned int      d_sync_vector;           // 802.15.4 standard is 4x 0 bytes and 1x0xA7
	unsigned int      d_threshold;             // how many bits may be wrong in sync vector
  unsigned int      d_choice;                //modulation scheme 1 for oqpsk, 2 for ook
	unsigned int      d_shift_reg;             // used to look for sync_vector
	int               d_preamble_cnt;          // count on where we are in preamble
	int               d_chip_cnt;              // counts the chips collected

	unsigned int      d_header;                // header bits
	int               d_headerbitlen_cnt;      // how many so far

	unsigned char     d_packet[MAX_PKT_LEN];   // assembled payload
	unsigned char     d_packet_byte;           // byte being assembled
	int               d_packet_byte_index;     // which bit of d_packet_byte we're working on
	int               d_packetlen;             // length of packet
	int               d_packetlen_cnt;         // how many so far
	int               d_payload_cnt;           // how many bytes in payload

	unsigned int      d_lqi;                   // Link Quality Information
	unsigned int      d_lqi_sample_count;

	// FIXME:
	char buf[256];
};

packet_sink::sptr packet_sink::make(unsigned int threshold, unsigned int choice) {
	return gnuradio::get_initial_sptr(new packet_sink_impl(threshold, choice));
}
