# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND bar_sources
    try_ff_impl.cc
    packet_checker_impl.cc
    snr_impl.cc
    out_sync_impl.cc
    symbol_sync_impl.cc
    packet_former_cc_impl.cc )

set(bar_sources "${bar_sources}" PARENT_SCOPE)
if(NOT bar_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT bar_sources)

add_library(gnuradio-bar SHARED ${bar_sources})
target_link_libraries(gnuradio-bar ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-bar PROPERTIES DEFINE_SYMBOL "gnuradio_bar_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-bar PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-bar RUNTIME_COMPONENT "bar_runtime" DEVEL_COMPONENT "bar_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_bar_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_bar.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_bar.cc
)

add_executable(test-bar ${test_bar_sources})

target_link_libraries(
  test-bar
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-bar
)

GR_ADD_TEST(test_bar test-bar)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

