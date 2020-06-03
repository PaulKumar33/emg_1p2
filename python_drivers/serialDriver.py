import numpy as np
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import serial
import re
import time
import matplotlib.pyplot as plt

portname="COM14"
baudrate = 500000
try:
    ser = serial.Serial(portname, baudrate)

except:
    print("Could not connect")

### START QtApp #####
app = QtGui.QApplication([])
####################

win = pg.GraphicsWindow(title="Signal from serial port") # creates a window
p = win.addPlot(title="Realtime plot")  # creates empty space for the plot in the window
curve = p.plot()                        # create an empty "plot" (a curve to plot)
windowWidth = 2000                       # width of the window displaying the curve - this is the time scale of the plot
Xm = np.linspace(0,0,windowWidth)          # create array of zeros that is the size of the window width
ptr = -windowWidth
xdata = []
time_arry = []
tik = time.time()
while(time.time() - tik <5):
    try:
        if (ser.inWaiting() > 0):  # Check for data not for an open port
            '''b1 = ser.read(ser.inWaiting())  # Read all data available at once
            if len(b1) % 2 != 0:  # Odd length, drop 1 byte
                b1 = b1[:-1]
            data_type = np.dtype(np.uint8)
            data_int = np.fromstring(b1, dtype=data_type)  # Convert bytes to numpy array
            data_int = data_int.byteswap()'''
            #val = ser.read(ser.inWaiting()).decode())
            #xdata.extend(np.fromval[1:-1])
            #print(np.fromstring(ser.read(ser.inWaiting()), dtype=np.uint16).byteswap())
            data_int = np.fromstring(ser.read(ser.inWaiting()), dtype=np.uint16).byteswap()
            time_arry.append(time.time() - tik)
            xdata.extend(data_int)
            Xm = np.append(Xm, data_int)
            ptr += len(data_int)
            Xm[:-len(data_int)] = Xm[len(data_int):]  # Scroll plot
            curve.setData(Xm[(len(Xm) - windowWidth):])
            curve.setPos(ptr, 0)
            QtGui.QApplication.processEvents()

    except Exception as e:
            print(e)

print(len(xdata))
x = np.linspace(0,5,len(xdata[10:]))
plt.plot(time_arry[10:], xdata[10:])
plt.show()


