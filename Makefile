all: main.c node.c key.c bptree.c
	gcc -g -o bptree.out main.c node.c key.c bptree.c -lreadline
debug: main.c node.c key.c bptree.c
	gcc -DDEBUG -g -o bptree.out main.c node.c key.c bptree.c -lreadline
