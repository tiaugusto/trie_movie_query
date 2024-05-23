#include "trie.h"

struct trieNode* createNode (char data) {
	struct trieNode* node = malloc(sizeof(struct trieNode));
	
	for (int i = 0; i < 40; i++)
		node -> children[i] = NULL;
		
	node -> end = false;
	node -> letter = data;
	
	return node;
}

void freeNode (struct trieNode* node) {
	for (int i = 0; i < 40; i++) {
		if (node -> children[i] != NULL)
			free(node -> children[i]);
	}	
	
	free(node);
}

void insertWord (struct trieNode *root, char *word) {
	//posições em children = 0-25(a-z); 26-35(0-9); 36(?);37(.);38(*);39(' ')
	char *i = word;
	struct trieNode *temp = root;
	
	while ((*i) != '\0' && (*i) != '\n') {
		int idx = (*i) - 'a';
		if ((*i) >= 'a' && (*i) <= 'z') {
			if (temp -> children[idx] == NULL)
				temp -> children[idx] = createNode((*i));
		} else if (isdigit((*i))) {
			idx = (*i) - '0' + 26;
			if (temp -> children[idx] == NULL)
				temp -> children[idx] = createNode((*i));
		} else if ((*i) == '?') {
			idx = 36;
			if (temp -> children[36] == NULL)
				temp -> children[36] = createNode('?');
		} else if ((*i) == '.') {
			idx = 37;
			if (temp -> children[37] == NULL)
				temp -> children[37] = createNode('.');
		} else if ((*i) == '*') {
			idx = 38;
			if (temp -> children[38] == NULL)
				temp -> children[38] = createNode('*');
		} else if ((*i) == ' ') {
		idx = 39;
			if (temp -> children[39] == NULL)
				temp -> children[39] = createNode(' ');	
		}

		i++;
		temp = temp -> children[idx];	
	}
	
	//o último nó a ser inserido é folha. 
	temp -> end = true;
	
}

void printTrie(struct trieNode* root) {
    // Prints the nodes of the trie
    if (!root)
        return;
    printf("%c", root->letter);
    for (int i=0; i<40; i++) {
        printTrie(root->children[i]);
    }
}

void printPrefix (struct trieNode *root, char *s) {
	if (!root)
		return;
	
	if (root -> end) {
		printf("\n%s", s);
	}

	printf("%c", root -> letter);
	for (int i=0; i<40; i++) {
        printPrefix(root->children[i], s);
    }
}

void prefix (struct trieNode* root, char *s) {
	char *i = s;

	struct trieNode *temp = root;

	while (*i != '\0' && *i != '\n') {
		int idx = *i - 'a';
		if (isdigit(*i)) {
			idx = *i - '0' + 26;
		} else if ((*i) == '?') {
			idx = 36;
		} else if ((*i) == '.') {
			idx = 37;
		} else if ((*i) == '*') {
			idx = 38;
		} else if ((*i) == ' ') {
			idx = 39;
		}

		printf("%c", *i);
		temp = temp -> children[idx];
		i++;
	}

	/*
	bool achouProx = false;
	for (int i = 0; i < 40 && !achouProx; i++) {
		if (temp -> children[i] != NULL) {
			temp  = temp -> children[i];
			achouProx = true;
		}
	}
	*/

	printPrefix(temp, s);

}
