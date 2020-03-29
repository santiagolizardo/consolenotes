#include "ui.h"

#include <ncurses.h>

extern Dimension screen_size;

void resize_handler(int sig) {
  (void)sig; // Silent unused warning
  getmaxyx(stdscr, screen_size.h, screen_size.w);
  wnoutrefresh(stdscr);
  doupdate();
}
