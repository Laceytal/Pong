FLAGS = -Wall -Werror -Wextra
CC = gcc

all: pong pong_interactive

pong: pong.c
	$(CC) $(FLAGS) pong.c

pong_interactive: pong_interactive.c 
	$(CC) $(FLAGS) pong_interactive.c 

rebuild: clean all

clean:
	rm pong pong_interactive