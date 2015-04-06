#include <stdlib.h>


#include <ncurses.h>
#include <unistd.h>

#include <panel.h>

#include <termios.h>
#include <signal.h>

#include "note.h"
#include "note_ui.h"

void resizeHandler(int);

     void resizeHandler(int sig)
     {
          int nh, nw;
          getmaxyx(stdscr, nh, nw);  /* get the new screen size */
     }

int main( int argc, char **argv ) {

	Note* note;
	note = create_note( "my title", "my note body" );
	print_note( note );
	print_note( NULL );

	int x = 0, y = 1;
	int parent_w, parent_h;
	int direction = +1;
	int score_size = 10;

	initscr();
	if(has_colors() == FALSE) {
		fprintf(stderr, "Your terminal does not support colors\n");
		endwin();
	}
	start_color();

	signal(SIGWINCH, resizeHandler);

	init_color(COLOR_YELLOW, 1000, 1000, 600);

	init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);

	noecho();
	curs_set( FALSE );

	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	getmaxyx(stdscr, parent_h, parent_w);

	WINDOW *field = newwin(parent_h - score_size, parent_w, 0, 0);
	attron(COLOR_PAIR(1));

	getch();

	int seconds = 0;
	char seconds_str[10];

	bool quit = false;

	NoteWindow *noteWindow;
	noteWindow = create_note_window(note);
	note_window_display( noteWindow );

	while(!quit)
	{
		sprintf( seconds_str, "%d", seconds );
		//clear();

		//wclear
//		werase(field);
		wborder(field, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER );
		mvwprintw( field, y, x, "o" );
		//wrefresh( field );
//		wnoutrefresh( field );
//		//wrefresh( score );
//		wnoutrefresh( score );
		doupdate();

		usleep(16666);

		x += direction;

		if( x >parent_w || x < 0 ) {
			direction *= -1;
		}

		int ch = getch();
		if( ch == KEY_LEFT ) direction = -1;
		if( ch == KEY_RIGHT ) direction = 1;
		if( ch == 'q' ) quit = true;
		if( ch == '?' ) quit = true;

		seconds++;
	}

	delwin(field);

	endwin();

	return EXIT_SUCCESS;
}
