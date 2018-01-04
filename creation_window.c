#include "creation_window.h"

#include <form.h>
#include <string.h>

#include "ui.h"
#include "string_utils.h"

extern Dimension screen_size;

Note* showCreateWindow(void) {
	Note* note = NULL;
	Dimension win_size;
	FIELD *field[3];
	FORM  *form;
	int ch;
	int rows, cols;
      
	field[0] = new_field(1, 37, 1, 14, 0, 0);
	field[1] = new_field(7, 37, 2, 14, 0, 0);
	field[2] = NULL;

	set_field_back(field[0], COLOR_PAIR(4) | A_UNDERLINE); 
	field_opts_off(field[0], O_AUTOSKIP);
	set_field_back(field[1], A_UNDERLINE); 
	field_opts_off(field[1], O_AUTOSKIP);

	form = new_form(field);

	scale_form(form, &rows, &cols);
    win_size.w = cols + 4;
    win_size.h = rows + 4;

	WINDOW *window = newwin(win_size.h, win_size.w, 10, (screen_size.w >> 1) - (win_size.w >> 1));
	WINDOW* form_win = derwin(window, rows, cols, 1, 1);
	set_form_win(form, window);
	set_form_sub(form, form_win);

	box(window, 0, 0);
	chtype a = getbkgd(window);
	wbkgd(window, COLOR_PAIR(4));
	wbkgd(form_win, COLOR_PAIR(4));

	bool quit = false;
	while(!quit)
	{
		post_form(form);
		mvwprintw(window, 2, 2, "Title: " );
		mvwprintw(window, 3, 2, "Body: " );
		wrefresh(window);

		wtimeout(window, 0);
		ch = wgetch(window);
		switch(ch)
		{
            case '\t':
            case KEY_DOWN:
				form_driver(form, REQ_NEXT_FIELD);
				form_driver(form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(form, REQ_PREV_FIELD);
				form_driver(form, REQ_END_LINE);
				break;
			case KEY_BACKSPACE:
			case 127:
				form_driver(form, REQ_DEL_PREV);
				break;
			case '\n':
				quit = true;
				break;
			case 27:
				quit = true;
				break;
			default:
				form_driver(form, ch);
				break;
		}
	}

	if(ch == '\n') {
		form_driver(form, REQ_VALIDATION);
		note = new_note(true);
		char* field_value = NULL;
		field_value = field_buffer(field[0], 0);
		note->title = string_is_empty(field_buffer(field[0], 0)) ? NULL : trim_string(field_value);
		field_value = field_buffer(field[1], 0);
		note->body = string_is_empty(field_buffer(field[1], 0)) ? NULL : trim_string(field_value);
	}

	unpost_form(form);
	free_form(form);
	free_field(field[0]);
	free_field(field[1]); 

	wbkgd(window, a);
	wclear(window);
	
	delwin(window);
	
	return note;
}

