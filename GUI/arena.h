#ifndef ARENA_H_
#define ARENA_H_

#include <SDL3/SDL_stdinc.h>
#include <stdlib.h>

#define ARENAPADDING (sizeof(void*) << 1)

struct Arena{
  Uint8 *mem;
  Uint32 cur, cap;
};
typedef struct Arena Arena;

Arena Arena_new(Uint32 bytes);

bool Arena_setSize(Arena*, Uint32 bytes);

void *Arena_alloc(Arena*, Uint32 bytes);

void Arena_pop(Arena*, Uint32 bytes);

void *Arena_get(Arena*, Uint32 index);

void Arena_destroy(Arena*);

#endif