
CFLAGS=-O2 -g
OUTPUT=cn
SOURCES=$(wildcard *.c) $(wildcard vendor/cJSON/cJSON.c)

all:
	gcc $(CFLAGS) -o $(OUTPUT) $(SOURCES) -lncurses -lpanel -lmenu -lform -lm 

clean:
	rm -f $(OUTPUT)

.PHONY: clean

