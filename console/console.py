import serial
import threading
import curses


def get_input(screen):
    while True:
        event = screen.getch()
        if event == ord("q"):
            curses.echo()
            curses.curs_set(1)
            screen.keypad(0)
            curses.endwin()
            # print("quit")
            break


def read_from_port(ser, screen):
    while True:
        data = ser.readline()
        screen.addstr(10, 10, "asdfa")


def start():
    screen = curses.initscr()
    curses.noecho()
    curses.curs_set(0)
    screen.keypad(1)
    screen.clear()
    screen.addstr("WELCOME TO //ROBINSTR//\n\n")
    screen.addstr("Press 'q' to quit.\n\n")
    thread1 = threading.Thread(target=get_input, args=(screen,))
    thread1.start()
    # port = '/dev/tty.usbmodem1411'
    # baud = 9600
    # serial_port = serial.Serial(port, baud)
    # thread2 = threading.Thread(target=read_from_port,
    #                            args=(serial_port, screen,))
    # thread2.start()


start()

