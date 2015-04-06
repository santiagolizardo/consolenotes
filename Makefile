
all:
	gcc -o cn *.c -lncurses -lpanel -lmenu

clean:
	rm cn

.PHONY: clean

