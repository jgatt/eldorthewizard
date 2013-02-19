#include "pos.h"
#include "sprite.h"

Pos::Pos(float x, float y)
  :x_(x),y_(y)
{}

float Pos::getX()
{
	return x_;
}

float Pos::getY()
{
	return y_;
}

void Pos::setX(float x)
{
  x_ = x; 
}

void Pos::setY(float y)
{
  y_ = y; 
}

void Pos::offset(float x, float y)
{
  x_ += x * Sprite::gamespeed_;
  y_ += y * Sprite::gamespeed_;    
}
