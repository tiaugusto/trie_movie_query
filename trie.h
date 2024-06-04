#ifndef TRIE_H
#define TRIE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define NUM_CHARS 38

struct trieNode {
	struct trieNode *children[NUM_CHARS];
	char letter;
	//identifica se chegamos ao fim da string(nó folha).
	bool end;
};

struct trieNode* createNode (char data);

void freeNode (struct trieNode* node);

void insertWord (struct trieNode *root, char *word);

void printTrie(struct trieNode *root);

void prefix(struct trieNode *root, char *s);

#endif
