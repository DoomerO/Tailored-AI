#include <stdio.h> 
#include <stdlib.h> 
#include "Tokenizer.h"

void printTokenizer(Tokenizer tokenizer){
  for(size_t i = 0; i < tokenizer.token->count; i++){
    unsigned key = tokenizer.token->items[i];
    if(key < tokenizer.pairMap->count){
      fprintf(stderr, "Error: Key [%u] is beyond bounds [%zu]", key, tokenizer.pairMap->count);
    }
    if(tokenizer.pairMap->items[key].a == key){
      printf("%c", key);
    } else{
      printf("[%u]", key);
    }
  }
  printf("\n");
}

int main(int argc, char **argv){
  if(argc < 2){
    puts("Args: [file]");
    return 1;
  }
  Tokenizer tokenizer = newTokenizer();

  const char *filename = argv[1];
  int error = readTokenizer(tokenizer, filename);
  switch (error){
  case 1:
    fputs("Error: File could not be read", stderr);
    return 1;
  case 2:
    fputs("Error: File doesn't match expected format", stderr);
    return 1;
  }

  getTokenizerText(tokenizer);

  freeTokenizer(tokenizer);
}