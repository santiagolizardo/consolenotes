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

#include "filesystem.h"

#include "json_note.h"
#include "persistence.h"
#include "vendor/cJSON/cJSON.h"

#define JSON_FILENAME "list.json"

Dimension screen_size;

void resizeHandler(int);

void resizeHandler(int sig) {
	getmaxyx(stdscr, screen_size.h, screen_size.w);
}

int main( int argc, char **argv ) {
	int i = 0;
	NoteListNode *root = NULL, *last = NULL;
	cJSON* doc = NULL;

	if( file_exists( JSON_FILENAME ) ) {
		doc = file_to_json(JSON_FILENAME);
		json_to_list_node(doc, &root, &last);
	}
	else {
		doc = cJSON_CreateArray();
	}

	initscr();

	check_colors_support();
	init_color_schemes();
	initialize_input();

	signal(SIGWINCH, resizeHandler);

	getmaxyx(stdscr, screen_size.h, screen_size.w);

	bool quit = false;

	refresh();

	NoteWindow* noteWindow = NULL;

	if(root) {
	NoteListNode* list = root;
	while(list) {
		noteWindow = create_note_window(list->note);
		note_window_display(noteWindow);

		print_note(list->note);
		list = list->next;
	}
	}

	int seconds = 0;
	while(!quit)
	{
		werase(stdscr);
		mvwvline(stdscr, 0, 20, ACS_VLINE, LINES);
		mvwvline(stdscr, 0, 40, ACS_VLINE, LINES);
		mvwvline(stdscr, 0, 60, ACS_VLINE, LINES);
		wnoutrefresh(stdscr);

		if(noteWindow)
		note_window_display( noteWindow );

		doupdate();
		timeout(-1);
		int ch = getch();
		if( ch == KEY_LEFT ) {
			noteWindow->position.x -= 5;
		}
		if( ch == KEY_RIGHT ) {
			noteWindow->position.x += 5;
		}
		if( ch == 'c' ) {
			Note* note = showCreateWindow();
			if(note) {
				noteWindow = create_note_window(note);
				noteWindow->note = note;
				cJSON* json_note = note_to_json(note);
				cJSON_AddItemToArray(doc, json_note);
				char* rendered = cJSON_Print(doc);
				write_file_content(JSON_FILENAME, rendered);
			}
		}
		if( ch == 'q' ) quit = true;
		if( ch == '?' ) showHelpWindow();
		mvprintw(1, 0, "%d", seconds++);
	}

	cJSON_Delete(doc);

	endwin();

	return EXIT_SUCCESS;
}

