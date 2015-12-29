#!/usr/bin/env python
import arduinoControl
import numpy as np
import matplotlib.pyplot as plt

if __name__=="__main__":
    plt.ion()
    fig = plt.figure()
    axes = fig.add_subplot(111)
    l, = plt.plot([],[],'r-')
    plt.xlim(0,180)
    plt.ylim(0,100)
    plt.xlabel('deg')
    plt.title('distance')
    ac = arduinoControl.ArduinoControl()
    while True:
        print "getting data.."
        ydata = ac.dist()
        print "done"
        xdata = np.arange(0,180,180/len(ydata),dtype=np.float32)
        l.set_data(xdata,ydata)
        plt.draw()
        #plt.show()
