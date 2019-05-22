# Usage:
# python2 snr_wrapper.py <rx id> <tx id>

import subprocess, shlex
from threading import Timer
from sys import argv
import time

freq = ['245', '915', '430']
freq = ['245']

rxdb = ['20']
#rxdb = ['0','4','8', '12', '16', '20']

txdb = ['0']
txdb = ['0', '3', '6', '9', '15']

dist = ['_300_' + str(e) for e in range(1, 4)]
#dist = ['300', '250', '200', '150', '100', '50']
#thd = ['0.18', '0.4', '0.8', '1.5', '3.75', '5.00']
mod = 'ook 1'
time_out = 30
time_out_1 = 4.0

i = 0

    
for r in rxdb:
 for t in txdb:
  for f in freq:
   for d in dist:
    i += 1
    """
    print "\n--------- Start of SNR Test " + str(i) + " ----------"
    cmd = 'python ./grc_py/snr_monitor.py ' + f + ' ' + d +' ' + r + ' ' + t + ' ' + argv[1] + ' ' + argv[2] + ' ' + mod
    proc = subprocess.Popen(shlex.split(cmd))
    start_time = time.time()
    timer = Timer(time_out, proc.kill)
    try:
      timer.start()
      proc.communicate()
    finally:
      timer.cancel()
    diff = (time.time() - start_time)
    print("--- End After %s seconds ---" % diff)
    """
    print("--- Getting PSPN " + str(i) + " ---")
    cmd = 'python ./grc_py/pure_sig.py ' + f + ' ' + d +' ' + r + ' ' + t + ' ' + argv[1] + ' ' + argv[2] + ' ' + mod
    proc = subprocess.Popen(shlex.split(cmd))
    start_time = time.time()
    timer = Timer(time_out_1, proc.kill)
    try:
      timer.start()
      proc.communicate()
    finally:
      timer.cancel()
    
    cmd = 'python ./grc_py/pure_noise.py ' + f + ' ' + d +' ' + r + ' ' + t + ' ' + argv[1] + ' ' + argv[2]
    proc = subprocess.Popen(shlex.split(cmd))
    start_time = time.time()
    timer = Timer(time_out_1, proc.kill)
    try:
      timer.start()
      proc.communicate()
    finally:
      timer.cancel()
    
    diff = (time.time() - start_time)
    print("--- End After %s seconds ---" % diff)
    #"""  
    print ""


for r in rxdb:
 for t in txdb:
  for f in freq:
   for d in dist:
     subprocess.call(['python', './proc_py/snr_reader.py', './bin_files/' + f + '_' + r + 'rx_' + t + 'tx_' + d])

