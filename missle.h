#ifndef MISSLE
#define MISSLE

#include "pos.h"
#include "main.h"
#include "sprite.h"

class Projectile : public Sprite
{
public:
  Projectile(Pos, Vel, Dim, bool);
  void update();
  virtual bool shouldDelete();
};

class Missile : public Projectile
{
public:
  Missile(Pos, Vel, Dim, bool);
  void draw(XInfo &);
};

class Lazer : public Projectile 
{
public:
  Lazer(Pos, Vel, Dim, bool);
  void draw(XInfo &);
  virtual bool shouldDelete();
};

#endif
