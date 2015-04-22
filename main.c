#include <stdlib.h>

#include <argp.h>

#include <ncurses.h>
#include <unistd.h>

#include <panel.h>

#include <termios.h>
#include <signal.h>

#include <time.h>
#include <stdlib.h>

#include "ui.h"
#include "note.h"
#include "note_ui.h"
#include "creation_window.h"
#include "help_window.h"
#include "colors.h"
#include "input.h"
#include "note_list.h"

#include "filesystem.h"

#include "yesno_dialog.h"

#include "agile.h"

#include "json_note.h"
#include "vendor/cJSON/cJSON.h"

#define JSON_FILENAME "list.json"

const char *argp_program_version = "ConsoleNotes 1.0";
const char *argp_program_bug_address = "http://github.com/santiagolizardo/consolenotes/";

static char doc[] = "ConsoleNotes is a simple program to manage and display sticky notes on the terminal";

static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"list",    'l', 0,      0,  "List all the notes" },
  {"print",   'p', 0,      OPTION_ALIAS },
  { 0 }
};


struct arguments
{
  int list, verbose;
};

Dimension screen_size;

void resizeHandler(int);

void resizeHandler(int sig) {
	getmaxyx(stdscr, screen_size.h, screen_size.w);
}

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'p': case 'l':
      arguments->list = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}
static struct argp argp = { options, parse_opt, NULL, doc };

int main( int argc, char **argv ) {
	int i = 0,
	    notes_len = 0;
	NoteWindow** noteWindows = NULL;

	  struct arguments arguments;
	  arguments.list = 0;

	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	srand((unsigned int)time(NULL));
	cJSON* doc = NULL;

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
	if( file_exists( JSON_FILENAME ) ) {
		doc = file_to_json(JSON_FILENAME);
		noteWindows = json_to_list_node(doc, &notes_len);
		cJSON_Delete(doc);
	}

	if(arguments.list) {
		endwin();
		for(i = 0; i < notes_len; i++) {
			print_note(noteWindows[ i ]->note);
		}
		exit(0);
	}

	while(!quit)
	{
		werase(stdscr);
		
		draw_lanes();

		for(i = 0; i < notes_len; i++) {
			if(noteWindows[ i ]) {
				note_window_display(noteWindows[ i ], i == selectedNoteIndex);
			}
		}

		doupdate();
		timeout(-1);
		if(notes_len) {
			noteWindow = noteWindows[ selectedNoteIndex ];
		}
		int ch = getch();
		if( ch == KEY_UP && noteWindow->position.y > 0) {
			noteWindow->position.y -= 1; 
		}
		if( ch == KEY_DOWN && noteWindow->position.y + window_size.h < screen_size.h) {
			noteWindow->position.y += 1; 
		}
		if( ch == KEY_LEFT && noteWindow->position.x > 0) {
			noteWindow->position.x -= 1; 
		}
		if( ch == KEY_RIGHT && noteWindow->position.x + window_size.w < screen_size.w) {
			noteWindow->position.x += 1;
		}
		if( ch == 'c' ) {
			Note* note = showCreateWindow();
			if(note) {
				notes_len++;
				NoteWindow* noteWindow = create_note_window(note, screen_size);
				noteWindow->note = note;
				
				noteWindows = (NoteWindow**)realloc(noteWindows, sizeof(NoteWindow*)*notes_len);
				noteWindows[ notes_len - 1 ] = noteWindow;
				selectedNoteIndex = notes_len - 1;
			}
		}
		if( ch == 'q' || ch == 27 ) quit = true;
		if( ch == '?' ) showHelpWindow();
		if( ch == 'p' ) {
			print_note(selectedNote);
		}
		if( ch == KEY_DC && show_yesno_dialog("Do you really want to delete this note?")) {
			noteWindows[ selectedNoteIndex ] = NULL;
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
		if(noteWindows[ i ]) {
			NoteWindow* noteWindow = noteWindows[ i ];
			cJSON* json_note = note_to_json(noteWindow->note, noteWindow->position);
			cJSON_AddItemToArray(doc, json_note);
		}
	}

	char* rendered = cJSON_Print(doc);
	cJSON_Delete(doc);
	write_file_content(JSON_FILENAME, rendered);

	return EXIT_SUCCESS;
}

