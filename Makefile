
SOURCES=$(wildcard *.c) $(wildcard vendor/cJSON/cJSON.c)

all:
	gcc -o cn $(SOURCES) -lncurses -lpanel -lmenu -lform -lm

clean:
	rm cn

.PHONY: clean

