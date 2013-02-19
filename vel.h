#ifndef VEL 
#define VEL 

class Vel
{
public:
  Vel(float, float); 
  float getX();
	float getY();
  void setY(float);
  void setX(float);

private:
  float dx_;
  float dy_;
};

#endif
