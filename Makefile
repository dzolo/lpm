# Organization: FIT VUT
# Project:      PDS - Longest Prefix Match
# File:         MakeFile
# Author:       Ondřej Fibich <xfibic01@stud.fit.vutbr.cz>

CFLAGS=-std=c99 -Wall -O3
NAME=lpm
AUTHOR=xfibic01
CC=gcc

all: $(NAME)

$(NAME): lpm.c trie.o
	$(CC)  -o $@ lpm.c trie.o $(CFLAGS)

trie.o: trie.c trie.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o $(NAME)

pack:
	cp ../doc/dokumentace.pdf ./dokumentace.pdf
	zip $(AUTHOR).zip *.c *.h Makefile dokumentace.pdf
