import serial
import numpy as np
import struct
class ArduinoControl(object):
    def __init__(self):
        self._ser = serial.Serial('/dev/ttyACM0',115200)

    def dist(self):
        while True:
            header = str(self._ser.readline())
            #print header[2]
            if header[:3] == "###":
                ndists = int(self._ser.readline())
                print ndists
                arr = np.zeros(ndists,np.float32)
                for jj in range(ndists):
                    data = self._ser.read(4)
                    vF, = struct.unpack('<f',data)
                    arr[jj] = vF
                return arr
