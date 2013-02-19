#include "particle.h"
#include "main.h"
#include <stdlib.h> 

const unsigned long LIFETIME = 90;

Particle::Particle(Pos pos, Vel vel, Dim dim)
  :Sprite(pos, vel, dim)
{lifetime_ = 0;}

bool Particle::shouldDelete()
{
  lifetime_ += 1;
  if (lifetime_ > LIFETIME)
  {
    return true;
  } 
  return false;
}

void ParticleSquare::draw(XInfo &xinfo)
{
  int gc_num = rand() % 4;
  XDrawRectangle(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[gc_num], 
                  pos_.getX() * scale_, 
                  pos_.getY() * scale_, 
                  dim_.getW() * scale_, 
                  dim_.getH() * scale_);  
}

void ParticleCircle::draw(XInfo &xinfo)
{
  int gc_num = rand() % 4;
  XDrawArc(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[gc_num], 
                  (pos_.getX() - (dim_.getW()/2)) * scale_, 
                  (pos_.getY() - (dim_.getH()/2)) * scale_, 
                  dim_.getW() * scale_, 
                  dim_.getH() * scale_, 
                  0, 23040);
}

ParticleSquare::ParticleSquare(Pos pos, Vel vel)
  :Particle(pos, vel, Dim(5, 5))
{}

ParticleCircle::ParticleCircle(Pos pos, Vel vel)
  :Particle(pos, vel, Dim(1, 1))
{}

