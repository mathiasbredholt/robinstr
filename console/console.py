import curses
import art
import pid
import time
import loadcell
import gfx
import threading
import arduino
import demo

MENU_ITEMS = 4

KP = 0.1
KI = 0.01
KD = 0.005

# ---------------
# Music interface
# ---------------


def play_demo(main_d):
    i = 0
    while i < len(demo.notes) and main_d["running"]:
        # arduino.play_note(main_d["arduino"], demo.notes[i], 127)
        time.sleep(demo.intervals[i])
        i += 1


def play_note(main_d, note, vel):
    diff = 127
    index = 0

    # Determine which string to tune and strike
    for i, target in enumerate(main_d["pid"]):
        if abs(note - target) < diff:
            diff = abs(note - target)
            index = i

    main_d["pid"][index]["target"] = note

    # strike hammer
    arduino.strike(index, vel)


# ---------------
# Peripherals
# ---------------

def connect_to_arduino(main_d):
    main_d["arduino"] = arduino.init()


def connect_to_loadcell(main_d):
    loadcell.init_phidget()


# ----------------
# Controller setup
# ----------------


def init_pid_controllers(main_d):
    main_d["pid"] = [{}, {}, {}, {}]

    for pid_n in main_d["pid"]:
        pid.init(pid_n, KP, KI, KD)


def run_controllers(main_d):
    while main_d["running"]:
        for i in range(0, 4):
            main_d["pid"][i]["current"] = loadcell.get_value(i)

            pid.update(main_d["pid"][i])

            arduino.set_pwm(main_d["arduino"], i, main_d["pid"][i]["output"])

        gfx.log(main_d["pid"][0]["current"])

        time.sleep(pid.DELTA_TIME)


# ---------------
# User interface
# ---------------


def draw_menu(main_d):
    focus = main_d["focus"]

    gfx.draw_art(art.title, 0, 0, 1)
    gfx.draw_art(art.connect, 0, 6, 2 if focus == 0 else 1)
    gfx.draw_art(art.demo, 0, 10, 2 if focus == 1 else 1)
    gfx.draw_art(art.test, 0, 14, 2 if focus == 2 else 1)
    gfx.draw_art(art.quit, 0, 18, 2 if focus == 3 else 1)
    gfx.draw_art(art.note, 48, 10, 1)


# -----------------
# Keyboard handling
# -----------------

def handle_key_event(main_d, key):
    focus = main_d["focus"]
    if key == ord("q"):
        running = False

        main_d["running"] = running

    elif key == curses.KEY_UP:
        focus = focus - 1 if focus > 0 else MENU_ITEMS - 1

        main_d["focus"] = focus
        draw_menu(main_d)

    elif key == curses.KEY_DOWN:
        focus = focus + 1 if focus < MENU_ITEMS - 1 else 0

        main_d["focus"] = focus
        draw_menu(main_d)

    elif key == 10:
        # Enter key hit, trigger events

        focus = main_d["focus"]

        if focus == 0:  # Connect
            connect_triggered(main_d)

        elif focus == 1:  # Play demo
            play_demo_triggered(main_d)

        elif focus == 2:  # Test
            test_triggered(main_d)

        elif focus == 3:  # Quit
            quit_triggered(main_d)


# -------------------
# Menu event handlers
# -------------------

def connect_triggered(main_d):
    connect_to_arduino(main_d)
    connect_to_loadcell(main_d)

    threading.Thread(
        target=run_controllers, args=(main_d,)).start()


def play_demo_triggered(main_d):
    demo = threading.Thread(target=play_demo, args=(main_d,)).start()


def test_triggered(main_d):
    pass


def quit_triggered(main_d):
    main_d["running"] = False


# -------------------
# Main application
# -------------------

def main(stdscr):
    gfx.set_screen(stdscr)
    gfx.init_colors()

    gfx.log("Welcome to ROBINSTR.")
    gfx.log("Press 'q' to quit.")

    main_d = {
        "running": True,
        "focus": 0
    }

    draw_menu(main_d)

    init_pid_controllers(main_d)

    while main_d["running"]:
        focus = main_d["focus"]
        running = main_d["running"]

        key = stdscr.getch()
        handle_key_event(main_d, key)

curses.wrapper(main)
