#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial
import os
import sys
import numpy as np
import time

port             = 'COM6'
baud             = 115200

if __name__ == "__main__":
    ser = serial.Serial(port, baud)
    ser.parity = serial.PARITY_NONE
    ser.stopbits = 1
    ser.bytesize = 8
    if ser.isOpen():
        print("INFO: Open serialport success.")
    else:
        print("INFO: Open serialport failed.")
        os.system("pause")
        sys.exit()

    try:
        while True:
            cnt = ser.inWaiting()
            if cnt > 0:
                data = ser.read(cnt)
                print(data)

    except KeyboardInterrupt:
        if ser != None:
            ser.close()

