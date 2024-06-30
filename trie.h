#ifndef TRIE_H
#define TRIE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define NUM_CHARS 39

struct trieNode {
	struct trieNode *children[NUM_CHARS];
	char letter;
	//identifica se chegamos ao fim da string(nó folha).
	bool end;
};

struct trieNode* createNode (char data);

void freeNode (struct trieNode* node);

char idxToChar (int idx);

int charToIdx (char c);

void insertWord (struct trieNode *root, char *word);

void prefix(struct trieNode *root, char *s, FILE *exit_file);

void longestPrefix (struct trieNode *root, char *currMovie, FILE *exit_file);

void listPatterns (struct trieNode *root, char *pattern, char *currPattern, int level, FILE *exit_file);

#endif
