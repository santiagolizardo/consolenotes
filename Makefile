
CFLAGS=-std=c11 -O3 -g -Wall -Wextra -Wpedantic -Iinclude -Ivendor -D_POSIX_C_SOURCE=200809L
MAIN_OUTPUT=cn
MAIN_SOURCES=$(wildcard src/*.c) $(wildcard vendor/cJSON/cJSON.c)
TEST_OUTPUT=test_runner
TEST_SOURCES=$(filter-out src/main.c,$(MAIN_SOURCES)) $(wildcard test/*.c)

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
	gcc $(CFLAGS) -o $(MAIN_OUTPUT) $(MAIN_SOURCES) $(LIBS)

test:
	gcc $(CFLAGS) -o $(TEST_OUTPUT) $(TEST_SOURCES) $(TEST_LIBS)

clean:
	rm -f $(MAIN_OUTPUT) $(TEST_OUTPUT)

format:
	clang-format -i --sort-includes src/*.c include/*.h

.PHONY: clean test

