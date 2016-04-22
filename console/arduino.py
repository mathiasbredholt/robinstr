import serial
import gfx
import threading

PORT = '/dev/tty.usbmodem1411'
BAUDRATE = 31250

STRIKE_TIME = 0.005  # seconds


def init():
    try:
        serial_port = serial.Serial(PORT, BAUDRATE)
        gfx.log("Connected to " + PORT)
        return serial_port
    except serial.serialutil.SerialException as e:
        gfx.log(e)
    return None


def close(serial_port):
    if serial_port is not None:
        serial_port.close()
        gfx.log("Closing serial port...")


def reconnect(serial_port):
    close(serial_port)
    return init()


def set_pwm(serial_port, index, val):
    if serial_port is not None:
        serial_port.write(b"{} {}".format(index, val))
    else:
        gfx.log("Instrument not connected.")


# Strike hammer
# index:    0-3
# vel:      0-127

def strike(serial_port, index, vel):
    set_pwm(serial_port, 4 + index, vel / 127 * 255)
    threading.Timer(STRIKE_TIME, stop_hammer, (serial_port, index,))


def stop_hammer(serial_port, index):
    set_pwm(serial_port, index, 0)
