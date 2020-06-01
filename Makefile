all: main.c 
	gcc -g -Wall -o sqlyt main.c

clean: 
	$(RM) sqlyt