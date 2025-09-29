void handleEvents(SDL_Event *eve, MenuState *menu){
  while(SDL_PollEvent(eve))
  switch(eve->type){
  case SDL_EVENT_QUIT:
  return;

  case SDL_EVENT_WINDOW_DESTROYED:
    SDL_Quit();
  break;

  case SDL_EVENT_MOUSE_MOTION:
    MouseHandler_move(menu->mouseH, eve->motion.x, eve->motion.y);
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
  }
}