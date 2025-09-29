#include "sdlFrameGUI.h"

void handleEvents(SDL_Event *eve, MenuState *menu){
  while(SDL_PollEvent(eve))
  switch(eve->type){
  case SDL_EVENT_QUIT:
  return;

  case SDL_EVENT_WINDOW_DESTROYED:
    SDL_Quit();
  break;

  case SDL_EVENT_MOUSE_MOTION:
    MouseHandler_setPos(menu->mouseH, eve->motion.x, eve->motion.y);
    MouseHandler_move(menu->mouseH, eve->motion.xrel, eve->motion.yrel);
  break;

  case SDL_EVENT_MOUSE_BUTTON_DOWN:
    MouseHandler_pressButton(menu->mouseH, eve->button.button);
  break;

  case SDL_EVENT_MOUSE_BUTTON_UP:
    MouseHandler_releaseButton(menu->mouseH, eve->button.button);
  break;

  case SDL_EVENT_MOUSE_WHEEL:
    MouseHandler_scroll(menu->mouseH, eve->wheel.y);
  break;

  case SDL_EVENT_KEY_DOWN:
    KeyboardHandler_pressKey(menu->keyboardH, eve->key.key);
  break;

  case SDL_EVENT_KEY_UP:
    KeyboardHandler_releaseKey(menu->keyboardH, eve->key.key);
  break;

  case SDL_EVENT_WINDOW_FOCUS_LOST:
    KeyboardHandler_clear(menu->keyboardH);
    MouseHandler_clear(menu->mouseH);
  break;
  }
}