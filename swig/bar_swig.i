/* -*- c++ -*- */

#define BAR_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "bar_swig_doc.i"

%{
#include "bar/try_ff.h"
#include "bar/packet_checker.h"
#include "bar/snr.h"
#include "bar/out_sync.h"
#include "bar/symbol_sync.h"
#include "bar/packet_former_cc.h"
%}


%include "bar/try_ff.h"
GR_SWIG_BLOCK_MAGIC2(bar, try_ff);
%include "bar/packet_checker.h"
GR_SWIG_BLOCK_MAGIC2(bar, packet_checker);
%include "bar/snr.h"
GR_SWIG_BLOCK_MAGIC2(bar, snr);

%include "bar/out_sync.h"
GR_SWIG_BLOCK_MAGIC2(bar, out_sync);
%include "bar/symbol_sync.h"
GR_SWIG_BLOCK_MAGIC2(bar, symbol_sync);
%include "bar/packet_former_cc.h"
GR_SWIG_BLOCK_MAGIC2(bar, packet_former_cc);
