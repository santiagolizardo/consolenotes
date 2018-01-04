#include <curses.h>
#include <menu.h>
#include <stdlib.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

char *choices[] = {
	"Choice 1",
	"Choice 2",
	"Choice 3",
	"Choice 4",
	"Exit",
  };
char *achoices[] = {
	"1)",
	"2)",
	"3)",
	"4)",
	" )",
};

int main_menu()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
	int n_choices, i;
	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

	char a[2];
	for(i = 0; i < n_choices; ++i) {
		sprintf( a, "%d", i);
	        my_items[i] = new_item(achoices[i], choices[i]);
	}
	my_items[n_choices] = (ITEM *)NULL;

	my_menu = new_menu((ITEM **)my_items);
	mvprintw(LINES - 2, 0, "F1 to Exit");
	menu_opts_off(my_menu, O_ONEVALUE);
	post_menu(my_menu);
	refresh();

	while((c = getch()) != KEY_F(1))
	{   switch(c)
	    {	case KEY_DOWN:
		        menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case KEY_HOME:
				menu_driver(my_menu, REQ_FIRST_ITEM);
				break;
			case KEY_END:
				menu_driver(my_menu, REQ_LAST_ITEM);
				break;
			case ' ':
				menu_driver(my_menu, REQ_TOGGLE_ITEM);
				break;
			case '\n':
				endwin();
				exit(0);
				break;
		}
	}	

	free_item(my_items[0]);
	free_item(my_items[1]);
	free_menu(my_menu);
	endwin();
	return 0;
}

