#include "dim.h"

Dim::Dim(float w, float h)
  :w_(w),h_(h)
{}

float Dim::getW()
{
  return w_;
}

float Dim::getH()
{
  return h_;
}

void Dim::setW(float w)
{
  w_ = w; 
}

void Dim::setH(float h)
{
  h_ = h; 
}
