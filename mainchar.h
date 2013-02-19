#ifndef MAINCHAR
#define MAINCHAR 

#include "main.h"
#include "sprite.h"

class Mainchar : public Sprite
{
public:
	Mainchar(Pos, Vel, Dim);
	void draw(XInfo &);
	void move(int, int);
  virtual void shoot(std::vector<Sprite *> &);
  virtual void spawnParticles(std::vector<Sprite *> &drawingParticles);
};

#endif
