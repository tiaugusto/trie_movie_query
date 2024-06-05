#include "trie.h"
#include <string.h>

struct trieNode* createNode (char data) {
	struct trieNode* node = malloc(sizeof(struct trieNode));
	
	for (int i = 0; i < NUM_CHARS; i++)
		node -> children[i] = NULL;
		
	node -> end = false;
	node -> letter = data;
	
	return node;
}

void freeNode (struct trieNode* node) {
	for (int i = 0; i < NUM_CHARS; i++) {
		if (node -> children[i] != NULL)
			free(node -> children[i]);
	}	
	
	free(node);
}


char idxToChar (int idx) {
	if (idx == 0)
		return '\0';
	else if (idx == 1)
		return ' ';
	else if (idx >= 2 && idx <= 11) 
		return (idx - 2) - '0';
	else if  (idx >= 12 && idx <= 37)
		return (idx - 12) + 'a';
	else if (idx == 38)
		return '?';
}

int charToIdx (char c) {
	int idx = 0;
	if (c == ' ') {
		idx = 1;
	} else if (c >= '0' && c <= '9') {
		idx = c - '0' + 2;
	} else if (c >= 'a' && c <= 'z') {
		idx = c - 'a' + 12;
	} else if (c == '?') {
		idx = 38;
	}

	return idx;
}

//ordem dos caracteres na trie: "\0", " ", "0", "1", "2", "3", "4", "5", "6", "7", "8","9","a", "b", "c", "d", 
//"e", "f", "g", "h", "i","j","k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "?" 
void insertWord (struct trieNode *root, char *word) {
	char *i = word;
	struct trieNode *temp = root;
	int idx = 0;

	while ((*i) != '\0' && (*i) != '\n') {
		int idx = charToIdx(*i);
		if (!temp -> children[idx]) {
			temp -> children[idx] = createNode((*i));
		}
		temp = temp -> children[idx];
		i++;
	}	
	
	//o último nó a ser inserido é folha. 
	temp -> end = true;
}

void printTrie(struct trieNode* root) {
    // Prints the nodes of the trie
    if (!root)
        return;
    printf("%c", root->letter);
    for (int i=0; i<NUM_CHARS; i++) {
        printTrie(root->children[i]);
    }
}

void printToFile (struct trieNode *root, char *prefix, char *currMovie, int level){ //FILE *exit_file) {
	if (root -> end) {
		currMovie[level] = '\0';
		printf("%s%s\n", prefix, currMovie);
	}

	for (int i = 0; i < NUM_CHARS; i++) {
		if (root -> children[i]) {
			currMovie[level] = idxToChar(i);
			printToFile(root -> children[i], prefix, currMovie, level + 1);
		}
	}
}

void prefix (struct trieNode* root, char *s) {
	char *i = s;

	struct trieNode *temp = root;
	char currMovie[1024];

	while (*i != '\0' && *i != '\n') {
		int idx = charToIdx((*i));
		temp = temp -> children[idx];
		i++;
	}

	printToFile(temp, s, currMovie, 0);
}