
objs = trie.o qualFilme.o

all: qualFilme

# regras de ligacao
qualFilme: qualFilme.o trie.o
	   gcc qualFilme.o trie.o -o qualFilme

trie.o: trie.c trie.h
	gcc -c trie.c
	
qualFilme.o: qualFilme.c trie.h
	gcc -c qualFilme.c

clean:
	-rm -f *~ *.o
	
