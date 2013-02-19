#ifndef DIM
#define DIM

class Dim
{
public:
  Dim(float, float);
  float getW();
	float getH();
  void setW(float);
  void setH(float);

private:
  float w_;
  float h_;

};

#endif
