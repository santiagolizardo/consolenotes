
[![Build Status](https://travis-ci.org/santiagolizardo/consolenotes.svg?branch=master)](https://travis-ci.org/santiagolizardo/consolenotes)

**ConsoleNotes** is a simple sticky notes application for terminal lovers. Use it to save tasks, reminders and other useful information in a simple textual/visual way.

![Screenshot](screenshot.png)

Features
========

- Notes stored in JSON format
- Arrange notes in tiles, cascade or randomly (shuffle)
- Jump to note
- Fold/Unfold notes
- UTF8 support

Code
====

- Written in C (std=c17)
- Dependencies: Make, cJSON, Ncurses5, Cunit (Unit tests only)

Build
=====

```shell
git clone --recurse-submodules https://github.com/santiagolizardo/consolenotes.git
cd consolenotes
make
make run
```

Usage
=====

Use the cursor keys to move a note.
Type 'c' to open the note creation dialog.
Use DELETE key to delete notes.
Use SPACE key to collapse/expand the selected note.
Use the ESCAPE key to close dialogs or exit the app.

Issues
======

Set the environment variable ESCDELAY to a low value if you don't want to wait after the ESCAPE key is pressed.

