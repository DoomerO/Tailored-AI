#include "sdlFrame.h"

void KeyboardHandler_pressKey(KeyboardHandler *kbH, SDL_Keycode key) {
  if (kbH->used >= sizeof(kbH->keys) / sizeof(*kbH->keys)) return;
  for (Uint32 i = 0; i < kbH->used; ++i) {
    if (kbH->keys[i] == key) return;
  }
  kbH->keys[kbH->used++] = key;
}


void KeyboardHandler_releaseKey(KeyboardHandler *kbH, SDL_Keycode key) {
  for (Uint32 i = 0; i < kbH->used; ++i) {
    if (kbH->keys[i] == key) {
      for (Uint32 j = i; j < kbH->used - 1; ++j)
        kbH->keys[j] = kbH->keys[j + 1];
      kbH->used--;
      kbH->keys[kbH->used] = 0;
      return;
    }
  }
}

void KeyboardHandler_clear(KeyboardHandler *kbH){
  *kbH = (KeyboardHandler){0};
}

bool KeyboardHandler_hasKey(KeyboardHandler *kbH, SDL_Keycode key){
  for(Uint32 i = 0; i < sizeof(kbH->keys) / sizeof(kbH->keys[0]); ++i)
  if(kbH->keys[i] == key) return true;
  return false;
}

void MouseHandler_setPos(MouseHandler *mouseH, float x, float y){
  mouseH->pos = (SDL_FPoint){.x = x, .y = y};
}

void MouseHandler_move(MouseHandler *mouseH, float dx, float dy){
  mouseH->move = (SDL_FPoint){.x = dx, .y = dy};
}

SDL_FPoint MouseHandler_getPos(MouseHandler *mouseH){
  return mouseH->pos;
}

SDL_FPoint MouseHandler_getMovement(MouseHandler *mouseH){
  SDL_FPoint ret = mouseH->move;
  mouseH->move = (SDL_FPoint){0};
  return ret;
}

void MouseHandler_pressButton(MouseHandler *mouseH, Uint8 button){
  SDL_assert(button > 0 && button <= 8);
  button = 1 << (button - 1);
  mouseH->down |= button;
}

void MouseHandler_releaseButton(MouseHandler *mouseH, Uint8 button){
  SDL_assert(button > 0 && button <= 8);
  button = 1 << (button - 1);
  mouseH->up |= button;
}

Uint8 MouseHandler_hasButton(MouseHandler *mouseH, Uint8 button){
  if(mouseH->down & button){
    if(mouseH->up & button){
      mouseH->down &= ~button;
      mouseH->up &= ~button;
      return MOUSE_CLICK;
    }
    return MOUSE_HOLD;
  }
  return MOUSE_NONE;
}

void MouseHandler_scroll(MouseHandler *mouseH, float scroll){
  mouseH->scroll = scroll;
}

float MouseHandler_getScroll(MouseHandler *mouseH){
  Sint16 scroll = mouseH->scroll;
  mouseH->scroll = 0.f;
  return scroll;
}

void MouseHandler_clear(MouseHandler *mouseH){
  *mouseH = (MouseHandler){.pos = mouseH->pos};
}

MenuState MenuState_new(MenuFunc fnTick, MenuFunc fnRender, KeyboardHandler *keyboardH, MouseHandler *mouseH, Uint32 auxBytes){
  MenuState menu = {
    .tick = fnTick,
    .render = fnRender,
    .keyboardH = keyboardH,
    .mouseH = mouseH,
    .memory = Arena_new(auxBytes)
  };
  return menu;
}

void MenuState_destroy(MenuState *menu){
  Arena_destroy(&menu->memory);
  *menu = (MenuState){0};
}

bool FPoint_equal(SDL_FPoint *a, SDL_FPoint *b){
  Uint64 *longA = (void*)a, *longB = (void*)b;
  return *longA == *longB;
}

void FPoint_copy(SDL_FPoint *dest, SDL_FPoint *src){
  Uint64 *longDest = (void*)dest, *longSrc = (void*)src;
  *longDest = *longSrc;
}