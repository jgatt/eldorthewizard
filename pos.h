#ifndef POS
#define POS

class Pos
{
public:
  Pos(float, float);
	float getX();
	float getY();
  void setY(float);
  void setX(float);
  void offset(float, float);
private:
    float x_;
    float y_;  
};

#endif
