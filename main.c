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

	NoteListNode* root = add_node( NULL, note );
	add_note( root, note2 );

	while(root) {
		print_note(root->note);
		root = root->next;
	}

	initscr();

	check_colors_support();
	init_color_schemes();
	initialize_input();

	signal(SIGWINCH, resizeHandler);

	getmaxyx(stdscr, screen_size.h, screen_size.w);

	bool quit = false;

	NoteWindow *noteWindow;
	noteWindow = create_note_window(note);
	note_window_display( noteWindow );

	while(!quit)
	{
		note_window_display( noteWindow );
		refresh();
		timeout(-1);
		int ch = getch();
		if( ch == KEY_LEFT ) -1;
		if( ch == KEY_RIGHT ) 1;
		if( ch == 'q' ) quit = true;
		if( ch == '?' ) showHelpWindow();
	}

	endwin();

	return EXIT_SUCCESS;
}

