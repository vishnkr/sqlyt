all: main.c memory.c buffer.c btree.c
	gcc -g -Wall -o sqlyt main.c memory.c buffer.c btree.c

clean: 
	$(RM) sqlyt