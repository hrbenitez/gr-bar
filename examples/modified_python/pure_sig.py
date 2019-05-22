#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: IEEE 802.15.4 Transceiver using OQPSK PHY
# Generated: Thu Mar 14 20:22:33 2019
##################################################

# Usage: 
# python2 pure_sig.py <frequency> <distance> <rx gain> <tx attenuation> <rx id> <tx id> <modulation> <threshold>
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
d_dir = './bin_files/' + str(argv[1]) + '_' + str(d_rxdb) + 'rx_' + str(d_txdb) + 'tx_' + str(d_dist)

sp_call(['mkdir', d_dir])

d_rxid = argv[5]
d_txid = argv[6]

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
from ieee802_15_4_oqpsk_phy import ieee802_15_4_oqpsk_phy  # grc-generated hier_block
from ieee802_15_4_ook_phy import ieee802_15_4_ook_phy  # grc-generated hier_block
from ieee802_15_4_cpfsk_phy import ieee802_15_4_cpfsk_phy  # grc-generated hier_block
from optparse import OptionParser
import foo
import ieee802_15_4
import pmt
import sip
from gnuradio import qtgui


class pure_sig(gr.top_block, Qt.QWidget):

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

        self.settings = Qt.QSettings("GNU Radio", "pure_sig")

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
        self.samp_rate = samp_rate = 4e6

        ##################################################
        # Blocks
        ##################################################
        self._tx_gain_range = Range(-20, 64, 0.01, tx_gain, 200)
        self._tx_gain_win = RangeWidget(self._tx_gain_range, self.set_tx_gain, "tx_gain", "counter_slider", float)
        self.top_layout.addWidget(self._tx_gain_win)
        self._rx_gain_range = Range(0, 72, 0.01, rx_gain, 200)
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
        self.qtgui_time_sink_x_0_1_0 = qtgui.time_sink_f(
        	buf*16, #size
        	samp_rate, #samp_rate
        	"HSS OQPSK", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0_1_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_1_0.set_y_axis(0, 2e-4)

        self.qtgui_time_sink_x_0_1_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_1_0.enable_tags(-1, False)
        self.qtgui_time_sink_x_0_1_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_1_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0_1_0.enable_grid(False)
        self.qtgui_time_sink_x_0_1_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_1_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0_1_0.enable_stem_plot(False)

        if not True:
          self.qtgui_time_sink_x_0_1_0.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]

        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0_1_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0_1_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_1_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_1_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_1_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_1_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_1_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_1_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_1_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_1_0_win)
        self.pluto_source_0 = iio.pluto_source(d_rxid, int(freq), int(4e6), int(20000000), buf*16, True, True, True, "manual", rx_gain, '', True)
        self.pluto_sink_0 = iio.pluto_sink(d_txid, int(freq), int(4e6), int(20000000), buf, True, tx_gain, '', True)
        self.ieee802_15_4_rime_stack_0 = ieee802_15_4.rime_stack(([129]), ([131]), ([132]), ([23,42]))
        
        self.ieee802_15_4_phy_0 = ieee802_15_4_oqpsk_phy()
        if argv[7] == 'ook':
          self.ieee802_15_4_phy_0 = ieee802_15_4_ook_phy(threshold=float(argv[8]))
        if argv[7] == 'fsk':
          self.ieee802_15_4_phy_0 = ieee802_15_4_cpfsk_phy()
        
        self.ieee802_15_4_mac_0 = ieee802_15_4.mac(False,0x8841,0,0x1aaa,0xffff,0x3344)
        self.foo_wireshark_connector_0 = foo.wireshark_connector(195, False)
        self.blocks_socket_pdu_0_0 = blocks.socket_pdu("UDP_SERVER", '', '52001', 10000, False)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_char*1)
        self.blocks_message_strobe_0_0 = blocks.message_strobe(pmt.intern('a' * bits), 10)
        self.blocks_file_sink_1_0_0 = blocks.file_sink(gr.sizeof_float*1, '/home/hrbenitez/Desktop/just_sig.bin', False)
        self.blocks_file_sink_1_0_0.set_unbuffered(False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, '/home/hrbenitez/Desktop/2_pluto.pcap', False)
        self.blocks_file_sink_0.set_unbuffered(True)
        self.blocks_complex_to_mag_squared_0 = blocks.complex_to_mag_squared(1)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_message_strobe_0_0, 'strobe'), (self.ieee802_15_4_rime_stack_0, 'bcin'))
        self.msg_connect((self.blocks_socket_pdu_0_0, 'pdus'), (self.ieee802_15_4_rime_stack_0, 'bcin'))
        self.msg_connect((self.ieee802_15_4_mac_0, 'pdu out'), (self.ieee802_15_4_phy_0, 'txin'))
        self.msg_connect((self.ieee802_15_4_mac_0, 'app out'), (self.ieee802_15_4_rime_stack_0, 'fromMAC'))
        self.msg_connect((self.ieee802_15_4_phy_0, 'rxout'), (self.foo_wireshark_connector_0, 'in'))
        self.msg_connect((self.ieee802_15_4_phy_0, 'rxout'), (self.ieee802_15_4_mac_0, 'pdu in'))
        self.msg_connect((self.ieee802_15_4_rime_stack_0, 'bcout'), (self.blocks_socket_pdu_0_0, 'pdus'))
        self.msg_connect((self.ieee802_15_4_rime_stack_0, 'toMAC'), (self.ieee802_15_4_mac_0, 'app in'))
        self.connect((self.blocks_complex_to_mag_squared_0, 0), (self.blocks_file_sink_1_0_0, 0))
        self.connect((self.blocks_complex_to_mag_squared_0, 0), (self.qtgui_time_sink_x_0_1_0, 0))
        self.connect((self.foo_wireshark_connector_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.ieee802_15_4_phy_0, 2), (self.blocks_null_sink_0, 1))
        self.connect((self.ieee802_15_4_phy_0, 3), (self.blocks_null_sink_0, 2))
        self.connect((self.ieee802_15_4_phy_0, 1), (self.blocks_null_sink_0, 0))
        self.connect((self.ieee802_15_4_phy_0, 0), (self.pluto_sink_0, 0))
        self.connect((self.pluto_source_0, 0), (self.blocks_complex_to_mag_squared_0, 0))
        self.connect((self.pluto_source_0, 0), (self.ieee802_15_4_phy_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "pure_sig")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_tx_gain(self):
        return self.tx_gain

    def set_tx_gain(self, tx_gain):
        self.tx_gain = tx_gain
        self.pluto_sink_0.set_params(int(self.freq), int(self.samp_rate), int(20000000), self.tx_gain, '', True)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_time_sink_x_0_1_0.set_samp_rate(self.samp_rate)
        self.pluto_source_0.set_params(int(self.freq), int(self.samp_rate), int(20000000), True, True, True, "manual", self.rx_gain, '', True)
        self.pluto_sink_0.set_params(int(self.freq), int(self.samp_rate), int(20000000), self.tx_gain, '', True)

    def get_rx_gain(self):
        return self.rx_gain

    def set_rx_gain(self, rx_gain):
        self.rx_gain = rx_gain
        self.pluto_source_0.set_params(int(self.freq), int(self.samp_rate), int(20000000), True, True, True, "manual", self.rx_gain, '', True)

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self._freq_callback(self.freq)
        self.pluto_source_0.set_params(int(self.freq), int(self.samp_rate), int(20000000), True, True, True, "manual", self.rx_gain, '', True)
        self.pluto_sink_0.set_params(int(self.freq), int(self.samp_rate), int(20000000), self.tx_gain, '', True)

    def get_buf(self):
        return self.buf

    def set_buf(self, buf):
        self.buf = buf

    def get_bits(self):
        return self.bits

    def set_bits(self, bits):
        self.bits = bits
        self.blocks_message_strobe_0_0.set_msg(pmt.intern('a' * self.bits))


def main(top_block_cls=pure_sig, options=None):
    if gr.enable_realtime_scheduling() != gr.RT_OK:
        print "Error: failed to enable real-time scheduling."

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
