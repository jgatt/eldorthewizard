#ifndef SPRITE
#define SPRITE

#include "pos.h"
#include "main.h"
#include "vel.h"
#include "dim.h"
#include <vector>

class Sprite
{
public:
  Sprite(Pos);
  Sprite(Pos, Vel, Dim); 
  virtual void draw(XInfo &) = 0;
  virtual void update();

  virtual void setX(float);
  virtual void setY(float);

  virtual void setDx(float);
  virtual void setDy(float);
  
  virtual void shoot(std::vector<Sprite *> &);
  int getX();
  int getY();

  virtual bool shouldDelete();
  static bool checkForCollide(Sprite &, Sprite &);
  virtual void spawnParticles(std::vector<Sprite *> &);
  virtual void rotate(float);

  static float gamespeed_;
  static float scale_;
protected:
  Pos pos_;
  Vel vel_;
  Dim dim_;
  bool isBad_;
};

#endif
