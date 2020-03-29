#include "goto_window.h"

#include <form.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "dialogs.h"
#include "ui.h"

extern Dimension screen_size;

size_t show_goto_window() {
  size_t index = SIZE_MAX;

  FIELD *field[2];
  FORM *form;
  Dimension win_size;
  int rows, cols;
  field[0] = new_field(1, 2,  // h, w
                       1, 10, // y, x
                       /* offscreen */ 0, /* nbuffers */ 0);
  field[1] = NULL;
  set_max_field(field[0], 2);
  set_field_type(field[0], TYPE_INTEGER, 0, 0, 99);
  set_field_fore(field[0], COLOR_PAIR(COLOR_PAIR_FIELD_FORE));
  set_field_back(field[0], COLOR_PAIR(COLOR_PAIR_FIELD));

  form = new_form(field);
  set_current_field(form, field[0]);
  scale_form(form, &rows, &cols);
  win_size.w = cols + 4;
  win_size.h = rows + 4;

  WINDOW *window = newwin(win_size.h, win_size.w, 10,
                          (screen_size.w >> 1) - (win_size.w >> 1));
  WINDOW *form_win = derwin(window, rows, cols, 1, 1);
  set_form_win(form, window);
  set_form_sub(form, form_win);
  bool quit = false;
  while (!quit) {
    post_form(form);
    mvwprintw(window, 2, 2, "Index:");
    wrefresh(window);
    wtimeout(window, 0);
    int ch = wgetch(window);
    switch (ch) {
    case 27:
      quit = true;
      break;
    case '\n':
      form_driver(form, REQ_VALIDATION);
      char *field_value = NULL;
      field_value = strdup(field_buffer(field[0], 0));
      index = atoi(field_value);
      quit = true;
      break;
    default:
      form_driver(form, ch);
      break;
    }
  }

  unpost_form(form);
  free_form(form);
  free_field(field[0]);

  wclear(window);

  delwin(window);

  return index;
}
