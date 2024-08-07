#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "trie.h"

char* padronizaString(char* entrada)
{
  size_t tam, wctam;
  wchar_t *letra, *palavra;
  char *nova, *p;
  char *r, *w;
  const char idx[256] =    // mapeia [A-Z,0-9,tab] para [a-z,0-9,' ']
                           // e outros caracteres para '?' (63) 
    {
      0,   1,   2,   3,   4,   5,   6,   7,   8,  32,  10,  11,  12,  13,  14,  15,  // 000-015
     16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  // 016-031
     32,  63,  63,  63,  63,  63,  63,  63,  63,  63,  42,  63,  63,  63,  46,  63,  // 032-047
     48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  63,  63,  63,  63,  63,  63,  // 048-063
     63,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,  // 064-079
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,  63,  63,  63,  63,  63,  // 080-095
     63,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,  // 096-111
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,  63,  63,  63,  63, 127,  // 112-127
     63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  // 128-143
     63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  // 144-159
     63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  // 160-175
     63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  63,  // 176-191
     97,  97,  97,  97,  97,  97,  97,  99, 101, 101, 101, 101, 105, 105, 105, 105,  // 192-207
    100, 110, 111, 111, 111, 111, 111, 120,  48, 117, 117, 117, 117, 121,  63,  63,  // 208-223
     97,  97,  97,  97,  97,  97,  97,  99, 101, 101, 101, 101, 105, 105, 105, 105,  // 224-239
    111, 110, 111, 111, 111, 111, 111,  63,  48, 117, 117, 117, 117, 121, 112, 121   // 240-255
    }; 

  tam= strlen( entrada);
  wctam= (tam+1)*4;
  nova = (char*) malloc(tam+1);
  palavra = (wchar_t*) malloc(wctam); 
  mbstowcs( palavra, entrada, wctam );
  p = nova; letra = palavra;
  while (*letra != '\0')
    if(*letra >= 0 && *letra <= 255)
      *p++ = idx[*letra++];
    else{
      *p++ = 63;                     // coloca '?' nos caracteres fora do intervalo [0,255]
      letra++;
    }
  *p = '\0';
  free( palavra );

  /* remove espaços brancos consecutivos. String termina com '\n' ou '\0' */
  r = w = nova;
  while( *r == ' ' && *r!='\0' && *r!='\n') r++;
  while( *r != '\0' && *r!='\n'){
    *w++ = *r++;
    if( *r == ' ' ){
      while( *r == ' ' ) r++;
      if( *r != '\0' && *r!= '\n' )
	*w++ = ' ';
    }
  }  
  *w= '\0';
  return nova;
}

int main(int argc, char *argv[])
{
    FILE *arq;
    char *linha = NULL;
    char* result = NULL;
	  unsigned long tamLinha;
  
    //muda o locale para mostrar os simbolos "certos.
    char* local = setlocale(LC_ALL, "pt_BR.UTF-8");
    if (local == NULL) return -1;
    
    // abre arquivo de entrada
    if(argc != 2){
      printf( "Erro nos argumentos da chamada\n" );
      printf( "Chamada: ./qualFilme nomeArqFilmes\n" );
      return 0;
    }
    arq = fopen(argv[1], "r");
    if(arq == NULL){
      printf( "Erro na abertura do arquivo\n" );
      return 0;
    }

	  struct trieNode* root = createNode('\0');
	
    while (getline(&linha, &tamLinha, arq) != -1) {
      result = padronizaString(linha); 
      insertWord(root, result);
    }

    //FILE *exit_file = fopen("saida.txt", "w");
    char buffer[1024];


    while (fgets(buffer, 1024, stdin)) {

      buffer[strlen(buffer) - 1] = '\0';

      char op = buffer[0];

      char str[1024];
      //copia a string em buffer que vem depois do caracter de operação.
      strcpy(str, buffer + 2);

      //padroniza str.
      strcpy(str, padronizaString(str));
      printf("%s\n", buffer);

      if (op == 'p') {
        prefix(root, str, stdout);
      } else if (op == 'l')
        longestPrefix(root, str, stdout);
      else if (op == 'c') {
        char currPattern[1024];
        listPatterns(root, str, currPattern, 0, stdout);
      }

    }


    freeTree(root);
    return 0;
}
