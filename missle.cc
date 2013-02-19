#include "missle.h"
#include <iostream>
#include <stdlib.h>

const int G = 4;
const int ZERO = 0;
const int FULLDEGREE = 23040;

Projectile::Projectile(Pos pos, Vel vel, Dim dim, bool isBad)
  :Sprite(pos, vel, dim)
{
  isBad_ = isBad;
}

Missile::Missile(Pos pos, Vel vel, Dim dim, bool isBad)
  :Projectile(pos, vel, dim, isBad)
{}

Lazer::Lazer(Pos pos, Vel vel, Dim dim, bool isBad)
  :Projectile(pos, vel, dim, isBad)
{}

void Lazer::draw(XInfo &xinfo)
{
  XFillRectangle(xinfo.display,
                  xinfo.win,
                  xinfo.gc[0],
                  pos_.getX() * scale_,
                  pos_.getY() * scale_,
                  dim_.getW() * scale_,
                  dim_.getH() * scale_);

}

void Missile::draw(XInfo & xinfo)
{
  if (isBad_)
  {
    XDrawArc(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[0], 
                  (pos_.getX() - (dim_.getW())/2) * scale_, 
                  (pos_.getY() - (dim_.getH())/2) * scale_, 
                  dim_.getW() * scale_, 
                  dim_.getH() * scale_, 
                  ZERO, FULLDEGREE);
  }
  else 
  {
    int gc_num = rand() % 4;
 
    XFillArc(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[gc_num], 
                  (pos_.getX() - (dim_.getW()/2)) * scale_, 
                  (pos_.getY() - (dim_.getH()/2)) * scale_, 
                  dim_.getW() * scale_, 
                  dim_.getH() * scale_, 
                  ZERO, FULLDEGREE);
  }
}

void Projectile::update()
{
  //APPLY GRAVITY THING...YA 
  //vel_.setY(vel_.getY() * 1.101);

  Sprite::update();
}

bool Projectile::shouldDelete()
{
  if (pos_.getY() > 600 + 60)
  {
    return true;
  }
  if (pos_.getY() < -60)
  {
    return true;
  }

  return false;
}

bool Lazer::shouldDelete()
{
  if (pos_.getY() > 600 + 60)
  {
    return true;
  }
  return false;
}
