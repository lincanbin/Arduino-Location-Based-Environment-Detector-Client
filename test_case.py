#! /usr/bin/env python
# -*- coding=utf-8 -*-
import serial, urllib, urllib2

print 'start\n'
ser = serial.Serial(2)  # open COM3
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
    #print url
    response = urllib2.urlopen(url).read()#.decode("UTF-8").encode(filesystemencoding)
    print response

    ser.flush()

ser.close()
#raw_input()