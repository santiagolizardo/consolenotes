#include "sort_window.h"

#include <curses.h>
#include <menu.h>
#include <stdlib.h>

const char* item_names[] = {
	"Cascade",
	"Tile",
	"Random",
};

SortMode show_sort_window() {
	int selection = 0;
	int n_choices = 3;
	ITEM **menu_items;
	menu_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

	for(int i = 0; i < n_choices; ++i) {
        menu_items[i] = new_item(item_names[i], NULL);
	}
	menu_items[n_choices] = (ITEM *)NULL;

	MENU *my_menu;
	my_menu = new_menu((ITEM **)menu_items);
	menu_opts_off(my_menu, O_ONEVALUE);
	post_menu(my_menu);
	refresh();

	int c;
	while((c = getch()) != '\n') {
		switch(c) {
			case KEY_DOWN:
		        menu_driver(my_menu, REQ_DOWN_ITEM);
		        selection++;
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				selection--;
				break;
		}
	}	

	free_item(menu_items[0]);
	free_item(menu_items[1]);
	free_item(menu_items[2]);
	free_menu(my_menu);

	return (SortMode)selection;
}

