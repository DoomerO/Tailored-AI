#include "sdlFrameGUI.h"
#include "eventHandler.h"
#include <SDL3/SDL_ttf.h>
#include <stdio.h>

#include "arena.c"
#include "sdlFrame.c"
#include "sdlFrameGUI.c"

void mainTick(SDL_Window *win, SDL_Renderer *rend, MenuState *menu){
  GUITextBox *tBox = Arena_get(&menu->memory, 0);
  int winw, winh;
  SDL_GetWindowSizeInPixels(win, &winw, &winh);
  GUITextBox_setDim(tBox, winw - 20, winh - 20);
  if(MouseHandler_hasButton(menu->mouseH, MOUSEB_LEFT) == MOUSE_CLICK)
    GUITextBox_setBuffer(tBox, "Left");
  if(MouseHandler_hasButton(menu->mouseH, MOUSEB_RIGHT) == MOUSE_CLICK)
    GUITextBox_setBuffer(tBox, "Right");
  if(MouseHandler_hasButton(menu->mouseH, MOUSEB_MID) == MOUSE_CLICK)
    GUITextBox_setBuffer(tBox, "Middle");
  float scroll = MouseHandler_getScroll(menu->mouseH);
  if(scroll){
    float prevSize = TTF_GetFontSize(tBox->font);
    TTF_SetFontSize(tBox->font, prevSize + scroll);
    tBox->outDated = true;
  }
}

void mainRender(SDL_Window *win, SDL_Renderer *rend, MenuState *menu){
  GUITextBox *tBox = Arena_get(&menu->memory, 0);

  SDL_SetRenderDrawColor(rend, Hex_expandRGB(0x000020));
  SDL_RenderClear(rend);

  GUITextBox_draw(tBox, rend);
}

Sint32 main(){
  SDL_Init(-1);
  
  TTF_Init();
  
  SDL_Window *win;
  SDL_Renderer *rend;
  SDL_CreateWindowAndRenderer("Main", 420, 420, SDL_WINDOW_RESIZABLE, &win, &rend);
  SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  
  SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND_PREMULTIPLIED);

  TTF_Font *font = TTF_OpenFont("fonts/font.ttf", 32.f);

  SDL_Event eve = {0};
  SDL_Time start, end = 0;
  SDL_GetCurrentTime(&start);

  SDL_srand(start);

  KeyboardHandler generalKb = {0};
  MouseHandler generalMo = {0};

  MenuState mainMenu = MenuState_new(mainTick, mainRender, &generalKb, &generalMo, 69420);
  *(GUITextBox*) Arena_alloc(&mainMenu.memory, sizeof(GUITextBox)) = GUITextBox_new(
    10, 10, 0, 0, font, Color_hex(0xffffffff), "Init"
  );

  while(eve.type != SDL_EVENT_QUIT){
    SDL_GetCurrentTime(&end);
    if(end - start > 16666666){
      start = end;
      handleEvents(&eve, &mainMenu);

      mainMenu.tick(win, rend, &mainMenu);
      mainMenu.render(win, rend, &mainMenu);

      SDL_RenderPresent(rend);
    }
    SDL_Delay(1);
  }

  MenuState_destroy(&mainMenu);

  TTF_CloseFont(font);
  TTF_Quit();

  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();

  SDL_Log("\nNormal Exit");
}