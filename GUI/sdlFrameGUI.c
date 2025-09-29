#include "sdlFrameGUI.h"

SDL_Color Color_hex(Uint32 hexColor){
  union{
    SDL_Color color;
    Uint32 hex;
  } convert = {.hex = hexColor};
  return convert.color;
}

GUIRect GUIRect_new(float x, float y, float w, float h, SDL_Color borderColor, SDL_Color fillColor){
  GUIRect gRect = {
    .x = x, .y = y,
    .w = w, .h = h,
    .borderColor = borderColor,
    .fillColor = fillColor
  };
  return gRect;
}

void GUIRect_setPos(GUIRect *gRect, float x, float y){
  SDL_assert(gRect);
  gRect->x = x, gRect->y = y;
}

void GUIRect_setDim(GUIRect *gRect, float w, float h){
  SDL_assert(gRect);
  gRect->w = w, gRect->h = h;
}

void GUIRect_setFillColor(GUIRect *gRect, SDL_Color fillColor){
  SDL_assert(gRect);
  gRect->fillColor = fillColor;
}

void GUIRect_setBorderColor(GUIRect *gRect, SDL_Color borderColor){
  SDL_assert(gRect);
  gRect->borderColor = borderColor;
}

void GUIRect_draw(GUIRect *gRect, SDL_Renderer *rend){
  SDL_assert(gRect);
  SDL_assert(rend);
  SDL_SetRenderDrawColor(rend, Color_expand(gRect->fillColor));
  SDL_RenderFillRect(rend, (void*)gRect);
  SDL_SetRenderDrawColor(rend, Color_expand(gRect->borderColor));
  SDL_RenderRect(rend, (void*)gRect);
}

void GUIRect_destroy(GUIRect *gRect){
  *gRect = (GUIRect){0};
}

static void GUITextBox_preRender(GUITextBox *tBox, SDL_Renderer *rend){
  tBox->outDated = false;
  SDL_Surface *canvas = TTF_RenderText_Blended_Wrapped(tBox->font, tBox->buffer, 0, tBox->textColor, tBox->bounds.w);
  if(tBox->preRender) SDL_DestroyTexture(tBox->preRender);
  tBox->preRender = SDL_CreateTextureFromSurface(rend, canvas);
  SDL_DestroySurface(canvas);
}

GUITextBox GUITextBox_new(float x, float y, float w, float h, TTF_Font *font, SDL_Color textColor, char *buffer){
  SDL_assert(font);
  GUITextBox tBox = {
    .bounds = {
      .x = x,
      .y = y,
      .w = w,
      .h = h
    },
    .font = font,
    .textColor = textColor,
    .buffer = buffer,
    .outDated = true
  };
  return tBox;
}

void GUITextBox_setPos(GUITextBox *tBox, float x, float y){
  SDL_assert(tBox);
  tBox->bounds.x = x, tBox->bounds.y = y;
}

void GUITextBox_setDim(GUITextBox *tBox, float w, float h){
  SDL_assert(tBox);
  tBox->outDated |= tBox->bounds.w != w || tBox->bounds.h != h;
  tBox->bounds.w = w, tBox->bounds.h = h;
}

void GUITextBox_setFont(GUITextBox *tBox, TTF_Font *font){
  SDL_assert(tBox);
  SDL_assert(font);
  tBox->outDated |= font != tBox->font;
  tBox->font = font;
}

void GUITextBox_setTextColor(GUITextBox *tBox, SDL_Color textColor){
  SDL_assert(tBox);
  tBox->outDated |= *(int*)&textColor != *(int*)&tBox->textColor;
  tBox->textColor = textColor;
}

void GUITextBox_draw(GUITextBox *tBox, SDL_Renderer *rend){
  SDL_assert(tBox);
  SDL_assert(rend);
  if(tBox->outDated) GUITextBox_preRender(tBox, rend);
  int w, h;
  TTF_GetStringSize(tBox->font, tBox->buffer, 0, &w, &h);
  SDL_FRect boundRect = {
    .x = tBox->bounds.x,
    .y = tBox->bounds.y,
    .w = SDL_min(w, tBox->bounds.w),
    .h = SDL_min(h * SDL_ceilf(w / tBox->bounds.w), tBox->bounds.h)
  };
  SDL_RenderTexture(rend, tBox->preRender, NULL, &boundRect);
}

void GUITextBox_setBuffer(GUITextBox *tBox, char *buffer){
  SDL_assert(tBox);
  SDL_assert(buffer);
  tBox->outDated |= buffer != tBox->buffer;
  tBox->buffer = buffer;
}

void GUITextBox_destroy(GUITextBox *tBox){
  SDL_assert(tBox);
  SDL_DestroyTexture(tBox->preRender);
  *tBox = (GUITextBox){0};
}