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
#include <time.h>
#include <stdlib.h>


#define JSON_FILENAME "list.json"

Dimension screen_size;

void resizeHandler(int);

void resizeHandler(int sig) {
	getmaxyx(stdscr, screen_size.h, screen_size.w);
}

int main( int argc, char **argv ) {
	int i = 0,
	    notes_len = 0;
	Note** notes = NULL;
	NoteWindow** noteWindows = NULL;

	srand((unsigned int)time(NULL));
	cJSON* doc = NULL;
	if( file_exists( JSON_FILENAME ) ) {
		doc = file_to_json(JSON_FILENAME);
		notes = json_to_list_node(doc, &notes_len);
		noteWindows = (NoteWindow**)malloc(sizeof(NoteWindow*) * notes_len);
		cJSON_Delete(doc);
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

	int selectedNoteIndex = 0;
	Note* selectedNote = NULL;

	for(i = 0; i < notes_len; i++) {
		noteWindows[ i ] = create_note_window(notes[ i ], screen_size);
	}

	while(!quit)
	{
		werase(stdscr);
		mvwvline(stdscr, 0, 20, ACS_VLINE, LINES);
		mvwvline(stdscr, 0, 40, ACS_VLINE, LINES);
		mvwvline(stdscr, 0, 60, ACS_VLINE, LINES);
		wnoutrefresh(stdscr);

		for(i = 0; i < notes_len; i++) {
			if(notes[i])
				note_window_display(noteWindows[ i ], i == selectedNoteIndex);
		}

		doupdate();
		timeout(-1);
		if(notes_len) {
			selectedNote = notes[ selectedNoteIndex ];
			noteWindow = noteWindows[ selectedNoteIndex ];
		}
		int ch = getch();
		if( ch == KEY_LEFT ) {
			if(noteWindow->position.x > 0)
				noteWindow->position.x -= 1;
		}
		if( ch == KEY_RIGHT ) {
			if(noteWindow->position.x + window_w < screen_size.w)
				noteWindow->position.x += 1;
		}
		if( ch == 'c' ) {
			Note* note = showCreateWindow();
			if(note) {
				notes_len++;
				notes = (Note**)realloc(notes, sizeof(Note*)*notes_len);
				noteWindows = (NoteWindow**)realloc(noteWindows, sizeof(NoteWindow*)*notes_len);
				NoteWindow* noteWindow = create_note_window(note, screen_size);
				notes[notes_len-1]=note;
				noteWindows[notes_len-1]=noteWindow;
				noteWindow->note = note;
			}
		}
		if( ch == 'q' ) quit = true;
		if( ch == '?' ) showHelpWindow();
		if( ch == 'p' ) {
			print_note(selectedNote);
		}
		if( ch == KEY_DC ) {
			notes[ selectedNoteIndex ] = NULL;
		}
		if( ch == '\t' ) {
			selectedNoteIndex++;
			if(selectedNoteIndex == notes_len)
				selectedNoteIndex = 0;
		}
	}

	endwin();

	doc = cJSON_CreateArray();
	for( i = 0; i < notes_len; i++ ) {
		if(notes[ i ]) {
			Note* note = notes[ i ];
			cJSON* json_note = note_to_json(note);
			cJSON_AddItemToArray(doc, json_note);
		}
	}

	char* rendered = cJSON_Print(doc);
	cJSON_Delete(doc);
	write_file_content(JSON_FILENAME, rendered);

	return EXIT_SUCCESS;
}

