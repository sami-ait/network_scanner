CC  = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Iinclude

SRC = src/main.c src/scanner.c src/ports.c
HEADERS = include/scanner.h include/ports.h 
NAME = network_scanner

.PHONY: all clean re test

all: $(NAME)

$(NAME): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

test: tests/test_ports.c src/ports.c $(HEADERS)
		$(CC) $(CFLAGS) tests/test_ports.c src/ports.c -o test_ports
		./test_ports

clean:
	rm -f $(NAME)

re: clean all