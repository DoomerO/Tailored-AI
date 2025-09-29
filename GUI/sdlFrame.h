#ifndef SDL_FRAME_H_
#define SDL_FRAME_H_

#include <SDL3/SDL.h>
#include "arena.h"

struct KeyboardHandler{
  SDL_Keycode keys[6];
  Uint16 used;
};
typedef struct KeyboardHandler KeyboardHandler;

void KeyboardHandler_pressKey(KeyboardHandler*, SDL_Keycode);
void KeyboardHandler_releaseKey(KeyboardHandler*, SDL_Keycode);
bool KeyboardHandler_hasKey(KeyboardHandler*, SDL_Keycode);
void KeyboardHandler_clear(KeyboardHandler*);

#define MOUSEB_LEFT (1 << 0)
#define MOUSEB_MID (1 << 1)
#define MOUSEB_RIGHT (1 << 2)
#define MOUSEB_EXTRA(x) (1 << (3 + (x)))
#define MOUSE_NONE 0
#define MOUSE_CLICK 1
#define MOUSE_HOLD 2

struct MouseHandler{
  SDL_FPoint pos;
  float scroll;
  Uint8 up;
  Uint8 down;
};
typedef struct MouseHandler MouseHandler;

void MouseHandler_move(MouseHandler*, float x, float y);
SDL_FPoint MouseHandler_getPos(MouseHandler*);
void MouseHandler_pressButton(MouseHandler*, Uint8 button);
void MouseHandler_releaseButton(MouseHandler*, Uint8 button);
Uint8 MouseHandler_hasButton(MouseHandler*, Uint8 button);
void MouseHandler_scroll(MouseHandler*, float scroll);
float MouseHandler_getScroll(MouseHandler*);
void MouseHandler_clear(MouseHandler*);

struct MenuState;
typedef void (*MenuFunc)(SDL_Window*, SDL_Renderer*, struct MenuState*);

struct MenuState{
  MenuFunc tick, render;
  KeyboardHandler *keyboardH;
  MouseHandler *mouseH;
  Arena memory;
};
typedef struct MenuState MenuState;


MenuState MenuState_new(MenuFunc fnTick, MenuFunc fnRender, KeyboardHandler*, MouseHandler*, Uint32 auxBytes);
void MenuState_destroy(MenuState *menu);

bool FPoint_equal(SDL_FPoint*, SDL_FPoint*);
void FPoint_copy(SDL_FPoint *dest, SDL_FPoint *src);

#endif