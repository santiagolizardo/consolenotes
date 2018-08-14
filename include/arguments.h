#pragma once

#include <argp.h>

const char *argp_program_version = "ConsoleNotes 1.0.0";
const char *argp_program_bug_address = "https://github.com/santiagolizardo/consolenotes/";

static char doc[] = "ConsoleNotes is a simple program to manage and display sticky notes on the terminal";

static struct argp_option options[] = {
	{"verbose",  'v', 0,      0,  "Produce verbose output", 0},
	{"list",    'l', 0,      0,  "List all the notes", 0},
	{"print",   'p', 0,      OPTION_ALIAS, NULL, 0},
	{"filename", 'f', "PATH", 0, "Path to the JSON file with the notes", 0},
	{NULL, 0, NULL, 0, NULL, 0}
};

struct arguments {
	bool list, verbose;
	char *filename;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	/* Get the input argument from argp_parse, which we
	  know is a pointer to our arguments structure. */
	struct arguments *arguments = state->input;

	switch (key) {
	case 'l':
	case 'p':
		arguments->list = true;
		break;
	case 'v':
		arguments->verbose = true;
		break;
	case 'f':
		arguments->filename = strdup(arg);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, NULL, doc, NULL, NULL, NULL};


