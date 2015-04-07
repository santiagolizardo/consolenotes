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

#include "vendor/cJSON/cJSON.h"

Dimension screen_size;

void resizeHandler(int);

void resizeHandler(int sig) {
	getmaxyx(stdscr, screen_size.h, screen_size.w);
}

int main( int argc, char **argv ) {
	NoteListNode *root = NULL, *last = NULL;

	char* json_string = read_file_content("list.json");
	cJSON* doc = cJSON_Parse(json_string);
	for( int i = 0; i < cJSON_GetArraySize(doc); i++ ) {
		cJSON* child = cJSON_GetArrayItem(doc, i);

		Note* n = (Note*)malloc(sizeof(Note));
		n->title = cJSON_GetObjectItem(child, "title")->valuestring;
		n->body = cJSON_GetObjectItem(child, "description")->valuestring;

		if(root == NULL) {
			root = last = add_node(NULL, n);
		}
		else {
			last = add_node(last, n);
		}
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

	NoteListNode* list = root;
	while(list) {
		noteWindow = create_note_window(list->note);
		note_window_display(noteWindow);

		print_note(list->note);
		list = list->next;
	}

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

