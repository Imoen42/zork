BIN = game

CFLAGS = -Wall -g -std=c99

CC = gcc

SRC = game.c room.c item.c character.c command.c

all:
	$(CC) -o $(BIN) $(SRC) $(CFLAGS)

run:
	./$(BIN)

clean:
	rm -f $(BIN)
