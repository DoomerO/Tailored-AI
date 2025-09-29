#ifndef LAYERMODELH
#define LAYERMODELH

#include "activationFn.h"

struct Layer{
  Mat weight, bias, output;
  enum LayerFunc act;
};
typedef struct Layer Layer;

Layer newLayer(u32 inp, u32 out){
  Layer ly = {.weight={NULL, inp, out}, .bias={NULL, 1, out}, .output={NULL, 0, out}, EnumLayerLinear};
  matAlloc(&ly.weight);
  matAlloc(&ly.bias);
  matRandomize(ly.weight);
  matRandomize(ly.bias);
  return ly;
}

void outputLayer(Layer *ly, Mat in){
  matDot(in, ly->weight, ly->output);
  u32 batchSize = ly->output.r;
  for(u32 i = 0; i < batchSize; ++i){
    Mat view = {.data = matView(ly->output, i, 0), .r = 1, .c = ly->output.c};
    matAdd(view, ly->bias);
  }
  if (ly->act) LayerFuncList[ly->act](ly->output);
}

void freeLayer(Layer *ly){
  matFree(&ly->weight);
  matFree(&ly->bias);
  matFree(&ly->output);
}

struct LayerModel{
  u32 layerSize;
  Layer *layer;
  enum LossFunc loss;
};
typedef struct LayerModel LayerModel;

struct LayerData{
  Mat input, output;
};
typedef struct LayerData LayerData;

LayerModel newLayerModel(u32 layers, u32 inp, ...){
  LayerModel lm = {layers, malloc(layers * sizeof(Layer)), EnumLossSquared};
  va_list args;
  va_start(args, inp);
  for(u32 i = 0; i < layers; ++i){
    u32 next = va_arg(args, u32);
    lm.layer[i] = newLayer(inp, next);
    inp = next;
  }
  va_end(args);
  return lm;
}

void setLayerModelBatchSize(LayerModel *lm, u32 batchSize){
  if(lm->layer[0].output.r == batchSize) return;
  for(u32 i = 0; i < lm->layerSize; ++i){
    free(lm->layer[i].output.data);
    lm->layer[i].output.r = batchSize;
    matAlloc(&lm->layer[i].output);
  }
}

void outputLayerModel(LayerModel *lm, Mat in){
  setLayerModelBatchSize(lm, in.r);
  outputLayer(&lm->layer[0], in);
  for (u32 i = 1; i < lm->layerSize; ++i)
    outputLayer(&lm->layer[i], lm->layer[i - 1].output);
}

static inline Mat readLayerModelOutput(LayerModel *lm){
  return lm->layer[lm->layerSize - 1].output;
}

float costLayerModel(LayerModel *restrict lm, LayerData *restrict ld){
  Mat out = readLayerModelOutput(lm);
  float total = LossFuncList[lm->loss](out, ld->output);
  return total / ld->input.r;
}

void trainLayerModel(LayerModel *restrict lm, LayerData *restrict ld, float eps, float rate){
  float original, dcost;
  for (u32 l = 0; l < lm->layerSize; ++l) {
    Layer ly = lm->layer[l];
    u32 ite = ly.weight.r * ly.weight.c;
    for (u32 i = 0; i < ite; ++i){
      original = ly.weight.data[i];
      outputLayerModel(lm, ld->input);
      dcost = costLayerModel(lm, ld);
      ly.weight.data[i] += eps;
      outputLayerModel(lm, ld->input);
      dcost = costLayerModel(lm, ld) - dcost;
      dcost /= eps;
      ly.weight.data[i] = original - rate * dcost;
    }
    for (u32 j = 0; j < ly.bias.c; ++j){
      original = ly.bias.data[j];
      outputLayerModel(lm, ld->input);
      dcost = costLayerModel(lm, ld);
      ly.bias.data[j] += eps;
      outputLayerModel(lm, ld->input);
      dcost = costLayerModel(lm, ld) - dcost;
      dcost /= eps;
      ly.bias.data[j] = original - rate * dcost;
    }
  }
}

void freeLayerModel(LayerModel *lm){
  for(u32 i = 0; i < lm->layerSize; ++i)
    freeLayer(&lm->layer[i]);
  free(lm->layer);
}

LayerData newLayerData(LayerModel *lm, u32 size){
  LayerData ld = {
    .input = {NULL, size, lm->layer[0].weight.r},
    .output = {NULL, size, lm->layer[lm->layerSize - 1].weight.c}
  };
  matAlloc(&ld.input);
  matAlloc(&ld.output);
  return ld;
}

void fillLayerData(LayerData *restrict ld, ...){
  u32 ic = ld->input.c, oc = ld->output.c;
  va_list args;
  va_start(args, ld);
  for (u32 i = 0; i < ld->input.r; ++i) {
    for (u32 j = 0; j < ic; ++j)
      matWrite(ld->input, i, j, va_arg(args, int));
    for (u32 j = 0; j < oc; ++j)
      matWrite(ld->output, i, j, va_arg(args, int));
  }
  va_end(args);
}

void fillfLayerData(LayerData *restrict ld, ...){
  u32 ic = ld->input.c, oc = ld->output.c;
  va_list args;
  va_start(args, ld);
  for (u32 i = 0; i < ld->input.r; ++i) {
    for (u32 j = 0; j < ic; ++j)
      matWrite(ld->input, i, j, va_arg(args, double));
    for (u32 j = 0; j < oc; ++j)
      matWrite(ld->output, i, j, va_arg(args, double));
  }
  va_end(args);
}

void freeLayerData(LayerData *ld){
  matFree(&ld->input);
  matFree(&ld->output);
}

#endif