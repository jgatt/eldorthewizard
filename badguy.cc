#include "badguy.h"
#include "sprite.h"
#include <vector>
#include <stdlib.h>
#include <iostream>

const unsigned long SHOOTTIME = 90;
const unsigned long SHOOTTIMELAZER = 180;

Badguy::Badguy(Pos pos, Vel vel, Dim dim)
  :Sprite(pos, vel, dim)
{
  isBad_ = true;
}

BadguyShooter::BadguyShooter(Pos pos, Vel vel, Dim dim)
  :Badguy(pos, vel, dim)
{
  shootTime = rand() % SHOOTTIME; 
}

BadguyLazer::BadguyLazer(Pos pos, Vel vel, Dim dim)
  :Badguy(pos, vel, dim)
{
  shootTime = rand() % SHOOTTIMELAZER; 
}

bool Badguy::shouldDelete()
{
  if (pos_.getX() < -120)
  {
    return true;
  } 
  return false;
}

void BadguyShooter::draw(XInfo &xinfo)
{
  XDrawArc(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[0], 
                  (pos_.getX() - ((dim_.getW())/2)) * scale_, 
                  (pos_.getY() - ((dim_.getH())/2)) * scale_, 
                  dim_.getW() * scale_, 
                  dim_.getH() * scale_, 
                  0, 23040);
}

void BadguyLazer::draw(XInfo &xinfo)
{
  XFillRectangle(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[0], 
                  pos_.getX() * scale_, 
                  pos_.getY() * scale_, 
                  dim_.getW() * scale_, 
                  dim_.getH() * scale_);
}

void BadguyShooter::shoot(std::vector<Sprite *> &drawingMissiles)
{
  shootTime += 1;

  if (shootTime > (SHOOTTIME / gamespeed_))
  { 
    Missile *m = new Missile(Pos(pos_.getX(), pos_.getY()),
                    Vel(-2, -2), 
                    Dim(4, 4),
                    true);

    drawingMissiles.push_back(m);
    shootTime = 0;
  }
}

void BadguyLazer::shoot(std::vector<Sprite *> &drawingMissiles)
{
  shootTime += 1;

  if (shootTime > SHOOTTIMELAZER / gamespeed_)
  { 
    Lazer *l = new Lazer(Pos(pos_.getX() + 9, pos_.getY() - 380),
                    Vel(-2, 20), 
                    Dim(15, 400),
                    true);

    drawingMissiles.push_back(l);
    shootTime = 0;
  }
}
