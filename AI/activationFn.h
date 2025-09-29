#ifndef ACTIVATIONFNH
#define ACTIVATIONFNH

#include "lowmat.h"
#include <math.h>

void LayerSigmoid(Mat out);
void LayerRectify(Mat out);
void LayerHeaviside(Mat out);
void LayerSoftmax(Mat out);
enum LayerFunc{
  EnumLayerLinear,
  EnumLayerSigmoid,
  EnumLayerRectify,
  EnumLayerHeaviside,
  EnumLayerSoftmax
};
extern void (*LayerFuncList[])(Mat);
float LossSquared(Mat out, Mat targ);
float LossAbsolute(Mat out, Mat targ);
float LossCategory(Mat out, Mat targ);
float LossBinary(Mat out, Mat targ);
enum LossFunc{
  EnumLossSquared,
  EnumLossAbsolute,
  EnumLossCategory,
  EnumLossBinary
};
extern float (*LossFuncList[])(Mat, Mat);

#endif

#ifdef ACTIVATIONFNH

void LayerSigmoid(Mat inp){
  float val;
  for(size_t i = 0; i < inp.r * inp.c; i++){
    val = inp.data[i];
    inp.data[i] = 1.f / (expf(-val) + 1.f);
  }
}

void LayerRectify(Mat inp){
  float val;
  for(size_t i = 0; i < inp.r * inp.c; i++){
    val = inp.data[i];
    inp.data[i] = val <= 0.f ? 0.f : val;
  }
}

void LayerHeaviside(Mat inp){
  float val;
  for(size_t i = 0; i < inp.r * inp.c; i++){
    val = inp.data[i];
    inp.data[i] = val < 0.f ? 0.f : 1.f;
  }
}

void LayerSoftmax(Mat inp){
  float total;
  for(unsigned i = 0; i < inp.r; i++){
    total = 0.f;
    for(unsigned j = 0; j < inp.c; j++)
      total += expf(inp.data[i * inp.c + j]);
    for(unsigned j = 0; j < inp.c; j++)
      inp.data[i * inp.c + j] = expf(inp.data[i * inp.c + j]) / total;
  }
}

void (*LayerFuncList[])(Mat) = {
  NULL,
  LayerSigmoid,
  LayerRectify,
  LayerHeaviside,
  LayerSoftmax
};

float LossSquared(Mat out, Mat targ){
  float total = 0.f;
  for (size_t i = 0; i < out.r * out.c; i++){
    float diff = out.data[i] - targ.data[i];
    total += diff * diff;
  }
  return total;
}

float LossAbsolute(Mat out, Mat targ){
  float total = 0.f;
  for (size_t i = 0; i < out.r * out.c; i++){
    float diff = out.data[i] - targ.data[i];
    total += diff < 0.f ? -diff : diff;
  }
  return total;
}

float LossCategory(Mat out, Mat targ){
  float total = 0.f;
  for (size_t i = 0; i < out.r * out.c; i++){
    float y = targ.data[i], py = out.data[i];
    if(py <= 0.f) py = 1e-7f;
    total += y * logf(py);
  }
  return -total;
}

float LossBinary(Mat out, Mat targ){
  float total = 0.f;
  for (size_t i = 0; i < out.r * out.c; i++){
    float y = targ.data[i], py = out.data[i];
    if(py <= 0.f) py = 1e-7f;
    if(py >= 1.f) py = 1.f - 1e-7f;
    total -= y * logf(py) + (1.f - y) * logf(1.f - py);
  }
  return total;
}

float (*LossFuncList[])(Mat, Mat) = {
  LossSquared,
  LossAbsolute,
  LossCategory,
  LossBinary
};

#endif