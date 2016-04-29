import serial
import gfx
import threading

PORT = '/dev/tty.usbmodem1d111'
BAUDRATE = 9600

STRIKE_TIME = 0.0114  # seconds


def init():
    try:
        serial_port = serial.Serial(PORT, BAUDRATE)
        gfx.log("Connected to " + PORT)
        reset_all(serial_port)
        return serial_port
    except serial.serialutil.SerialException as e:
        gfx.log(e)
    return None


def close(serial_port):
    if serial_port is not None:
        reset_all(serial_port)
        serial_port.close()
        gfx.log("Closing serial port...")


def reset_all(serial_port):
    for i in range(0, 7):
        set_pwm(serial_port, i, 0)


def reconnect(serial_port):
    close(serial_port)
    return init()


def set_pwm(serial_port, index, val):
    if serial_port is not None:
        data = bytearray([index, val])
        serial_port.write(data)
    else:
        gfx.log("Instrument not connected.")


# Strike hammer
# index:    0-3
# vel:      0-127

def strike(serial_port, index, vel):
    power = int(vel / 127 * 255)
    set_pwm(serial_port, 4 + index, power)
    threading.Timer(STRIKE_TIME, stop_hammer,
                    (serial_port, index,)).start()


def stop_hammer(serial_port, index):
    set_pwm(serial_port, 4 + index, 0)
