#ifndef BADGUY
#define BADGUY

#include "sprite.h"
#include "pos.h"
#include "vel.h"
#include "dim.h"
#include "main.h"
#include "missle.h"
#include <vector>

class Badguy : public Sprite
{
public:
  Badguy(Pos, Vel, Dim);
  virtual bool shouldDelete();
protected:
  unsigned long shootTime; 
};

class BadguyShooter : public Badguy 
{
public:
  BadguyShooter(Pos, Vel, Dim);
  void shoot(std::vector<Sprite *> &);
  void draw(XInfo &);
};

class BadguyLazer : public Badguy
{
public:
  BadguyLazer(Pos, Vel, Dim);
  void shoot(std::vector<Sprite *> &);
  void draw(XInfo &);
};

#endif
