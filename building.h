#ifndef BUILDING 
#define BUILDING 

#include "main.h"
#include "sprite.h" 
#include <vector>

class Building : public Sprite
{
public:
  Building(Pos); 
  void draw(XInfo &xinfo);
  static void initBuildings(std::vector<Sprite *> &, 
                  std::vector<Sprite *> &,
                  unsigned int);
  static void constructLayer(std::vector<Sprite *> &, 
                  std::vector<Sprite *> &, 
                  int); 

  virtual bool shouldDelete();
};

#endif 
