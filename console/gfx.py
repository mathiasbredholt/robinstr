import curses

MAX_LINES = 5

log_line = 0


def set_screen(scr):
    global stdscr
    stdscr = scr
    stdscr.clear()
    curses.curs_set(0)


# COLORS:
# 1: RED, BLACK
# 2: BLACK, RED
# 2: WHITE, BLACK

def init_colors():
    curses.init_pair(1, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_RED, 6)
    curses.init_pair(3, curses.COLOR_WHITE, curses.COLOR_BLACK)


def log(string):

    h, w = stdscr.getmaxyx()

    global log_line
    if log_line >= MAX_LINES:
        log_line = 0
        for i in range(0, MAX_LINES):
            stdscr.move(h - MAX_LINES + i, 0)
            stdscr.clrtoeol()

    stdscr.move(h - MAX_LINES + log_line, 0)
    stdscr.clrtoeol()
    stdscr.addstr(h - MAX_LINES + log_line, 0,
                  str(string), curses.color_pair(3))
    log_line += 1
    stdscr.refresh()


def draw_art(input="", x=0, y=0, color=1):
    for i in range(0, len(input)):
        stdscr.addstr(i + y, x, input[i], curses.color_pair(color))
    stdscr.refresh()
