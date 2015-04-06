#include <stdlib.h>

#include <ncurses.h>
#include <unistd.h>

#include <panel.h>

#include <termios.h>
#include <signal.h>

#include "ui.h"
#include "note.h"
#include "note_ui.h"
#include "help_window.h"
#include "colors.h"
#include "input.h"
#include "note_list.h"

Dimension screen_size;

void resizeHandler(int);

void resizeHandler(int sig) {
	int nh, nw;
	getmaxyx(stdscr, nh, nw);  /* get the new screen size */
}

int main( int argc, char **argv ) {

	Note* note;
	note = create_note("my title", "my note body");

	Note* note2;
	note2 = create_note("cool title!", "i am just testing");

	Note* note3;
	note3 = create_note("phplinux", "phplinux apapa");

	NoteListNode* list = add_node( NULL, note );
	NoteListNode* list2 = add_node( list, note2 );
	add_node( list2, note3 );

	while(list) {
		print_note(list->note);
		list = list->next;
	}

	initscr();

	check_colors_support();
	init_color_schemes();
	initialize_input();

	signal(SIGWINCH, resizeHandler);

	getmaxyx(stdscr, screen_size.h, screen_size.w);

	bool quit = false;

	refresh();

	NoteWindow *noteWindow;
	noteWindow = create_note_window(note);
	note_window_display( noteWindow );

	int seconds = 0;
	while(!quit)
	{
		werase(stdscr);
		wnoutrefresh(stdscr);
		note_window_display( noteWindow );
		doupdate();
		timeout(-1);
		int ch = getch();
		if( ch == KEY_LEFT ) {
			noteWindow->position.x -= 5;
			mvprintw(0, 0, "left");
		}
		if( ch == KEY_RIGHT ) {
			noteWindow->position.x += 5;
			mvprintw(0, 0, "right");
		}
		if( ch == 'c' ) showCreateWindow();;
		if( ch == 'q' ) quit = true;
		if( ch == '?' ) showHelpWindow();
		mvprintw(1, 0, "%d", seconds++);
	}

	endwin();

	return EXIT_SUCCESS;
}

