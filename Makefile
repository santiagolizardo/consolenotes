
SOURCES=$(wildcard *.c) $(wildcard vendor/cJSON/cJSON.c)

all:
	gcc -o cn $(SOURCES) -lncurses -lpanel -lmenu -lform

clean:
	rm cn

.PHONY: clean

