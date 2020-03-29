#include "input.h"

#include <ncurses.h>

void initialize_input(void) {
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
}
