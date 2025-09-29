#include "arena.h"

Arena Arena_new(Uint32 bytes){
  Uint32 padSize = (bytes / ARENAPADDING + !!(bytes % ARENAPADDING)) * ARENAPADDING;
  Arena arena = {
    .mem = malloc(padSize),
    .cur = 0,
    .cap = padSize
  };
  return arena;
}

bool Arena_setSize(Arena *arena, Uint32 bytes){
  Uint32 padSize = (bytes / ARENAPADDING + !!(bytes % ARENAPADDING)) * ARENAPADDING;
  void *newMem = realloc(arena->mem, padSize);
  if(!newMem) return false;
  arena->mem = newMem;
  arena->cap = padSize;
  return true;
}

void *Arena_alloc(Arena *arena, Uint32 bytes){
  if(arena->cur + bytes > arena->cap) return NULL;
  void *allocMem = arena->mem + arena->cur;
  arena->cur += bytes;
  return allocMem;
}

void Arena_pop(Arena* arena, Uint32 bytes){
  arena->cur -= bytes > arena->cur ? arena->cur : bytes;
}

void *Arena_get(Arena *arena, Uint32 index){
  if(index >= arena->cur) return NULL;
  return arena->mem + index;
}

void Arena_destroy(Arena *arena){
  free(arena->mem);
  *arena = (Arena){0};
}