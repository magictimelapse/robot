import serial
import numpy as np
import struct
import time
class ArduinoControl(object):
    def __init__(self):
        self._ser = serial.Serial('/dev/ttyACM0',115200)
        time.sleep(5)
    def _sendCommand(self,command):
        print "sending: ",command
        self._ser.write(command)
        ans = self._ser.readline()
        print "received: ",ans
        #time.sleep(0.5)

    def _dists(self):
        while True:
            header = str(self._ser.readline())
            #print header[2]
            if header[:3] == "###":
                ndists = int(self._ser.readline())
                print ndists
                #arr = np.zeros(ndists,np.float32)
                data = self._ser.read(4*ndists)
                arr = np.fromstring(data, dtype='<f4')
                #self._ser.read(2)
#for jj in range(ndists):
                #    data = self._ser.read(4)
                #    vF, = struct.unpack('<f',data)
                #    arr[jj] = vF
                return arr


    def sweep(self, start, stop):
        command = 's'
        self._sendCommand(command)
        startByte = struct.pack('<i',start)[0]
        stopByte  = struct.pack('<i',stop)[0]
        self._sendCommand(startByte)
        self._sendCommand(stopByte)
        return self._dists()

    def dist(self):
        command = 'd'
        self._sendCommand(command)
        dist = float(self._ser.readline())
        return dist

    def running(self):
        command = 'r'
        self._sendCommand(command)
        running = int(self._ser.readline())
        if running == 1:
            return True
        else:
            return False
        


    def heading(self):
        command = 'h'
        self._sendCommand(command)
        heading = float(self._ser.readline())
        time.sleep(0.5)
        return heading

