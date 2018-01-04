
CFLAGS=-O3 -g -Wall -pedantic
OUTPUT=cn
SOURCES=$(wildcard *.c) $(wildcard vendor/cJSON/cJSON.c)

LIBS=-lncurses -lpanel -lmenu -lform -lm

OSTYPE=$(shell echo $$OSTYPE)

ifeq ("$(OSTYPE)", "cygwin")
	LIBS+= -largp
endif
ifeq ("$(OSTYPE)", "darwin16")
	CFLAGS+= -I/usr/local/include -L/usr/local/lib
	LIBS+= -largp
endif

all:
	gcc $(CFLAGS) -o $(OUTPUT) $(SOURCES) $(LIBS) 

clean:
	rm -f $(OUTPUT)

.PHONY: clean

