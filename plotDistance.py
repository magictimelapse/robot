#!/usr/bin/env python
import arduinoControl
import numpy as np
import matplotlib.pyplot as plt

if __name__=="__main__":
    plt.ion()
    fig = plt.figure()
    ax = fig.add_subplot(111,projection='polar')
    #l, = plt.plot([],[],'r-')
    ax.set_rmax(100)
    ax.grid(True)
    #plt.xlim(0,180)
    #plt.ylim(0,100)
    #plt.xlabel('deg')
    #plt.title('distance')
    ac = arduinoControl.ArduinoControl()
    pair = True
    while True:
        print "heading"
        heading = ac.heading()
        print heading
        print "sweep"
        if pair:
            ydata= ac.sweep(45,135)
        else:
            ydata=ac.sweep(135,45)
        pair = not pair
        #print "getting data.."
        #ydata = ac.dist()
        print "done"
        xdata = np.arange(0,np.pi/180,np.pi/180/len(ydata),dtype=np.float32)
        #ax.set_data(xdata,ydata)
        plt.cla()
        ax.plot(xdata,ydata,color='r',linewidth=3)
        #plt.draw()
        plt.show()
