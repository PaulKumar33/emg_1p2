import serial
import time
import matplotlib.pyplot as plt
import numpy as np

COM = "COM3"
ser = serial.Serial(COM, 1000000)

ser.flushInput()

measurement_period = 5
print("recording from serial port {0} for {1}".format(COM, measurement_period))

tik = time.time()
decoded_bytes = []
while(time.time() - tik < measurement_period):
	 #ser_bytes = ser.readline()
	try:
		byte= float(ser.readline().decode('utf-8').rstrip())
		decoded_bytes.append(byte)
	except ValueError:
		print("Was not float")

print("Final length after {0}s: {1}".format(measurement_period, len(decoded_bytes)))
x = np.linspace(0,10,len(decoded_bytes))
plt.plot(x, decoded_bytes)

plt.title("Test Signal: Sine 1kHz, baud: 1MHz, Prescaler 128")
plt.show()




