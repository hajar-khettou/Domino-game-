CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = main.c domino.c joueur.c jeu.c
OBJ = $(SRC:.c=.o)
TARGET = domino_game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

