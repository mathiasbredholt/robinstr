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


def focus_changed(main_d):
    draw_menu(main_d)


def draw_menu(main_d):
    focus = main_d["focus"]

    gfx.draw_art(art.title, 0, 0, 1)
    gfx.draw_art(art.connect, 0, 6, 2 if focus == 0 else 1)
    gfx.draw_art(art.demo, 0, 10, 2 if focus == 1 else 1)
    gfx.draw_art(art.test, 0, 14, 2 if focus == 2 else 1)
    gfx.draw_art(art.quit, 0, 18, 2 if focus == 3 else 1)
    gfx.draw_art(art.note, 48, 10, 1)


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


def connect_to_loadcell(main_d):
    if loadcell.init_phidget():
        threading.Thread(
            target=loadcell.get_value, args=(main_d,)).start()


def enter_key_triggered(main_d):
    focus = main_d["focus"]

    if focus == 0:  # Connect
        connect_to_loadcell(main_d)
        # main_d["arduino"] = arduino.reconnect()
        threading.Thread(
            target=run_controllers, args=(main_d,)).start()
    elif focus == 1:  # Play demo
        demo = threading.Thread(target=play_demo, args=(main_d,)).start()
    elif focus == 2:  # Test
        pass
    elif focus == 3:  # Quit
        main_d["running"] = False


def run_controllers(main_d):
    while main_d["running"]:
        pid.update(main_d["pid"][0])
        pid.update(main_d["pid"][1])
        pid.update(main_d["pid"][2])
        pid.update(main_d["pid"][3])

        arduino.set_pwm(main_d["arduino"], 0, main_d["pid"][0]["output"])
        arduino.set_pwm(main_d["arduino"], 1, main_d["pid"][1]["output"])
        arduino.set_pwm(main_d["arduino"], 2, main_d["pid"][2]["output"])
        arduino.set_pwm(main_d["arduino"], 3, main_d["pid"][3]["output"])

        time.sleep(pid.DELTA_TIME)


def main(stdscr):
    gfx.set_screen(stdscr)
    gfx.init_colors()

    gfx.log("Welcome to ROBINSTR.")
    gfx.log("Press 'q' to quit.")

    loadcell_values = [0, 0, 0, 0]

    main_d = {
        "running": True,
        "focus": 0,
        "loadcell_values": [0, 0, 0, 0],
        "arduino": arduino.init(),
        "pid": [{}, {}, {}, {}],
    }

    pid.init(main_d["pid"][0], KP, KI, KD)
    pid.init(main_d["pid"][1], KP, KI, KD)
    pid.init(main_d["pid"][2], KP, KI, KD)
    pid.init(main_d["pid"][3], KP, KI, KD)

    draw_menu(main_d)

    while main_d["running"]:
        focus = main_d["focus"]
        running = main_d["running"]

        event = stdscr.getch()
        if event == ord("q"):
            running = False

            main_d["running"] = running
        elif event == curses.KEY_UP:
            focus = focus - 1 if focus > 0 else MENU_ITEMS - 1

            main_d["focus"] = focus
            focus_changed(main_d)
        elif event == curses.KEY_DOWN:
            focus = focus + 1 if focus < MENU_ITEMS - 1 else 0

            main_d["focus"] = focus
            focus_changed(main_d)
        elif event == 10:
            enter_key_triggered(main_d)

curses.wrapper(main)
