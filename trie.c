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

void freeTree (struct trieNode *root) {
	if (!root)
		return;
	
	for (int i = 0; i < NUM_CHARS; i++) {
		if (!root -> children[i])
			freeTree(root -> children[i]);
	}

	free(root);
}


char idxToChar (int idx) {
	if (idx == 0)
		return '\0';
	else if (idx == 1)
		return ' ';
	else if (idx >= 2 && idx <= 11) 
		return (idx - 2) + '0';
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

	while ((*i) != '\0' && (*i) != '\n') {
		int idx = charToIdx(*i);
		if (!temp -> children[idx]) {
			temp -> children[idx] = createNode((*i));
		}
		temp = temp -> children[idx];
		i++;
	}	
	
	//o último nó a ser inserido marca o fim de determinada palavra. 
	temp -> end = true;
}


void printPrefixToFile (struct trieNode *root, char *prefix, char *currMovie, int level, FILE *exit_file) {
	if (root -> end) {
		currMovie[level] = '\0';
		fprintf(exit_file, "%s%s\n", prefix, currMovie);
	}

	//explora os caracteres que vem depois do prefixo em comum.
	for (int i = 0; i < NUM_CHARS; i++) {
		if (root -> children[i]) {
			currMovie[level] = idxToChar(i);
			printPrefixToFile(root -> children[i], prefix, currMovie, level + 1, exit_file);
		}
	}
}

void prefix (struct trieNode* root, char *s, FILE *exit_file) {
	char *i = s;

	struct trieNode *temp = root;
	//currMovie é um buffer que guarda o atual filme que contém o prefixo s.
	char currMovie[1024];

	//"descendo" na trie de acordo com o prefixo(s) para o último caracter desse prefixo.
	while (*i != '\0' && *i != '\n') {
		int idx = charToIdx((*i));
		temp = temp -> children[idx];
		i++;
	}

	//printa em exit file a partir de currMovie cada filme que contém o prefixo s.
	printPrefixToFile(temp, s, currMovie, 0, exit_file);
}

void longestPrefix (struct trieNode *root, char *currMovie, FILE *exit_file) {
	char longest[1024];

	//root começa na primeira letra do filme a ser analisado.
	struct trieNode *temp = root -> children[charToIdx(currMovie[0])];

	int level = 0;

	//no caso mais extremo, o maior prefixo de currMovie será ele mesmo.
	//caso contrário, basta descermos na árvore até encontrar um filho nulo.
	for (int i = 1; i < strlen(currMovie) + 1 && temp != NULL; i++) {
		longest[level] = temp -> letter;
		level++;
		temp = temp -> children[charToIdx(currMovie[i])];
	}

	longest[level] = '\0';
	fprintf(exit_file, "%s\n", longest);
}

//pattern é o padrão lido ao usar c padrao, e currPattern um buffer que armazena cada string que se encaixa em pattern.
void listPatterns (struct trieNode *root, char *pattern, char *currPattern, int level, FILE *exit_file) {
	if (!root)
		return;
	
	if (*pattern == '\0') {
		if (root -> end) {
			currPattern[level] = '\0';
			fprintf(exit_file, "%s\n", currPattern);
		}
	} else if (*pattern == '.') {
		//quando o caracter for '.' incluimos apenas um caracter e avançamos pattern.
		for (int i = 0; i < NUM_CHARS; i++) {
			if (root -> children[i]) {
				currPattern[level] = idxToChar(i);
				listPatterns(root -> children[i], pattern + 1, currPattern, level + 1, exit_file);
			}
		}
	} else if (*pattern == '*') {
		//chamada para tratar o caso em que * corresponde a zero caracteres.
		listPatterns(root, pattern + 1, currPattern, level, exit_file);
		for (int i = 0; i < NUM_CHARS; i++) {
			//chamada para o caso em que * corresponde a um ou mais caracteres(pattern não avança).
			if (root -> children[i]) {
				currPattern[level] = idxToChar(i);	
				listPatterns(root -> children[i], pattern, currPattern, level + 1, exit_file);
			}
		}
	} else {
		int idx = charToIdx(*pattern);
		if (root -> children[idx]) {
			currPattern[level] = (*pattern);
			listPatterns(root -> children[idx], pattern + 1, currPattern, level + 1, exit_file);
		}
	}
}