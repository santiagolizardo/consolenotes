#include "creation_window.h"

#include "ui.h"

extern Dimension screen_size;

#include <form.h>
#include <string.h>

Note* showCreateWindow() {
	Note* note = NULL;
	const Dimension win_size = { 20, 40 };
	FIELD *field[3];
	FORM  *form;
	int ch;
	
	int rows, cols;
      
	field[0] = new_field(1, 13, 1, 14, 0, 0);
	field[1] = new_field(3, 13, 2, 14, 0, 0);
	field[2] = NULL;

//	set_field_fore(field[0], COLOR_PAIR(4)); 
	set_field_back(field[0], COLOR_PAIR(4) | A_UNDERLINE); 
//	set_field_back(field[0], A_UNDERLINE); 	/* Print a line for the option 	*/
	field_opts_off(field[0], O_AUTOSKIP);  	/* Don't go to next field when this */
						/* Field is filled up 		*/
	set_field_back(field[1], A_UNDERLINE); 
	field_opts_off(field[1], O_AUTOSKIP);

	form = new_form(field);

	scale_form(form, &rows, &cols);

	WINDOW *window = newwin(rows +4,cols+4, 10, (screen_size.w>>1)-(win_size.w>>1));
	WINDOW* otherwin = derwin(window, rows, cols, 1, 1);
	set_form_win(form, window);
	set_form_sub(form, otherwin);

	box(window, 0, 0);
	//wborder(window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER );
	chtype a = getbkgd(window);
	wbkgd(window, COLOR_PAIR(4));
	wbkgd(otherwin, COLOR_PAIR(4));

	bool quit = false;
	while(!quit)
	{
		post_form(form);
		mvwprintw(window, 2, 2, "Title: " );
		mvwprintw(window, 3, 2, "Body: " );
		wrefresh(window);

		wtimeout(window, 0);
		ch = getch();
		switch(ch)
		{	case KEY_DOWN:
				form_driver(form, REQ_NEXT_FIELD);
				form_driver(form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(form, REQ_PREV_FIELD);
				form_driver(form, REQ_END_LINE);
				break;
			case KEY_BACKSPACE:
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
		note->title = strdup(field_buffer(field[0], 0));
		note->body = strdup(field_buffer(field[1], 0));
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

