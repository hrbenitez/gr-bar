#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Pure Noise
# Generated: Thu Mar 14 20:57:22 2019
##################################################

# Usage: 
# python2 pure_noise.py <frequency> <distance> <rx gain> <tx attenuation> <rx id> <tx id> <modulation>
# distance - not really necessary, for directory/filename purposes

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

from PyQt5 import Qt
from PyQt5 import Qt, QtCore
from PyQt5.QtCore import QObject, pyqtSlot
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import iio
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import sip
import sys
from gnuradio import qtgui


class pure_noise(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Pure Noise")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Pure Noise")
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

        self.settings = Qt.QSettings("GNU Radio", "pure_noise")

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
        self.haven = haven = 0x100000
        
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
        self.qtgui_number_sink_0 = qtgui.number_sink(
            gr.sizeof_float,
            0,
            qtgui.NUM_GRAPH_HORIZ,
            1
        )
        self.qtgui_number_sink_0.set_update_time(0.10)
        self.qtgui_number_sink_0.set_title("")

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        units = ['', '', '', '', '',
                 '', '', '', '', '']
        colors = [("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"),
                  ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black")]
        factor = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        for i in xrange(1):
            self.qtgui_number_sink_0.set_min(i, -1)
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
        self.pluto_sink_0_0 = iio.pluto_sink(d_txid, int(freq), int(4e6), int(20000000), buf, False, tx_gain, '', True)
        self.blocks_nlog10_ff_0 = blocks.nlog10_ff(10, 1, 0)
        self.blocks_moving_average_xx_1 = blocks.moving_average_ff(haven/32, 1, 4000)
        self.blocks_moving_average_xx_0 = blocks.moving_average_ff(haven/32, 1, 4000)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_float*1, '/home/hrbenitez/Desktop/just_sig.bin', False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_float*1, d_dir + '/snr_pure.bin', False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.blocks_divide_xx_0 = blocks.divide_ff(1)
        self.blocks_complex_to_mag_squared_0 = blocks.complex_to_mag_squared(1)
        self.analog_const_source_x_0 = analog.sig_source_c(0, analog.GR_CONST_WAVE, 0, 0, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_const_source_x_0, 0), (self.pluto_sink_0_0, 0))
        self.connect((self.blocks_complex_to_mag_squared_0, 0), (self.blocks_moving_average_xx_1, 0))
        self.connect((self.blocks_divide_xx_0, 0), (self.blocks_nlog10_ff_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.blocks_moving_average_xx_0, 0))
        self.connect((self.blocks_moving_average_xx_0, 0), (self.blocks_divide_xx_0, 0))
        self.connect((self.blocks_moving_average_xx_1, 0), (self.blocks_divide_xx_0, 1))
        self.connect((self.blocks_nlog10_ff_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.blocks_nlog10_ff_0, 0), (self.qtgui_number_sink_0, 0))
        self.connect((self.pluto_source_0, 0), (self.blocks_complex_to_mag_squared_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "pure_noise")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_tx_gain(self):
        return self.tx_gain

    def set_tx_gain(self, tx_gain):
        self.tx_gain = tx_gain
        self.pluto_sink_0_0.set_params(int(self.freq), int(4e6), int(20000000), self.tx_gain, '', True)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_rx_gain(self):
        return self.rx_gain

    def set_rx_gain(self, rx_gain):
        self.rx_gain = rx_gain
        self.pluto_source_0.set_params(int(self.freq), int(4e6), int(20000000), True, True, True, "manual", self.rx_gain, '', True)

    def get_haven(self):
        return self.haven

    def set_haven(self, haven):
        self.haven = haven
        self.blocks_moving_average_xx_1.set_length_and_scale(self.haven/32, 1)
        self.blocks_moving_average_xx_0.set_length_and_scale(self.haven/32, 1)

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self._freq_callback(self.freq)
        self.pluto_source_0.set_params(int(self.freq), int(4e6), int(20000000), True, True, True, "manual", self.rx_gain, '', True)
        self.pluto_sink_0_0.set_params(int(self.freq), int(4e6), int(20000000), self.tx_gain, '', True)

    def get_buf(self):
        return self.buf

    def set_buf(self, buf):
        self.buf = buf


def main(top_block_cls=pure_noise, options=None):

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
