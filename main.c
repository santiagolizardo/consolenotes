#include <stdlib.h>

#include <argp.h>

#include <ncurses.h>
#include <unistd.h>

#include <panel.h>

#include <termios.h>
#include <signal.h>

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <form.h>
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

#include "json_note.h"
#include "vendor/cJSON/cJSON.h"

#define JSON_FILENAME "list.json"

const bool print_formatted = true;

const char *argp_program_version = "ConsoleNotes 1.0.0";
const char *argp_program_bug_address = "https://github.com/santiagolizardo/consolenotes/";

static char doc[] = "ConsoleNotes is a simple program to manage and display sticky notes on the terminal";

static struct argp_option options[] = {
	{"verbose",  'v', 0,      0,  "Produce verbose output" },
	{"list",    'l', 0,      0,  "List all the notes" },
	{"print",   'p', 0,      OPTION_ALIAS },
	{NULL}
};


struct arguments {
  int list, verbose;
};

Dimension screen_size;

void resizeHandler(int sig) {
	getmaxyx(stdscr, screen_size.h, screen_size.w);
	wnoutrefresh(stdscr);
	doupdate();
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
	struct arguments *arguments = state->input;

	switch (key) {
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

void archive_current_note(NoteLink* selected_link) {
	selected_link->note->archived = true;
}

void delete_current_note(NoteLink* selected_link, NoteLink** note_list_head) {
	if(selected_link == *note_list_head) {
		if(selected_link->next) {
			*note_list_head = selected_link->next;
		} else {
			*note_list_head = NULL;
		}
		selected_link = NULL;
		return;
	}
	if(selected_link->prev) {
		selected_link->prev->next = selected_link->next;
	}
	if(selected_link->next) {
		selected_link->next->prev = selected_link->prev;
	}
	selected_link = NULL;
}

void goto_previous_note(NoteLink** selected_link_ptr, NoteLink* note_list_head) {
	NoteLink* selected_link = *selected_link_ptr;
	selected_link->note->focused = false;
	if(selected_link->prev) {
		selected_link = selected_link->prev;
	} else {
		selected_link = note_list_head;
	}
	selected_link->note->focused = true;
}

void goto_next_note(NoteLink** selected_link_ptr, NoteLink* note_list_head) {
	NoteLink* selected_link = *selected_link_ptr;
	selected_link->note->focused = false;
	if(selected_link->next) {
		selected_link = selected_link->next;
	} else {
		selected_link = note_list_head;
	}
	selected_link->note->focused = true;
}

void goto_note(NoteLink** selected_link_ptr, NoteLink* note_list_head) {
	NoteLink* selected_link = *selected_link_ptr;
	FIELD *field[2];
	FORM *form;
	Dimension win_size;
	int rows, cols;
	field[0] = new_field(
		1, 2, // h, w
		1, 10, // y, x
		/* offscreen */ 0, /* nbuffers */ 0);
	field[1] = NULL;
	set_max_field(field[0], 2);
	set_field_type(field[0], TYPE_INTEGER, 0, 0, 99);
	set_field_fore(field[0], COLOR_PAIR(1));
	set_field_back(field[0], COLOR_PAIR(2));

	form = new_form(field);
	set_current_field(form, field[0]);
	scale_form(form, &rows, &cols);
    win_size.w = cols + 4;
    win_size.h = rows + 4;

	WINDOW *window = newwin(win_size.h, win_size.w, 10, (screen_size.w >> 1) - (win_size.w >> 1));
	WINDOW* form_win = derwin(window, rows, cols, 1, 1);
	set_form_win(form, window);
	set_form_sub(form, form_win);
	bool quit = false;
	while(!quit)
	{
		post_form(form);
		mvwprintw(window, 2, 2, "Index:" );
		wrefresh(window);
		wtimeout(window, 0);
		int ch = wgetch(window);
		switch(ch) {
			case 27:
				quit = true;
				break;
			case '\n':
				form_driver(form, REQ_VALIDATION);
				char* field_value = NULL;
				field_value = strdup(field_buffer(field[0], 0));
				int index = atoi(field_value);
				if(index < count_notes(note_list_head)) {
					selected_link->note->focused = false;
					NoteLink* first = note_list_head;
					size_t count = 0;
					while(first) {
						first->note->focused = index == count;
						if(index == count) {
							selected_link = first;
						}
						first = first->next;
						count++;
					}
				}

				quit = true;
				break;
			default:
				form_driver(form, ch);
				break;
		}
	}

	unpost_form(form);
	free_form(form);
	free_field(field[0]);

	wclear(window);
	
	delwin(window);
}

void create_new_note(NoteLink** selected_link, NoteLink** note_list_head, NoteLink* note_list_tail) {
	Note* new_note = show_create_window();
	if(new_note) {
		create_note_window(new_note);
		randomize_position(new_note);
		
		NoteLink* new_link = new_note_link();
		new_link->note = new_note;
		new_link->note->focused = true;
		new_link->prev = note_list_tail;
		if(note_list_tail) {
			note_list_tail->next = new_link;
		}
		if(*selected_link) {
			(*selected_link)->note->focused = false;
		}
		*selected_link = new_link;
		if(!*note_list_head) {
			*note_list_head = *selected_link;
		}
	}
}

typedef enum {
	SORT_MODE_CASCADE = 0,
	SORT_MODE_MOSAIC,
	SORT_MODE_RANDOM
} SortMode;

SortMode show_sort_window(NoteLink* note_link_head) {
	return SORT_MODE_CASCADE;
}

int main( int argc, char **argv ) {
	struct arguments arguments;
	arguments.list = 0;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	srand((unsigned int)time(NULL));

	NoteLink* note_list_head = NULL;
	cJSON* doc = NULL;

	if( file_exists( JSON_FILENAME ) ) {
		doc = file_to_json(JSON_FILENAME);
		note_list_head = json_to_list_node(doc);
		cJSON_Delete(doc);
	}

	if(arguments.list) {
		print_all_note_links(note_list_head);
		exit(EXIT_SUCCESS);
	}

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

	while(!quit) {
		selected_link = NULL;

		werase(stdscr);
		wnoutrefresh(stdscr);
		
		NoteLink* link = note_list_head;
		NoteLink* note_list_tail = link;
		Note* note = NULL; 
		int i = 0;
		while(link) {
			note_list_tail = link;
			Note* note = link->note;
			note->index = i;
			if(note->focused)
				selected_link = link;
			if(!note->archived) {
				create_note_window(note);
				note_window_display(note);
			}
			link = link->next;
			i++;
		}

		if(!selected_link && note_list_head) {
			selected_link = note_list_head;
			selected_link->note->focused = true;
		}
		if(selected_link) {
			note = selected_link->note;
		}

		doupdate();
		timeout(-1);
		int ch = getch();
		switch(ch) {
			case KEY_UP:
				if(note && note->window.position.y > 0) {
					note->window.position.y -= 1; 
				}
				break;
			case KEY_DOWN:
				if(note && note->window.position.y + window_size.h < screen_size.h) {
					note->window.position.y += 1; 
				}
				break;
			case KEY_LEFT:
				if(note && note->window.position.x > 0) {
					note->window.position.x -= 1; 
				}
				break;
			case KEY_RIGHT:
				if(note && note->window.position.x + window_size.w < screen_size.w) {
					note->window.position.x += 1;
				}
				break;
			case ' ':
				note->toggled = !note->toggled;
				break;
			case 'a':
				if(show_yesno_dialog("Do you really want archive this note?")) {
					archive_current_note(selected_link);
				}
				break;
			case 's': {
				SortMode sort_mode = show_sort_window(note_list_head);
				if(sort_mode == SORT_MODE_CASCADE) {
					NoteLink* a = note_list_head;
					int xx = 0, yy = 0;
					while(a) {
						a->note->window.position.x = xx;
						a->note->window.position.y = yy;
						xx += 4;
						yy += 4;
						a = a->next;
					}
				} else {
					NoteLink* a = note_list_head;
					int xx = 0, yy = 0;
					while(a) {
						a->note->window.position.x = xx;
						a->note->window.position.y = yy;
						xx += 41;
						yy += 0;
						a = a->next;
					}
				}
				} break;
			case 'g':
				goto_note(&selected_link, note_list_head);
				break;
			case 'c':
				create_new_note(&selected_link, &note_list_head, note_list_tail);
				break;
			case '?':
				show_help_window();
				break;
			case 'q':
			case 27:
				quit = true;
				break;
			case KEY_PPAGE:
				goto_previous_note(&selected_link, note_list_head);
				break;
			case KEY_NPAGE:
				goto_next_note(&selected_link, note_list_head);
				break;
			case KEY_DC:
				if(show_yesno_dialog("Do you really want to delete this note?")) {
					delete_current_note(selected_link, &note_list_head);
				}
				break;
		}
	}

	endwin();

	cJSON* updated_doc = link_list_to_json(note_list_head);
	char* printable_doc = print_formatted ? cJSON_Print(updated_doc) : cJSON_PrintUnformatted(updated_doc);
	cJSON_Delete(updated_doc);

	write_file_content(JSON_FILENAME, printable_doc);
	free(printable_doc);

	return EXIT_SUCCESS;
}

