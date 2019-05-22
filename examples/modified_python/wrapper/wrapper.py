# Usage:
# python2 wrapper.py <grc py file> <rx id> <tx id>
# grc py file - any file in modified python file 

import subprocess, shlex
from threading import Timer
from sys import argv
import time

freq = ['245', '915', '430']
#freq = ['430']

rxdb = ['20']
#rxdb = ['0','4','8', '12', '16', '20']

txdb = ['0']
#txdb = ['0','3','6','9','12','15']

dist = ['50_' + str(e) for e in range(1, 4)]
#dist = ['300', '250', '200', '150', '100', '50']

thd = ['']
#thd = ['5.60', '3.75', '3.50','2.50','1.75','0.90']
#thd = ['0.18', '0.4', '0.8', '1.5', '3.75', '5.00']

m = 'psk'

if (m != 'ook'):
  thd = ['']

time_out = 10

i = 0
for r in rxdb:
 for t in txdb:
  for f in freq:
   for d in dist:
      h = ''
      #h = thd[txdb.index(t)]
    #for h in thd:
      i += 1
      print "\n--------- Start of Test " + str(i) + " ----------"
      cmd = 'python ' + argv[1] + ' ' + f + ' ' + h + '_' + d +' ' + r + ' ' + t + ' ' + argv[2] + ' ' + argv[3] + ' ' + m
      
      if m == 'ook':
        cmd += ' ' + h + '*1e-6'
      
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
      
      while diff < time_out:
        print("----- Test too short, repeating test " + str(i) + " -----")
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
        
      print ""

