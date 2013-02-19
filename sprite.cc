#include "sprite.h"
#include <iostream>
#include <math.h>
#include <vector>
#include "particle.h"
#include "stdlib.h"

using namespace std;

Sprite::Sprite(Pos pos)
  :pos_(pos),vel_(Vel(-2, 0)),dim_(Dim(30, 30))
{}

Sprite::Sprite(Pos pos, Vel vel, Dim dim)
  :pos_(pos),vel_(vel),dim_(dim)
{}

float Sprite::scale_ = 1.0f;
float Sprite::gamespeed_ = 1.0f;

void Sprite::update()
{
  pos_.offset(vel_.getX(), vel_.getY());
}

int Sprite::getX()
{
  return pos_.getX();
}

int Sprite::getY()
{
  return pos_.getY();
}

void Sprite::setX(float x)
{
  pos_.setX(x);
}

void Sprite::setY(float y)
{
  pos_.setY(y);
}

void Sprite::setDx(float x)
{
  vel_.setX(x);
}

void Sprite::setDy(float y)
{
  vel_.setY(y);
}

bool Sprite::checkForCollide(Sprite & s1, Sprite & s2)
{
  if (s1.isBad_ == false && s2.isBad_ == true)
  {
    if ((s1.pos_.getX() >= s2.pos_.getX()
                  && s1.pos_.getX() <= s2.pos_.getX() + s2.dim_.getW())
                  || (s2.pos_.getX() > s1.pos_.getX()
                  && s2.pos_.getX() < s1.pos_.getX() + s1.dim_.getW()))
      if (s1.pos_.getY() >= s2.pos_.getY()
                  && s1.pos_.getY() <= s2.pos_.getY() + s2.dim_.getH()
                  || (s2.pos_.getY() > s1.pos_.getY()
                  && s2.pos_.getY() < s1.pos_.getY() + s1.dim_.getH()))  
      {
        return true;
      } 
    return false;
  }
}

bool Sprite::shouldDelete()
{
  return false;
}

void Sprite::shoot(vector<Sprite *> &drawingMissiles)
{}

void Sprite::spawnParticles(vector<Sprite *> &drawingParticles)
{
  int toSpawn = rand() % 25 + 20; 
  //int toSpawn = rand() % 15 + 5; 

  for (int i = 0; i < toSpawn; i++)
  {
    float velX = -2 + (float)(rand() % 49 + 1) / 100.0f;
    float velY = (float)(rand() % 49 + 1) / 100.0f;

    float posX = pos_.getX() + (float)(rand() % (int)dim_.getW());  
    float posY = pos_.getY() + (float)(rand() % (int)dim_.getH());  

    Particle *p;

    if ((rand() % 10 + 1) < 4) 
      p = new ParticleSquare(Pos(posX, posY), Vel(velX, velY)); 
    else
      p = new ParticleCircle(Pos(posX, posY), Vel(velX, velY)); 

    drawingParticles.push_back(p);    
  }
}

void Sprite::rotate(float angle)
{
  
}
