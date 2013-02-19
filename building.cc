#include <X11/Xlib.h>
#include "building.h"
#include <stdlib.h>
#include "particle.h"
#include "badguy.h"
#include <iostream>

using namespace std;

const int BWIDTH = 30;
const int BHEIGHT = 30;

//yee boiy help functuroos
void Building::constructLayer(vector<Sprite *> &drawingObjects, vector<Sprite *> &drawingBaddies, int xPos)
{
  float yPos = 400 - (BWIDTH);

  int amount = rand() % 6 + 2;
  for (int i = 0; i < amount; i++)
  {
    Building *temp = new Building(Pos(xPos, yPos));
    drawingObjects.push_back(temp);
    yPos -= 30;
  }

  int badGuyRand = rand() % 100 + 1; 
  if ((rand() % 100) <= 15) 
  {
    Badguy *badguy = new BadguyShooter(Pos(xPos + 15, yPos + 15), 
                    Vel(-2, 0), 
                    Dim(20, 20));

    drawingBaddies.push_back(badguy);
  }
  else if ((rand() % 100) >= 90) 
  {
    Badguy *badguy = new BadguyLazer(Pos(xPos, 0), 
                    Vel(-2, 0), 
                    Dim(30, 10));

    drawingBaddies.push_back(badguy); 
  }
  
  Building *temp = new Building(Pos(xPos, -90));
  drawingObjects.push_back(temp);
  
} 

Building::Building(Pos pos)
  :Sprite(pos)
{
  isBad_ = true;
}

void Building::draw(XInfo &xinfo)
{
  XDrawRectangle(xinfo.display, 
                  xinfo.win, 
                  xinfo.gc[0], 
                  pos_.getX() * scale_, 
                  pos_.getY() * scale_, 
                  dim_.getW() * scale_, 
                  dim_.getH() * scale_);
}

void Building::initBuildings(vector<Sprite *> &drawingObjects, vector<Sprite *> &drawingBaddies, unsigned int width)
{
  int xPos = 220 + (BWIDTH);
  int totalLayers = (width + 1000) / 30;
  for (int i = 0; i < totalLayers; i++)
  {
    Building::constructLayer(drawingObjects, 
                    drawingBaddies,
                    xPos); 

    xPos += 30;
  }
}

bool Building::shouldDelete()
{
  if (pos_.getX() < -120)
  {
    return true;
  } 
  return false;
}
