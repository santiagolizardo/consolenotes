
CFLAGS=-O3 -g -Wall -pedantic
OUTPUT=cn
SOURCES=$(wildcard *.c) $(wildcard vendor/cJSON/cJSON.c)

LIBS=-lncurses -lpanel -lmenu -lform -lm

OSTYPE=$(shell echo $$OSTYPE)

ifeq ("$(OSTYPE)", "cygwin")
	LIBS+= -largp
endif
ifneq (, $(filter "$(OSTYPE)", "darwin16" "darwin17"))
	CFLAGS+= -I/usr/local/include -L/usr/local/lib
	LIBS+= -largp
endif

all:
	gcc $(CFLAGS) -o $(OUTPUT) $(SOURCES) $(LIBS) 

.PHONY: clean
clean:
	rm -f $(OUTPUT)


