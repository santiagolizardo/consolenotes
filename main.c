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
#include "note_link.h"
#include "creation_window.h"
#include "help_window.h"
#include "colors.h"
#include "input.h"

#include "filesystem.h"

#include "yesno_dialog.h"

#include "agile.h"

#include "json_note.h"
#include "vendor/cJSON/cJSON.h"

#define JSON_FILENAME "list.json"

const bool print_formatted = true;

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

void archive_current_note() {
}

void delete_current_note() {
}

int main( int argc, char **argv ) {
	int i = 0;
	NoteLink* notes = NULL;

	struct arguments arguments;
	arguments.list = 0;

	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	srand((unsigned int)time(NULL));
	cJSON* doc = NULL;

	initscr();

	if(has_colors() == FALSE) {
		fprintf(stderr, "Your terminal does not support colors\n");
		endwin();
		return EXIT_FAILURE;
	}
	start_color();
	init_color_schemes();

	initialize_input();

	signal(SIGWINCH, resizeHandler);

	getmaxyx(stdscr, screen_size.h, screen_size.w);
	wbkgd(stdscr, COLOR_PAIR(8));

	bool quit = false;

	refresh();

	NoteLink* selected_link = NULL;

	if( file_exists( JSON_FILENAME ) ) {
		doc = file_to_json(JSON_FILENAME);
		notes = json_to_list_node(doc);
		cJSON_Delete(doc);
	}

	if(arguments.list) {
		endwin();
		print_all_note_links(notes);
		exit(0);
	}

	while(!quit)
	{
		selected_link = NULL;

		werase(stdscr);
		
		draw_lanes();

		NoteLink* link = notes;
		NoteLink* last = link;
		Note* note = link->note;
		while(link) {
			last = link;
			Note* note = link->note;
			if(note->focused)
				selected_link = link;
			if(!note->archived) {
				create_note_window(note);
				note_window_display(note);
			}
			link = link->next;
		}

		if(!selected_link && notes) {
			selected_link = notes;
			selected_link->note->focused = true;
		}

		doupdate();
		timeout(-1);
		int ch = getch();
		if( ch == '+' && selected_link->next ) {
			if(selected_link->prev)
				selected_link->prev->next = selected_link->next;
			selected_link->next->prev = selected_link->prev;
			if(selected_link == notes)
				notes = selected_link->next;
			selected_link->prev = last;
			last->next = selected_link;
			selected_link->next = NULL;
		}
		if( ch == '-' && selected_link->prev ) {
			selected_link->prev->next = selected_link->next;
			if(selected_link->next)
				selected_link->next->prev = selected_link->prev;
			selected_link->prev = NULL;
			selected_link->next = notes;
			notes = selected_link;
		}
		if(selected_link)
			note = selected_link->note;
		if( ch == KEY_UP && note->window.position.y > 0) {
			note->window.position.y -= 1; 
		}
		if( ch == KEY_DOWN && note->window.position.y + window_size.h < screen_size.h) {
			note->window.position.y += 1; 
		}
		if( ch == KEY_LEFT && note->window.position.x > 0) {
			note->window.position.x -= 1; 
		}
		if( ch == KEY_RIGHT && note->window.position.x + window_size.w < screen_size.w) {
			note->window.position.x += 1;
		}
		if(ch == ' ') {
			note->toggled = !note->toggled;
		}
		if( ch == 'c' ) {
			Note* new_note = showCreateWindow();
			if(new_note) {
				create_note_window(new_note);
				randomize_position(new_note);
				
				NoteLink* new_link = new_note_link();
				new_link->note = new_note;
				new_link->prev = last;
				last->next = new_link;
				selected_link = new_link;
			}
		}
		if(ch == 'a' && show_yesno_dialog("Do you really want archive this note?")) {
			archive_current_note();
			if(selected_link)
				selected_link->note->archived = true;
		}
		if( ch == 'q' || ch == 27 ) quit = true;
		if( ch == '?' ) showHelpWindow();
		if( ch == KEY_DC && show_yesno_dialog("Do you really want to delete this note?")) {
			if(selected_link->prev) {
				selected_link->prev->next = selected_link->next;
			}
			if(selected_link->next) {
				selected_link->next->prev = selected_link->prev;
			}
		}
		if( ch == '\t' ) {
			selected_link->note->focused = false;
			if(selected_link->next) {
				selected_link = selected_link->next;
			}
			else
			{
				selected_link = notes;
			}
			selected_link->note->focused = true;
		}
	}

	endwin();

	cJSON* updated_doc = link_list_to_json(notes);
	char* printable_doc = print_formatted ? cJSON_Print(updated_doc) : cJSON_PrintUnformatted(updated_doc);
	cJSON_Delete(updated_doc);

	write_file_content(JSON_FILENAME, printable_doc);
	free(printable_doc);

	return EXIT_SUCCESS;
}

