#! /usr/bin/env python
# -*- coding=utf-8 -*-
import time, serial, urllib, urllib2
print 'start\n'
try:
    ser = serial.Serial(2)    # open COM3
    ser.baudrate = 9600
    for x in xrange(1,100):
        print x,
        line = ser.readline().replace('\r\n', '').split(',')
        print line
        data = {
            'device_index': 13726247339,
            'longitude': 113.540718,
            'latitude': 22.256467,
            'temperature': line[1],
            'humidity': line[2],
            'particulate_matter': line[0]
        }
        url = 'http://monitor.ourjnu.com/submit.php?'+urllib.urlencode(data)
        try:
            response = urllib2.urlopen(url).read()
            print response
            time.sleep(3)    #sleep 3 seconds
        except:
            print 'Network error.'
        ser.flush()
    ser.close()
except:
    print 'Could not open port COM3.'
#raw_input()