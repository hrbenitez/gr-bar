#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: IEEE 802.15.4 Transceiver using OQPSK PHY
# Generated: Thu Mar 14 20:24:03 2019
##################################################

# Usage: 
# python2 full_setup.py <frequency> <distance> <rx gain> <tx attenuation> <rx id> <tx id> <modulation> <threshold>
# distance - not really necessary, for directory/filename purposes
# threshold - only necessary for OOK measurements

from subprocess import call as sp_call
from sys import argv

if argv[1] == '430':
  d_freq = 430000000
elif argv[1] == '915':
  d_freq = 915000000
else:
  d_freq = 2450000000
  
d_dist = argv[2]
d_rxdb = argv[3]
d_txdb = argv[4]
d_dir = './bin_files/' + str(argv[1]) + '_' + str(d_dist) + 'cm_' + str(d_rxdb) + 'rx_' + str(d_txdb) + 'tx'

sp_call(['mkdir', d_dir])

d_rxid = argv[5]
d_txid = argv[6]

print "\nTesting: " + d_dir

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from PyQt5 import Qt
from PyQt5 import Qt, QtCore
from PyQt5.QtCore import QObject, pyqtSlot
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import iio
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from grc_gnuradio import blks2 as grc_blks2
from ieee802_15_4_oqpsk_phy import ieee802_15_4_oqpsk_phy  # grc-generated hier_block
from optparse import OptionParser
import bar
import foo
import ieee802_15_4
import pmt
import sip
from gnuradio import qtgui


