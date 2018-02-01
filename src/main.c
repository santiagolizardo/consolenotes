#include <stdlib.h>

#include <locale.h>

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
#include "goto_window.h"
#include "help_window.h"
#include "colors.h"
#include "input.h"
#include "sort_window.h"

#include "filesystem.h"

#include "dialogs.h"

#include "json_note.h"
#include "cJSON/cJSON.h"

#include "arguments.h"

#define DEFAULT_JSON_FILENAME "list.json"

const bool print_formatted = true;

Dimension screen_size;

int main( int argc, char **argv ) {
	struct arguments arguments;
	arguments.list = false;
	arguments.filename = NULL;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	if(arguments.filename == NULL) {
		arguments.filename = strdup(DEFAULT_JSON_FILENAME);
	}

	setlocale(LC_ALL, "");

	srand((unsigned int)time(NULL));

	NoteLink* note_list_head = NULL;
	cJSON* doc = NULL;

	if( file_exists(arguments.filename) ) {
		doc = file_to_json(arguments.filename);
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

	signal(SIGWINCH, resize_handler);

	getmaxyx(stdscr, screen_size.h, screen_size.w);
	wbkgd(stdscr, COLOR_PAIR(COLOR_PAIR_NO_COLOR));

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
				if(show_confirmation_dialog("Do you really want archive this note?")) {
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
				} else if(sort_mode == SORT_MODE_MOSAIC) {
					NoteLink* a = note_list_head;
					int xx = 0, yy = 0;
					while(a) {
						a->note->window.position.x = xx;
						a->note->window.position.y = yy;
						xx += 41;
						yy += 0;
						a = a->next;
					}
				} else {
					NoteLink* a = note_list_head;
					int xx = 0, yy = 0;
					while(a) {
						a->note->window.position.x = xx;
						a->note->window.position.y = yy;
						randomize_position(a->note);
						a = a->next;
					}
				}
				} break;
			case 'g': {
				size_t index = show_goto_window();
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
				} else {
					show_information_dialog("Invalid note index");
				}
				} break;
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
				if(show_confirmation_dialog("Do you really want to delete this note?")) {
					delete_current_note(selected_link, &note_list_head);
				}
				break;
		}
	}

	endwin();

	cJSON* updated_doc = link_list_to_json(note_list_head);
	char* printable_doc = print_formatted ? cJSON_Print(updated_doc) : cJSON_PrintUnformatted(updated_doc);
	cJSON_Delete(updated_doc);

	write_file_content(arguments.filename, printable_doc);
	free(printable_doc);

	free(arguments.filename);

	return EXIT_SUCCESS;
}

