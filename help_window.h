#pragma once

#include "ui.h"

extern Dimension screen_size;

#include <form.h>

void showHelpWindow() {
	const Dimension win_size = { 20, 40 };
	WINDOW *help_window = newwin(win_size.h, win_size.w, 10, (screen_size.w>>1)-(win_size.w>>1));
	box(help_window, 0, 0);
	wborder(help_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER );
	chtype a = getbkgd(help_window);
	wbkgd(help_window, COLOR_PAIR(3));
	mvwprintw(help_window, 2, 2, "? This help" );
	mvwprintw(help_window, 3, 2, "q Quit application" );
	wnoutrefresh(help_window);
	doupdate();
	//nodelay(help_window, TRUE);
//	refresh();
	wtimeout(help_window, -1);
	wgetch(help_window);
	wbkgd(help_window, a);
	wclear(help_window);
	werase(help_window);
	wnoutrefresh(help_window);
	doupdate();

	delwin(help_window);
}

void showCreateWindow() {
	const Dimension win_size = { 20, 40 };
	FIELD *field[3];
	FORM  *my_form;
	int ch;
	
	int rows, cols;
      
	field[0] = new_field(1, 10, 4, 18, 0, 0);
	field[1] = new_field(1, 10, 6, 18, 0, 0);
	field[2] = NULL;

//	set_field_fore(field[0], COLOR_PAIR(4)); 
	set_field_back(field[0], COLOR_PAIR(4) | A_UNDERLINE); 
//	set_field_back(field[0], A_UNDERLINE); 	/* Print a line for the option 	*/
	field_opts_off(field[0], O_AUTOSKIP);  	/* Don't go to next field when this */
						/* Field is filled up 		*/
	set_field_back(field[1], A_UNDERLINE); 
	field_opts_off(field[1], O_AUTOSKIP);

	my_form = new_form(field);

	scale_form (my_form, &rows, &cols); /* get dimensions of form */

	WINDOW *help_window = newwin(rows +4,cols+4, 10, (screen_size.w>>1)-(win_size.w>>1));
	set_form_win (my_form, help_window);

	WINDOW* otherwin = derwin (help_window, rows, cols, 2, 2);
	set_form_sub (my_form, otherwin);

	box(help_window, 0, 0);
	//wborder(help_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER );
	chtype a = getbkgd(help_window);
	wbkgd(help_window, COLOR_PAIR(4));
	wbkgd(otherwin, COLOR_PAIR(4));

	post_form(my_form);
	mvwprintw(help_window, 2, 2, "Title: " );
	mvwprintw(help_window, 3, 2, "Body: " );

	wnoutrefresh(help_window);
	
//	mvwprintw(help_window,4, 10, "Value 1:");
//	mvwprintw(help_window,6, 10, "Value 2:");
	doupdate();

	/* Loop through to get user requests */
	while((ch = getch()) != KEY_F(1))
	{	switch(ch)
		{	case KEY_DOWN:
				/* Go to next field */
				form_driver(my_form, REQ_NEXT_FIELD);
				/* Go to the end of the present buffer */
				/* Leaves nicely at the last character */
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP:
				/* Go to previous field */
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_BACKSPACE:
				form_driver(my_form, REQ_DEL_PREV);
				break;
			default:
				/* If this is a normal character, it gets */
				/* Printed				  */	
				form_driver(my_form, ch);
				break;
		}
//	wbkgd(help_window, COLOR_PAIR(4));
	mvwprintw(help_window, 2, 2, "Title: " );
	mvwprintw(help_window, 3, 2, "Body: " );

	post_form(my_form);

		wnoutrefresh(help_window);
	doupdate();

	}

	/* Un post form and free the memory */
	unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
	free_field(field[1]); 

	wnoutrefresh(help_window);
	doupdate();
	wtimeout(help_window, -1);
	wgetch(help_window);
	wbkgd(help_window, a);
	wclear(help_window);
	werase(help_window);
	wnoutrefresh(help_window);
	doupdate();

	delwin(help_window);
}