class full_setup(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "IEEE 802.15.4 Transceiver using OQPSK PHY")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("IEEE 802.15.4 Transceiver using OQPSK PHY")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "full_setup")

        if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
            self.restoreGeometry(self.settings.value("geometry").toByteArray())
        else:
            self.restoreGeometry(self.settings.value("geometry", type=QtCore.QByteArray))

        ##################################################
        # Variables
        ##################################################
        self.tx_gain = tx_gain = float(d_txdb)
        self.samp = samp = 4
        self.rx_gain = rx_gain = float(d_rxdb)
        self.freq = freq = float(d_freq)
        self.buf = buf = 0x8000
        self.bits = bits = 16

        ##################################################
        # Blocks
        ##################################################
        self._tx_gain_range = Range(-20, 64, 0.01, float(d_txdb), 200)
        self._tx_gain_win = RangeWidget(self._tx_gain_range, self.set_tx_gain, "tx_gain", "counter_slider", float)
        self.top_layout.addWidget(self._tx_gain_win)
        self._rx_gain_range = Range(0, 72, 0.01, float(d_rxdb), 200)
        self._rx_gain_win = RangeWidget(self._rx_gain_range, self.set_rx_gain, "rx_gain", "counter_slider", float)
        self.top_layout.addWidget(self._rx_gain_win)
        self._freq_options = [2450000000, 915000000, 430000000]
        self._freq_labels = ['2.45GHz', '915MHz', '430MHz']
        self._freq_tool_bar = Qt.QToolBar(self)
        self._freq_tool_bar.addWidget(Qt.QLabel('Channel'+": "))
        self._freq_combo_box = Qt.QComboBox()
        self._freq_tool_bar.addWidget(self._freq_combo_box)
        for label in self._freq_labels: self._freq_combo_box.addItem(label)
        self._freq_callback = lambda i: Qt.QMetaObject.invokeMethod(self._freq_combo_box, "setCurrentIndex", Qt.Q_ARG("int", self._freq_options.index(i)))
        self._freq_callback(self.freq)
        self._freq_combo_box.currentIndexChanged.connect(
        	lambda i: self.set_freq(self._freq_options[i]))
        self.top_layout.addWidget(self._freq_tool_bar)
        self._bits_range = Range(1, 127, 1, 16, 200)
        self._bits_win = RangeWidget(self._bits_range, self.set_bits, "bits", "counter_slider", int)
        self.top_layout.addWidget(self._bits_win)
        self.qtgui_number_sink_0 = qtgui.number_sink(
            gr.sizeof_float,
            0,
            qtgui.NUM_GRAPH_HORIZ,
            6
        )
        self.qtgui_number_sink_0.set_update_time(0.10)
        self.qtgui_number_sink_0.set_title("")

        labels = ['Error Rate', 'Throughput', 'Latency', 'Bit Error Rate', 'Symbol Error Rate',
                  'SNR', '', '', '', '']
        units = ['', 'bits/sec', 'ms', '', '',
                 'dB', '', '', '', '']
        colors = [("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"),
                  ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black")]
        factor = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        for i in xrange(6):
            self.qtgui_number_sink_0.set_min(i, 0)
            self.qtgui_number_sink_0.set_max(i, 1)
            self.qtgui_number_sink_0.set_color(i, colors[i][0], colors[i][1])
            if len(labels[i]) == 0:
                self.qtgui_number_sink_0.set_label(i, "Data {0}".format(i))
            else:
                self.qtgui_number_sink_0.set_label(i, labels[i])
            self.qtgui_number_sink_0.set_unit(i, units[i])
            self.qtgui_number_sink_0.set_factor(i, factor[i])

        self.qtgui_number_sink_0.enable_autoscale(True)
        self._qtgui_number_sink_0_win = sip.wrapinstance(self.qtgui_number_sink_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_number_sink_0_win)
        self.pluto_source_0 = iio.pluto_source(d_rxid, int(freq), int(4e6), int(20000000), buf*16, True, True, True, "manual", rx_gain, '', True)
        self.pluto_sink_0 = iio.pluto_sink(d_txid, int(freq), int(4e6), int(20000000), buf, False, tx_gain, '', True)
        self.ieee802_15_4_rime_stack_0 = ieee802_15_4.rime_stack(([129]), ([131]), ([132]), ([23,42]))
        
        self.ieee802_15_4_oqpsk_phy_0_0 = ieee802_15_4_oqpsk_phy()
        self.ieee802_15_4_oqpsk_phy_0 = ieee802_15_4_oqpsk_phy()
        if argv[7] == 'ook':
          exec "d_thr = " + argv[8] in locals()
          self.ieee802_15_4_phy_0_0 = ieee802_15_4_ook_phy(threshold=d_thr)
          self.ieee802_15_4_phy_0 = ieee802_15_4_ook_phy(threshold=d_thr)
          print "Modulation: OOK\nThreshold: " + str(d_thr) + "\n"
        elif argv[7] == 'fsk':
          self.ieee802_15_4_phy_0_0 = ieee802_15_4_cpfsk_phy()
          self.ieee802_15_4_phy_0 = ieee802_15_4_cpfsk_phy()
          print "Modulation: FSK\n"
        
        self.ieee802_15_4_mac_0 = ieee802_15_4.mac(False,0x8841,0,0x1aaa,0xffff,0x3344)
        self.foo_wireshark_connector_0 = foo.wireshark_connector(195, False)
        self.foo_packet_pad_0 = foo.packet_pad(False, False, 0.001, 0, buf)
        self.blocks_threshold_ff_0 = blocks.threshold_ff(10, 20, 0)
        self.blocks_socket_pdu_0_0 = blocks.socket_pdu("UDP_SERVER", '', '52001', 10000, False)
        self.blocks_repeat_0 = blocks.repeat(gr.sizeof_float*1, 2)
        self.blocks_null_sink_0_0_0 = blocks.null_sink(gr.sizeof_float*1)
        self.blocks_null_sink_0_0 = blocks.null_sink(gr.sizeof_gr_complex*1)
        self.blocks_nlog10_ff_0 = blocks.nlog10_ff(10, 1, 0)
        self.blocks_moving_average_xx_0_0 = blocks.moving_average_ff((1024+64*bits)*10, 1, (1024+64*bits)*11)
        self.blocks_moving_average_xx_0 = blocks.moving_average_ff((1024+64*bits)*10, 1, (1024+64*bits)*11)
        self.blocks_message_strobe_0_0 = blocks.message_strobe(pmt.intern(''.join(chr(e) for e in range(65, 65+bits))), 500)
        self.blocks_file_sink_1_1 = blocks.file_sink(gr.sizeof_float*1, d_dir + '/thr.bin', False)
        self.blocks_file_sink_1_1.set_unbuffered(False)
        self.blocks_file_sink_1_0_1 = blocks.file_sink(gr.sizeof_float*1, d_dir + '/lat.bin', False)
        self.blocks_file_sink_1_0_1.set_unbuffered(False)
        self.blocks_file_sink_1_0_0_0 = blocks.file_sink(gr.sizeof_float*1, d_dir + '/snr.bin', False)
        self.blocks_file_sink_1_0_0_0.set_unbuffered(False)
        self.blocks_file_sink_1_0_0 = blocks.file_sink(gr.sizeof_float*1, d_dir + '/per.bin', False)
        self.blocks_file_sink_1_0_0.set_unbuffered(False)
        self.blocks_file_sink_1_0 = blocks.file_sink(gr.sizeof_float*1, d_dir + '/ser.bin', False)
        self.blocks_file_sink_1_0.set_unbuffered(False)
        self.blocks_file_sink_1 = blocks.file_sink(gr.sizeof_float*1, d_dir + '/ber.bin', False)
        self.blocks_file_sink_1.set_unbuffered(False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, d_dir + '/output_padded.pcap', False)
        self.blocks_file_sink_0.set_unbuffered(True)
        self.blocks_divide_xx_0 = blocks.divide_ff(1)
        self.blocks_complex_to_mag_squared_0 = blocks.complex_to_mag_squared(1)
        self.blocks_char_to_float_0_1_0 = blocks.char_to_float(1, 1)
        self.blks2_error_rate_0_0 = grc_blks2.error_rate(
        	type='SER',
        	win_size=2**20,
        	bits_per_symbol=2,
        )
        self.blks2_error_rate_0 = grc_blks2.error_rate(
        	type='BER',
        	win_size=2**20,
        	bits_per_symbol=1,
        )
        self.bar_snr_0_0 = bar.snr(5)
        self.bar_snr_0 = bar.snr(0)
        self.bar_packet_checker_0 = bar.packet_checker(500, 10, 2)
        self.bar_out_sync_0 = bar.out_sync(20)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_message_strobe_0_0, 'strobe'), (self.ieee802_15_4_rime_stack_0, 'bcin'))
        self.msg_connect((self.blocks_socket_pdu_0_0, 'pdus'), (self.ieee802_15_4_rime_stack_0, 'bcin'))
        self.msg_connect((self.ieee802_15_4_mac_0, 'pdu out'), (self.ieee802_15_4_oqpsk_phy_0, 'txin'))
        self.msg_connect((self.ieee802_15_4_mac_0, 'app out'), (self.ieee802_15_4_rime_stack_0, 'fromMAC'))
        self.msg_connect((self.ieee802_15_4_oqpsk_phy_0_0, 'rxout'), (self.foo_wireshark_connector_0, 'in'))
        self.msg_connect((self.ieee802_15_4_oqpsk_phy_0_0, 'rxout'), (self.ieee802_15_4_mac_0, 'pdu in'))
        self.msg_connect((self.ieee802_15_4_rime_stack_0, 'bcout'), (self.blocks_socket_pdu_0_0, 'pdus'))
        self.msg_connect((self.ieee802_15_4_rime_stack_0, 'toMAC'), (self.ieee802_15_4_mac_0, 'app in'))
        self.connect((self.bar_out_sync_0, 0), (self.blks2_error_rate_0, 0))
        self.connect((self.bar_out_sync_0, 2), (self.blks2_error_rate_0, 1))
        self.connect((self.bar_out_sync_0, 1), (self.blks2_error_rate_0_0, 0))
        self.connect((self.bar_out_sync_0, 3), (self.blks2_error_rate_0_0, 1))
        self.connect((self.bar_packet_checker_0, 2), (self.bar_snr_0, 0))
        self.connect((self.bar_packet_checker_0, 0), (self.blocks_file_sink_1_0_0, 0))
        self.connect((self.bar_packet_checker_0, 1), (self.blocks_file_sink_1_1, 0))
        self.connect((self.bar_packet_checker_0, 2), (self.blocks_threshold_ff_0, 0))
        self.connect((self.bar_packet_checker_0, 0), (self.qtgui_number_sink_0, 0))
        self.connect((self.bar_packet_checker_0, 1), (self.qtgui_number_sink_0, 1))
        self.connect((self.bar_snr_0, 0), (self.blocks_file_sink_1_0_1, 0))
        self.connect((self.bar_snr_0, 1), (self.blocks_null_sink_0_0_0, 0))
        self.connect((self.bar_snr_0, 0), (self.qtgui_number_sink_0, 2))
        self.connect((self.bar_snr_0_0, 1), (self.blocks_moving_average_xx_0, 0))
        self.connect((self.bar_snr_0_0, 0), (self.blocks_moving_average_xx_0_0, 0))
        self.connect((self.blks2_error_rate_0, 0), (self.blocks_file_sink_1, 0))
        self.connect((self.blks2_error_rate_0, 0), (self.qtgui_number_sink_0, 3))
        self.connect((self.blks2_error_rate_0_0, 0), (self.blocks_file_sink_1_0, 0))
        self.connect((self.blks2_error_rate_0_0, 0), (self.qtgui_number_sink_0, 4))
        self.connect((self.blocks_char_to_float_0_1_0, 0), (self.blocks_repeat_0, 0))
        self.connect((self.blocks_complex_to_mag_squared_0, 0), (self.bar_snr_0_0, 0))
        self.connect((self.blocks_divide_xx_0, 0), (self.blocks_nlog10_ff_0, 0))
        self.connect((self.blocks_moving_average_xx_0, 0), (self.blocks_divide_xx_0, 1))
        self.connect((self.blocks_moving_average_xx_0_0, 0), (self.blocks_divide_xx_0, 0))
        self.connect((self.blocks_nlog10_ff_0, 0), (self.blocks_file_sink_1_0_0_0, 0))
        self.connect((self.blocks_nlog10_ff_0, 0), (self.qtgui_number_sink_0, 5))
        self.connect((self.blocks_repeat_0, 0), (self.bar_snr_0_0, 1))
        self.connect((self.blocks_threshold_ff_0, 0), (self.bar_snr_0, 1))
        self.connect((self.foo_packet_pad_0, 0), (self.ieee802_15_4_oqpsk_phy_0, 0))
        self.connect((self.foo_packet_pad_0, 0), (self.pluto_sink_0, 0))
        self.connect((self.foo_wireshark_connector_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.ieee802_15_4_oqpsk_phy_0, 2), (self.bar_out_sync_0, 0))
        self.connect((self.ieee802_15_4_oqpsk_phy_0, 3), (self.bar_out_sync_0, 1))
        self.connect((self.ieee802_15_4_oqpsk_phy_0, 1), (self.bar_out_sync_0, 2))
        self.connect((self.ieee802_15_4_oqpsk_phy_0, 3), (self.bar_packet_checker_0, 0))
        self.connect((self.ieee802_15_4_oqpsk_phy_0, 1), (self.bar_packet_checker_0, 1))
        self.connect((self.ieee802_15_4_oqpsk_phy_0, 0), (self.foo_packet_pad_0, 0))
        self.connect((self.ieee802_15_4_oqpsk_phy_0_0, 2), (self.bar_out_sync_0, 3))
        self.connect((self.ieee802_15_4_oqpsk_phy_0_0, 3), (self.bar_out_sync_0, 4))
        self.connect((self.ieee802_15_4_oqpsk_phy_0_0, 1), (self.bar_out_sync_0, 5))
        self.connect((self.ieee802_15_4_oqpsk_phy_0_0, 3), (self.bar_packet_checker_0, 2))
        self.connect((self.ieee802_15_4_oqpsk_phy_0_0, 1), (self.bar_packet_checker_0, 3))
        self.connect((self.ieee802_15_4_oqpsk_phy_0_0, 1), (self.blocks_char_to_float_0_1_0, 0))
        self.connect((self.ieee802_15_4_oqpsk_phy_0_0, 0), (self.blocks_null_sink_0_0, 0))
        self.connect((self.pluto_source_0, 0), (self.blocks_complex_to_mag_squared_0, 0))
        self.connect((self.pluto_source_0, 0), (self.ieee802_15_4_oqpsk_phy_0_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "full_setup")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_tx_gain(self):
        return self.tx_gain

    def set_tx_gain(self, tx_gain):
        self.tx_gain = tx_gain
        self.pluto_sink_0.set_params(int(self.freq), int(4e6), int(20000000), self.tx_gain, '', True)

    def get_samp(self):
        return self.samp

    def set_samp(self, samp):
        self.samp = samp

    def get_rx_gain(self):
        return self.rx_gain

    def set_rx_gain(self, rx_gain):
        self.rx_gain = rx_gain
        self.pluto_source_0.set_params(int(self.freq), int(4e6), int(20000000), True, True, True, "manual", self.rx_gain, '', True)

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self._freq_callback(self.freq)
        self.pluto_source_0.set_params(int(self.freq), int(4e6), int(20000000), True, True, True, "manual", self.rx_gain, '', True)
        self.pluto_sink_0.set_params(int(self.freq), int(4e6), int(20000000), self.tx_gain, '', True)

    def get_buf(self):
        return self.buf

    def set_buf(self, buf):
        self.buf = buf

    def get_bits(self):
        return self.bits

    def set_bits(self, bits):
        self.bits = bits
        self.blocks_moving_average_xx_0_0.set_length_and_scale((1024+64*self.bits)*10, 1)
        self.blocks_moving_average_xx_0.set_length_and_scale((1024+64*self.bits)*10, 1)
        self.blocks_message_strobe_0_0.set_msg(pmt.intern(''.join(chr(e) for e in range(65, 65+self.bits))))


def main(top_block_cls=full_setup, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
