import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import random as rand
# import serial

# ser = serial.Serial()
# ser.port = 'COM11'
# ser.baudrate = 115200
# ser.setDTR(False)
# ser.setRTS(False)

# ser.open()

y_val = []

def read_data():
    y_val.append(rand.randrange(75, 85))
    x_val = np.arange(0, len(y_val))
        
    return x_val, y_val

def animate(i):
    x_val, y_val = read_data()

    plt.cla()
    plt.title("Valores de RPM")
    plt.ylabel("RPM")
    plt.grid()
    plt.tight_layout()
    plt.plot(x_val, y_val)

anim = FuncAnimation(plt.gcf(), animate, interval=1000, cache_frame_data=False)

plt.show()

# while True:
#     b = ser.readline()
#     print(b)