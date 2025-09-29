#ifndef TOKENIZERH
#define TOKENIZERH
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include <ctype.h>

struct Pair{
  unsigned a, b;
};
typedef struct Pair Pair;

struct PairMap{
  Pair *items;
  size_t count, capacity;
};
typedef struct PairMap PairMap;

struct Hash{
  Pair key;
  size_t value;
};
typedef struct Hash Hash;

struct Token{
  unsigned *items;
  size_t count, capacity;
};
typedef struct Token Token;

struct Tokenizer{
  Token *token;
  PairMap *pairMap;
};
typedef struct Tokenizer Tokenizer;

void initPairMap(PairMap *pairMap){
  for(unsigned i = 0; i < 256; i++){
    da_append(pairMap, (Pair){i});
  }
}

void initToken(Token *token, FILE *fp){
  char c = fgetc(fp);

  while(c != EOF){
    da_append(token, (unsigned char)c);
    c = fgetc(fp);
  }
}

Tokenizer newTokenizer(){
  Tokenizer tokenizer = {
    .token = calloc(1, sizeof(Token)),
    .pairMap = calloc(1, sizeof(PairMap))
  };
  return tokenizer;
}

void initTokenizer(Tokenizer *tokenizer, FILE *file){
  initPairMap(tokenizer->pairMap);
  initToken(tokenizer->token, file);
}

int comapareHashAsc(const void *a, const void *b){
  const Hash *hashA = a, *hashB = b;
  return (int)hashA->value - (int)hashB->value;
}

int comapareHashDes(const void *a, const void *b){
  const Hash *hashA = a, *hashB = b;
  return (int)hashB->value - (int)hashA->value;
}

void increaseHash(Hash **hash, Pair pair){
  ptrdiff_t index = hmgeti(*hash, pair);
  if(index < 0) hmput(*hash, pair, 1);
  else (*hash)[index].value++;
}

void decreaseHash(Hash **hash, Pair pair){
  ptrdiff_t index = hmgeti(*hash, pair);
  (*hash)[index].value--;
}

void buildHash(Hash **hash, Tokenizer tokenizer){
  for(size_t i = 0; i < tokenizer.token->count - 1; i++){
    Pair sample = {tokenizer.token->items[i], tokenizer.token->items[i + 1]};
    increaseHash(hash, sample);
  }
}

ptrdiff_t maxIndexHash(Hash **hash){
  ptrdiff_t maxIndex = 0;
  for(size_t i = 1; i < hmlenu(*hash); i++){
    if((*hash)[i].value > (*hash)[maxIndex].value){
      maxIndex = i;
    }
  }
  return maxIndex;
}

void bakeTokenizer(Tokenizer tokenizer){
  Hash *hashArray = 0;
  Token out = {};
  size_t iteCount = 1;
  buildHash(&hashArray, tokenizer);

  for(;;){
    if(iteCount % 32); else printf("Iteration %zu\r", iteCount);
    ptrdiff_t maxIndex = maxIndexHash(&hashArray);
    if(hashArray[maxIndex].value <= 3) break;
    Pair maxPair = hashArray[maxIndex].key;
    da_append(tokenizer.pairMap, maxPair);
    out.count = 0;
    for(size_t i = 0; i < tokenizer.token->count; i++){
      if(i + 1 >= tokenizer.token->count){
        da_append(&out, tokenizer.token->items[i]);
        break;
      }
      Pair inPlace = {tokenizer.token->items[i], tokenizer.token->items[i + 1]};
      if(memcmp(&inPlace, &maxPair, sizeof(inPlace)) == 0){
        if(out.count){
          inPlace.a = out.items[out.count - 1];
          inPlace.b = tokenizer.token->items[i];
          decreaseHash(&hashArray, inPlace);
          inPlace.b = tokenizer.pairMap->count - 1;
          increaseHash(&hashArray, inPlace);
        }

        decreaseHash(&hashArray, maxPair);

        da_append(&out, tokenizer.pairMap->count - 1);
        i++;
        if(i + 1 < tokenizer.token->count){
          inPlace.a = tokenizer.token->items[i];
          inPlace.b = tokenizer.token->items[i + 1];
          decreaseHash(&hashArray, inPlace);
          
          inPlace.a = out.items[out.count - 1];
          increaseHash(&hashArray, inPlace);
        }
      } else{
        da_append(&out, tokenizer.token->items[i]);
      }
    }
    Token temp = *tokenizer.token;
    *tokenizer.token = out;
    out = temp;
    iteCount++;
  }
  da_free(out);
  hmfree(hashArray);
}

void getTokenText(String_Builder *strb, unsigned key, PairMap *pairMap){
  if(key == pairMap->items[key].a){
    da_append(strb, (char)pairMap->items[key].a);
  } else{
    getTokenText(strb, pairMap->items[key].a, pairMap);
    getTokenText(strb, pairMap->items[key].b, pairMap);
  }
}

void getTokenizerText(Tokenizer tokenizer){
  String_Builder strb = {0, 0, 0};
  for(unsigned i = 256; i < tokenizer.pairMap->count; i++){
    strb.count = 0;
    getTokenText(&strb, i, tokenizer.pairMap);
    printf("[%u] => [", i);
    for(size_t j = 0; j < strb.count; j++){
      printf(isprint(strb.items[j]) ? "%c" : "{%02X}", (unsigned char)strb.items[j]);
    }
    printf("]\n");
  }
  sb_free(strb);
}

void freeTokenizer(Tokenizer tokenizer){
  da_free(*tokenizer.token);
  da_free(*tokenizer.pairMap);
};

int dumpTokenizer(Tokenizer tokenizer, const char *dumpFile){
  return write_entire_file(dumpFile, tokenizer.pairMap->items, tokenizer.pairMap->count * sizeof(Pair));
}

int readTokenizer(Tokenizer tokenizer, const char *filename){
  String_Builder strb = {};
  if(!read_entire_file(filename, &strb)){
    return 1;
  }
  if(strb.count % sizeof(Pair)){
    return 2;
  }
  Pair *buffer = (void*)strb.items;
  size_t count = strb.count / sizeof(Pair);
  for(size_t i = 0; i < count; i++){
    da_append(tokenizer.pairMap, buffer[i]);
  }
  sb_free(strb);
  return 0;
}

#endif