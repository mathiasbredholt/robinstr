import serial
import curses
import art
import pid
import time

MENU_ITEMS = 2


def get_input(stdscr, runtime_settings):
    focus = runtime_settings["focus"]
    running = runtime_settings["running"]

    event = stdscr.getch()
    if event == ord("q"):
        running = False

        runtime_settings["running"] = running
    elif event == curses.KEY_UP:
        focus = focus - 1 if focus > 0 else MENU_ITEMS - 1

        runtime_settings["focus"] = focus
        focus_changed(stdscr, runtime_settings)
    elif event == curses.KEY_DOWN:
        focus = focus + 1 if focus < MENU_ITEMS - 1 else 0

        runtime_settings["focus"] = focus
        focus_changed(stdscr, runtime_settings)
    elif event == curses.KEY_ENTER:
        running = False

        runtime_settings["running"] = running

    # def read_from_port(ser, stdscr):
    #     while True:
    #         data = ser.readline()
    #         stdscr.addstr(10, 10, "asdfa")


def focus_changed(stdscr, runtime_settings):
    draw_menu(stdscr, runtime_settings)


def draw_menu(stdscr, runtime_settings):
    focus = runtime_settings["focus"]

    draw_art(stdscr, art.title, 0, 0, 1)
    draw_art(stdscr, art.demo, 0, 10, 2 if focus == 0 else 1)
    draw_art(stdscr, art.test, 0, 14, 2 if focus == 1 else 1)

    stdscr.refresh()


# COLORS:
# 1: RED, BLACK
# 2: BLACK, RED
# 2: WHITE, BLACK


def init_colors():
    curses.init_pair(1, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_BLACK, curses.COLOR_RED)
    curses.init_pair(3, curses.COLOR_WHITE, curses.COLOR_BLACK)


def draw_art(stdscr, input="", x=0, y=0, color=1):
    for i in range(0, len(input)):
        stdscr.addstr(i + y, x, input[i], curses.color_pair(color))


def log(stdscr, string):
    stdscr.addstr(18, 0, str(string), curses.color_pair(3))
    stdscr.refresh()


# def play_demo():


def main(stdscr):
    pid_dict = {}

    target = 10.0
    current = 0.0

    pid.init(pid_dict, 1, 0.1, 1)

    runtime_settings = {
        "running": True,
        "focus": 0
    }

    init_colors()
    stdscr.clear()

    curses.curs_set(False)
    draw_menu(stdscr, runtime_settings)
    stdscr.refresh()

    while runtime_settings["running"]:
        # get_input(stdscr, runtime_settings)
        pid.update(pid_dict, target, current)
        # log(stdscr, "asdf")
        log(stdscr, pid_dict["output"])
        time.sleep(0.1)


#     # curses.noecho()
#     # stdscr.keypad(1)
#     stdscr.addstr("Press 'q' to quit.\n\n")
#     # thread1 = threading.Thread(target=get_input, args=(stdscr,))
#     # thread1.start()
#     stdscr.refresh()
#     stdscr.getkey()
    port = '/dev/tty.usbmodem1411'
    baud = 31250
    # serial_port = serial.Serial(port, baud)
#     # thread2 = threading.Thread(target=read_from_port,
#     #                            args=(serial_port, stdscr,))
#     # thread2.start()


curses.wrapper(main)
