#include "mainchar.h"
#include "main.h"
#include "missle.h"
#include <vector>
#include <iostream>
#include "particle.h"
#include "stdlib.h"

using namespace std;
        
const int ZERO = 0;
const int FULLDEGREE = 23040;

Mainchar::Mainchar(Pos pos, Vel vel, Dim dim)
	:Sprite(pos, vel, dim)
{}

void Mainchar::draw(XInfo &xinfo)
{
	XDrawArc(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[0], 
                  (pos_.getX() - (dim_.getW()/2)) * scale_, 
                  (pos_.getY() - (dim_.getH()/2)) * scale_, 
                  dim_.getW() * scale_, 
                  dim_.getH() * scale_, 
                  ZERO, FULLDEGREE);

  XFillArc(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[0], 
                  pos_.getX() * scale_, 
                  (pos_.getY() - ((dim_.getH())/4)) * scale_, 
                  5.0f * scale_, 
                  5.0f * scale_, 
                  ZERO, FULLDEGREE);

  XFillArc(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[0], 
                  (pos_.getX() - ((dim_.getW())/2)) * scale_, 
                  (pos_.getY() - ((dim_.getH())/2) - 5) * scale_, 
                  dim_.getW() * scale_, 
                  (dim_.getH()/4) * scale_, 
                  ZERO, FULLDEGREE);
  
  XFillRectangle(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[0], 
                  (pos_.getX() - 8) * scale_, 
                  (pos_.getY() - (dim_.getH() * 1.2)) * scale_, 
                  (dim_.getW() / 1.5f) * scale_, 
                  (dim_.getH() / 1.3f) * scale_); 
}

void Mainchar::move(int x, int y)
{
    vel_.setX(x);
    vel_.setY(y); 
}

void Mainchar::shoot(std::vector<Sprite *> &drawingMissiles)
{
  Missile *m = new Missile(Pos(pos_.getX(), pos_.getY()),
                    Vel(0, 2), 
                    Dim(8, 8),
                    false);

  drawingMissiles.push_back(m);
}

void Mainchar::spawnParticles(vector<Sprite *> &drawingParticles)
{
  int toSpawn = rand() % 15 + 5; 
  //int toSpawn = rand() % 15 + 5; 

  for (int i = 0; i < toSpawn; i++)
  {
    float velX = -2 + (float)(rand() % 69 + 1) / 100.0f;
    float velY = (float)(rand() % 69 + 1) / 100.0f;

    float posX = pos_.getX();  
    float posY = pos_.getY() + (int)(dim_.getH() / 3);  

    Particle *p;

    if ((rand() % 10 + 1) < 4) 
      p = new ParticleSquare(Pos(posX, posY), Vel(velX, velY)); 
    else
      p = new ParticleCircle(Pos(posX, posY), Vel(velX, velY)); 

    drawingParticles.push_back(p);    
  }

}
