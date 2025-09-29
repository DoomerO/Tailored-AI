#ifndef SDL_FRAME_GUI_H_
#define SDL_FRAME_GUI_H_

#include "sdlFrame.h"
#include "SDL3/SDL_ttf.h"

#define Color_expand(_color) (_color).r, (_color).g, (_color).b, (_color).a
#define Hex_expandRGBA(_hexColor) (Uint8)((_hexColor) >> 24), (Uint8)((_hexColor) >> 16), (Uint8)((_hexColor) >> 8), (Uint8)(_hexColor)
#define Hex_expandRGB(_hexColor) (Uint8)((_hexColor) >> 16), (Uint8)((_hexColor) >> 8), (Uint8)((_hexColor)), (Uint8)(255)

SDL_Color Color_hex(Uint32 hexColor);

struct GUIRect{
  float x, y, w, h;
  SDL_Color borderColor, fillColor;
};
typedef struct GUIRect GUIRect;

GUIRect GUIRect_new(float x, float y, float w, float h, SDL_Color borderColor, SDL_Color fillColor);
void GUIRect_setPos(GUIRect*, float x, float y);
void GUIRect_setDim(GUIRect*, float w, float h);
void GUIRect_setFillColor(GUIRect*, SDL_Color);
void GUIRect_setBorderColor(GUIRect*, SDL_Color);
void GUIRect_draw(GUIRect*, SDL_Renderer*);
void GUIRect_destroy(GUIRect*);

struct GUITextBox{
  SDL_FRect bounds;
  TTF_Font *font;
  SDL_Color textColor;
  char *buffer;
  SDL_Texture *preRender;
  bool outDated;
};
typedef struct GUITextBox GUITextBox;

GUITextBox GUITextBox_new(float x, float y, float w, float h, TTF_Font*, SDL_Color, char *buffer);
void GUITextBox_setPos(GUITextBox*, float x, float y);
void GUITextBox_setDim(GUITextBox*, float w, float h);
void GUITextBox_setFont(GUITextBox*, TTF_Font*);
void GUITextBox_setTextColor(GUITextBox*, SDL_Color);
void GUITextBox_setBuffer(GUITextBox*, char*);
void GUITextBox_draw(GUITextBox*, SDL_Renderer*);
void GUITextBox_destroy(GUITextBox*);

#endif