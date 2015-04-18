
OUTPUT=cn
SOURCES=$(wildcard *.c) $(wildcard vendor/cJSON/cJSON.c)

all:
	gcc -o $(OUTPUT) $(SOURCES) -lncurses -lpanel -lmenu -lform -lm

clean:
	rm -f $(OUTPUT)

.PHONY: clean

