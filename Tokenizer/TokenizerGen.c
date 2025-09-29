#include <stdio.h> 
#include <stdlib.h> 
#include "Tokenizer.h"

void printTokenizer(Tokenizer tokenizer){
  for(size_t i = 0; i < tokenizer.token->count; i++){
    unsigned key = tokenizer.token->items[i];
    if(key >= tokenizer.pairMap->count){
      fprintf(stderr, "Error: Key [%u] is beyond bounds [%zu]\n", key, tokenizer.pairMap->count);
      return;
    }
    if(tokenizer.pairMap->items[key].a == key){
      printf("%c", key);
    } else{
      printf("[%u]", key);
    }
  }
  printf("\n");
}

int main(int argc, const char **argv){
  if(argc < 2){
    puts("Args: [file]");
    return 1;
  }
  Tokenizer tokenizer = newTokenizer();

  const char *filename = argv[1];

  FILE *fp = fopen(filename, "r");
  if (!fp){
    fprintf(stderr, "Error: Could not open file %s\n", filename);
    return 1;
  }
  initTokenizer(&tokenizer, fp);
  fclose(fp);
  filename = argc > 2 ? argv[2] : "TokenDump.bin";
  bakeTokenizer(tokenizer);
  
  if(dumpTokenizer(tokenizer, filename) == 0){
    fputs("Error: Failed to dump the tokenizer contents", stderr);
  } else{
    puts("Tokenizer contents dumped");
  }

  freeTokenizer(tokenizer);
}