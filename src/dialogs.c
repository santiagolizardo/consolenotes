#include "dialogs.h"

#include <ncurses.h>
#include <string.h>

#include "colors.h"
#include "ui.h"

extern Dimension screen_size;

bool show_confirmation_dialog(const char *const message) {
  bool answer = false;
  Dimension win_size = {.w = strlen(message) + 4, .h = 5};

  WINDOW *window =
      newwin(win_size.h, win_size.w, (screen_size.h >> 1) - (win_size.h >> 1),
             (screen_size.w >> 1) - (win_size.w >> 1));

  box(window, 0, 0);
  wbkgd(window, COLOR_PAIR(COLOR_PAIR_CONFIRMATION_DIALOG));

  mvwprintw(window, 2, 2, "%s", message);
  wrefresh(window);
  wtimeout(window, 0);
  int ch = getch();
  answer = (ch == 'y' || ch == 'Y' || ch == '\n');

  delwin(window);

  return answer;
}

void show_information_dialog(const char *const message) {
  Dimension win_size = {.w = strlen(message) + 4, .h = 5};

  WINDOW *window =
      newwin(win_size.h, win_size.w, (screen_size.h >> 1) - (win_size.h >> 1),
             (screen_size.w >> 1) - (win_size.w >> 1));

  box(window, 0, 0);
  wbkgd(window, COLOR_PAIR(COLOR_PAIR_INFORMATION_DIALOG));

  mvwprintw(window, 2, 2, "%s", message);
  wrefresh(window);
  wtimeout(window, 0);
  getch();

  delwin(window);
}
