CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wwrite-strings -Wshadow -Wunreachable-code -std=c99
OBJ=main.o display.o util.o game.o move.o
LDFLAGS=
EXEC=play_rushHour

all: main clean

main: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(EXEC)

display.o: display.c display.h
	$(CC) -o display.o -c display.c $(CFLAGS)

util.o: util.c util.h
	$(CC) -o util.o -c util.c $(CFLAGS)

game.o: game.c game.h
	$(CC) -o game.o -c game.c $(CFLAGS)

move.o: move.c move.h
	$(CC) -o move.o -c move.c $(CFLAGS)

clean:
	rm -f $(OBJ)

mrproper: clean
	rm -rf $(EXEC)