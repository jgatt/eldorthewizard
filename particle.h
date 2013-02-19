#ifndef PARTICLE
#define PARTICLE

#include "sprite.h"

class Particle : public Sprite
{
public:
  Particle(Pos, Vel, Dim);
  bool shouldDelete(); 
  virtual void draw(XInfo &) = 0;
protected:
  unsigned long lifetime_;
};

class ParticleSquare : public Particle
{
public:
  ParticleSquare(Pos, Vel);
  void draw(XInfo &);
};

class ParticleCircle : public Particle
{
public:
  ParticleCircle(Pos, Vel);
  void draw(XInfo &);
};

#endif
