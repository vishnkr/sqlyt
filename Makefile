all: main.c memory.c buffer.c
	gcc -g -Wall -o sqlyt main.c memory.c buffer.c 

clean: 
	$(RM) sqlyt