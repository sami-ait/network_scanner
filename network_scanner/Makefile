CC  = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Iinclude

SRC = src/main.c src/scanner.c
NAME = network_scanner

.PHONY: all clean re

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all