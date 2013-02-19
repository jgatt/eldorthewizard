#ifndef GAME_MAIN 
#define GAME_MAIN 

#include <X11/Xlib.h>
#include <string>
#include "pos.h"

struct XInfo;

void initX(int argc, char *argv[], XInfo &xinfo);

void eventloop(XInfo &xinfo);

void draw();

unsigned long now();

void gameOver();

void quit(XInfo &);

std::string convertIntToStr(int);

struct XInfo
{
	Display *display;
	int screen;
  Window win;
  GC gc[4];
	Pixmap	pixmap;
  
  int width;
  int height;
};

#endif
