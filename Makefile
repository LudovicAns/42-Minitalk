# Compilation
CC = gcc

# Flags
CFLAGS = -Wall -Wextra -Werror

all: client server

client:
	@gcc client.c -o client

server:
	@gcc server.c -o server

clean:
	@rm -rf *.o

fclean: clean
	@rm -rf server
	@rm -rf client

re: fclean all

.PHONY: all clean fclean re