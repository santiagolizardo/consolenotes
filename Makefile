
CFLAGS=-std=c11 -O3 -g -Wall -pedantic
OUTPUT=cn
SOURCES=$(wildcard *.c) $(wildcard vendor/cJSON/cJSON.c)
MAIN_SOURCES=$(filter-out test_runner.c,$(SOURCES))
TEST_SOURCES=$(filter-out main.c,$(SOURCES))

LIBS=-lncurses -lpanel -lmenu -lform -lm
TEST_LIBS=$(LIBS) -lcunit

OSTYPE=$(shell echo $$OSTYPE)

ifeq ("$(OSTYPE)", "cygwin")
	LIBS+= -largp
endif
ifneq (, $(filter "$(OSTYPE)", "darwin16" "darwin17"))
	CFLAGS+= -I/usr/local/include -L/usr/local/lib
	LIBS+= -largp
endif

all:
	gcc $(CFLAGS) -o $(OUTPUT) $(MAIN_SOURCES) $(LIBS)

test:
	gcc $(CFLAGS) -o test_runner $(TEST_SOURCES) $(TEST_LIBS)

.PHONY: clean
clean:
	rm -f $(OUTPUT)


