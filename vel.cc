#include "vel.h"

Vel::Vel(float dx, float dy)
  :dx_(dx),dy_(dy)
{}

float Vel::getX()
{
	return dx_;
}

float Vel::getY()
{
	return dy_;
}

void Vel::setX(float dx)
{
  dx_ = dx; 
}

void Vel::setY(float dy)
{
  dy_ = dy; 
}
