#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/time.h>
#include <string>
#include <fstream>
#include "main.h"
#include "building.h"
#include "pos.h"
#include "mainchar.h"
#include "missle.h"

using namespace std;

const int Border = 5;
const int BufferSize = 10;
const int FPS = 60;
const int STARTWINDOWWIDTH = 600;
const int STARTWINDOWHEIGHT = 400; 
float scale = 1.0f;

Mainchar *mainchar = NULL;
vector<Sprite *> drawingObjects;
vector<Sprite *> drawingMissiles;
vector<Sprite *> drawingBaddies;
vector<Sprite *> drawingParticles;

bool inPause = true;
bool resetFlag = true;
bool inHighScore = false;
bool inWizardMode = true;
bool inViewHighScores = false;

string name = "";

unsigned long resetTimer = 0;

float score = 0.0f;

unsigned int width = 600;
unsigned int height = 400;

unsigned int oldHeight = 400;

float baseGameSpeed = 0.25f;
float upperGameSpeed = 3.75f;
int high_scores[10];
int cur_high_pos = -1;
string high_score_names[10];
Atom wmDeleteMessage; 

template <class T>
void deleteInVector(vector<T*> &deleteme) 
{
    while(!deleteme.empty()) {
        delete deleteme.back();
        deleteme.pop_back();
    }
    //delete deleteme;
}

void gameOver()
{ 
  deleteInVector(drawingBaddies);
  deleteInVector(drawingObjects);
  deleteInVector(drawingParticles);
  deleteInVector(drawingMissiles);

  mainchar->setX(20); 
  mainchar->setY(100); 

  for (int i = 0 ; i < 10; i++)
  {
    if (score > high_scores[i])
    {
      cur_high_pos = i;
      break;
    }
  }
  if (cur_high_pos != -1)
    inHighScore = true;
  else
    score = 0;

  resetFlag = true; 
  inViewHighScores = true;
  Sprite::gamespeed_ = 1.0f;
}

void quit(XInfo &xinfo)
{
  //XORG screws up my computer
  gameOver(); 
  XAutoRepeatOn(xinfo.display);

  //write to files
  ofstream highscores;
  highscores.open("highscores.txt");
  for (int i = 0; i < 10; i++)
  {
    highscores << high_scores[i] << " " << high_score_names[i] << endl;
  } 
  highscores.close();

  delete mainchar;
  cerr << "Terminating normally." << endl;
  exit(0);
}

string convertIntToStr(int number)
{
   stringstream ss;
   
   ss << number;
   return ss.str();
}

unsigned long now() 
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void initX(int argc, char *argv[], XInfo &xinfo)
{
	XSizeHints hints;
  unsigned long background, foreground;
  int screen;	
  mainchar = new Mainchar(Pos(20, 100), Vel(0, 0), Dim(25, 25));

	xinfo.display = XOpenDisplay( "" );
  if ( !xinfo.display )
  { 
		cerr << "Can't open display." << endl;
  }

	screen = DefaultScreen( xinfo.display );
  background = WhitePixel( xinfo.display, screen );
  foreground = BlackPixel( xinfo.display, screen );

	hints.x = 100;
  hints.y = 100;
	hints.flags = PPosition | PSize;
  hints.width = width;
  hints.height = height;	

  xinfo.win = XCreateSimpleWindow( xinfo.display, 
                  DefaultRootWindow( xinfo.display ), hints.x, hints.y, 
                  hints.width, hints.height, Border, foreground, background );

  wmDeleteMessage = XInternAtom(xinfo.display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(xinfo.display, xinfo.win, &wmDeleteMessage, 1);

  XSetStandardProperties(
		xinfo.display,		// display containing the window
		xinfo.win,		// window whose properties are set
		"Eldor the Wizard's Colourful Adventure",		// window's title
		"Eldor the Wizard's Colourful Adventure",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	xinfo.gc[0] = XCreateGC (xinfo.display, xinfo.win, 0, 0 );
	xinfo.gc[1] = XCreateGC (xinfo.display, xinfo.win, 0, 0 );
	xinfo.gc[2] = XCreateGC (xinfo.display, xinfo.win, 0, 0 );
	xinfo.gc[3] = XCreateGC (xinfo.display, xinfo.win, 0, 0 );
	xinfo.gc[4] = XCreateGC (xinfo.display, xinfo.win, 0, 0 );
  
  XSetBackground( xinfo.display, xinfo.gc[0], background );
  XSetForeground( xinfo.display, xinfo.gc[0], foreground );

  XSetBackground( xinfo.display, xinfo.gc[1], background );
  XSetForeground( xinfo.display, xinfo.gc[1], 0x0000ff );

  XSetBackground( xinfo.display, xinfo.gc[2], background );
  XSetForeground( xinfo.display, xinfo.gc[2], 0xff0000 );

  XSetBackground( xinfo.display, xinfo.gc[3], background );
  XSetForeground( xinfo.display, xinfo.gc[3], 0x00ff00 );


	XSelectInput( xinfo.display, xinfo.win,
                ButtonPressMask | KeyPressMask | ExposureMask 
                | KeyReleaseMask 
                );

  XAutoRepeatOff(xinfo.display);
  XMapRaised( xinfo.display, xinfo.win );
}

void drawString(XInfo &xinfo, string s, int x, int y)
{
  XDrawString(xinfo.display,
                  xinfo.win,
                  xinfo.gc[0],
                  x*Sprite::scale_, y*Sprite::scale_, 
                  s.c_str(),
                  s.length());
}

void displayHighScores(XInfo &xinfo)
{
  drawString(xinfo, "You Died!", 100, 50);  
  drawString(xinfo, "High Scores: ", 100, 100); 
  for (int i = 0; i < 10; i++)
  {
    string s =  convertIntToStr(i) + ". " + convertIntToStr(high_scores[i]) + " " + high_score_names[i];
    drawString(xinfo, s, 100, 125 + i*25); 
  }
}

void draw(XInfo &xinfo)
{
	XClearWindow(xinfo.display, xinfo.win);
	//XFillRectangle(xinfo.display, xinfo.win, xinfo.gc, 0, 0, 600, 400);

  if (inHighScore || inViewHighScores)
  {

    displayHighScores(xinfo);
    
    if (inHighScore) 
    {
      drawString(xinfo, "Enter Your Name (press enter to finish):", 300, 50);
      drawString(xinfo, "Name: " + name, 300, 100);
    }
    if (inViewHighScores && !inHighScore)
    {
      //do something
      drawString(xinfo, "Press h to start again.", 300, 50);
      drawString(xinfo, "Press f to return to the main menu.", 300, 75);
      drawString(xinfo, "Press q to quit.", 300, 100); 
    } 
    
    return;
  }

  if (inPause)
  {
    string s[8] = {"Jason Gatt Presents: ", "Eldor The Wizard's Colourful Adventure!", 
            "Instructions: Press 'f' to start the game, and to pause it.",
            "Use w(up), a(left), s(down), and d(right) to move.  Use space to shoot bombs.",
            "Press q to quit.",
            "Move through the tunnel and try not to hit any of the buildings",
            "Make sure to avoid all of the missiles as well!"};

    for (int i = 0; i < 7; i++)
    {
      drawString(xinfo, s[i], 100, 100 + i*25); 
    }

    return;
  }

  for (int i = 0; i < drawingMissiles.size(); i++)
  {
    drawingMissiles[i]->draw(xinfo);
  }
  for (int i = 0; i < drawingParticles.size(); i++)
  {
    drawingParticles[i]->draw(xinfo);
  }
  for (int i = 0; i < drawingObjects.size(); i++)
  {
    drawingObjects[i]->draw(xinfo);
  }
  for (int i = 0; i < drawingBaddies.size(); i++)
  {
    drawingBaddies[i]->draw(xinfo);
  }
	mainchar->draw(xinfo);
  
  string scoreText = "Score: " + convertIntToStr((int)score); 
  XDrawString(xinfo.display,
                  xinfo.win,
                  xinfo.gc[0],
                  440, 25,
                  scoreText.c_str(),
                  scoreText.length());

  XFlush(xinfo.display);
}

void deleteFromDrawing(vector<Sprite *> &s, int i)
{
  Sprite *temp = s.at(i);
  s.erase(s.begin() + i); 
  delete temp;  
}

void update(XInfo &xinfo)
{
  if (inHighScore || inViewHighScores)
    return;

  if (inPause)
    return;

  if (resetFlag)
  {
    Building::initBuildings(drawingObjects, drawingBaddies, width); 
    resetFlag = false;
  }

  XWindowAttributes windowInfo;
  XGetWindowAttributes(xinfo.display, xinfo.win, &windowInfo);

  height = windowInfo.height;
  width = windowInfo.width;

  if (height != oldHeight)
  {
    float multiplyer = (float)height / (float)oldHeight;
    scale * multiplyer;
    Sprite::scale_ = Sprite::scale_ * multiplyer;

    oldHeight = height;

    //fixUpScaling();
  }
  

  //CHECK IF NEW BUILDINGS NEED TO BE MADE
  if (drawingObjects.back()->getX() < (width + ((90))))
  {
    Building::constructLayer(drawingObjects,
                    drawingBaddies, 
                    width + (120));
  }

  mainchar->update();
  if (inWizardMode)
  {
    mainchar->spawnParticles(drawingParticles);        
  }

  for (int i = 0; i < drawingObjects.size(); i++)
  {
    drawingObjects[i]->update();
    if (Sprite::checkForCollide(*mainchar, *drawingObjects[i]))
    {
      cout << "You Done Goofed!" << endl;
      gameOver(); 
      return;
    }
    if (drawingObjects[i]->shouldDelete())
    {
      deleteFromDrawing(drawingObjects, i);  
    }
  }
  for (int i = 0; i < drawingBaddies.size(); i++)
  {
    drawingBaddies[i]->update();
    if (Sprite::checkForCollide(*mainchar, *drawingBaddies[i]))
    {
      cout << "You Done Goofed! Baddy" << endl;
      gameOver(); 
      return;
    }
    if (drawingBaddies[i]->getX() < width)
      drawingBaddies[i]->shoot(drawingMissiles);
    if (drawingBaddies[i]->shouldDelete())
    {
      deleteFromDrawing(drawingBaddies, i);
    }
  }
  for (int i = 0; i < drawingMissiles.size(); i++)
  {
    int toDeleteBlock = -1; int toDeleteBaddy = -1;

    drawingMissiles[i]->update();
    if (Sprite::checkForCollide(*mainchar, *drawingMissiles[i]))
    {
      cout << "You Done Goofed! Missile" << endl;
      gameOver(); 
      return;
    }
    for (int j = 0; j < drawingObjects.size(); j++)
    {
      if (Sprite::checkForCollide(*drawingMissiles[i], *drawingObjects[j])
                      && toDeleteBlock == -1)
      {
        toDeleteBlock = j;
        break;
      }  
    }

    for (int j = 0; j < drawingBaddies.size(); j++)
    {
      if (Sprite::checkForCollide(*drawingMissiles[i], *drawingBaddies[j])
                      && toDeleteBaddy == -1)
      {
        toDeleteBaddy = j;
        break;
      }  
    }
    
    if (drawingMissiles[i]->shouldDelete())
    {
      deleteFromDrawing(drawingMissiles, i);
    }

    if (toDeleteBlock >= 0)
    {
      deleteFromDrawing(drawingMissiles, i);
      drawingObjects[toDeleteBlock]->spawnParticles(drawingParticles);
      deleteFromDrawing(drawingObjects, toDeleteBlock);
    } 

    if (toDeleteBaddy >= 0)
    {
      deleteFromDrawing(drawingMissiles, i);
      drawingBaddies[toDeleteBaddy]->spawnParticles(drawingParticles);
      deleteFromDrawing(drawingBaddies, toDeleteBaddy);
      score += 500;
    }
  }
  
  for (int i = 0; i < drawingParticles.size(); i++)
  {
    drawingParticles[i]->update();
    if (drawingParticles[i]->shouldDelete())
    {
      deleteFromDrawing(drawingParticles, i);
    }
  }
  
  score += 0.4 * Sprite::gamespeed_;
  //Approx one 'speed level' per 30s
  Sprite::gamespeed_ += 0.00014; 
} 

void handleKeyRelease(XInfo &xinfo, XEvent &event)
{
  KeySym key;
	char text[BufferSize];
	
   int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)

  if ( i == 1 ) 
  {
    switch (text[0])
    {
     case 'w':
        mainchar->setDy(0);
        break;
      case 'd':
        mainchar->setDx(0);
        break;
      case 'a':
        mainchar->setDx(0);
        break;
      case 's':
        mainchar->setDy(0);
        break;
    }
  }
}

void handleKeyPress(XInfo &xinfo, XEvent &event)
{
  KeySym key;
	char text[BufferSize];
	
   int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)

  if (inHighScore)
  {
    //Handle Naming
    if (XLookupKeysym(&event.xkey, 0) == XK_Return)
    {
      int i = 9;
      while (i > cur_high_pos)
      {
        high_scores[i] = high_scores[i-1]; 
        high_score_names[i] = high_score_names[i-1]; 
        i--;
      }

      high_score_names[cur_high_pos] = name; 
      high_scores[cur_high_pos] = score;

      inHighScore = false;
      name = "";
      cur_high_pos = -1;
      score = 0;
      return;
    }
    if (XLookupKeysym(&event.xkey, 0) == XK_BackSpace)
    {
      name = name.substr(0, name.size() - 1);  
      return;
    }

    if (i == 1)
      name += text[0]; 
    return;
  }

  if (XLookupKeysym(&event.xkey, 0) == XK_space)  
  {
    mainchar->shoot(drawingMissiles); 
  } 
  else if ( i == 1 ) 
  {
    switch (text[0])
    {
      case 'q':
        quit(xinfo);
        break;
      case 'f':
        if (inPause)
          inPause = false;
        else
          inPause = true;
        inViewHighScores = false;
        break; 
      case 'h':
        if (inViewHighScores)
          inViewHighScores = false;
        else
          inViewHighScores = true;
        break;
      case 'm':
        if (inWizardMode)
          inWizardMode = false;
        else
          inWizardMode = true;
        break;
      case 'w':
        mainchar->setDy(-5 + (Sprite::gamespeed_ - 1));
        break;
      case 'd':
        mainchar->setDx(5 - (Sprite::gamespeed_ - 1));
        break;
      case 'a':
        mainchar->setDx(-5 + (Sprite::gamespeed_ - 1));
        break;
      case 's':
        mainchar->setDy(5 - (Sprite::gamespeed_ - 1));
        break;
      case 'u':
        if (Sprite::gamespeed_ < upperGameSpeed)
          Sprite::gamespeed_ += 0.25f;
          if (Sprite::gamespeed_ > upperGameSpeed)
            Sprite::gamespeed_ = upperGameSpeed;
        break;
      case 'j':
        //Don't Allow the Game to stop...
        if (Sprite::gamespeed_ > baseGameSpeed)
          Sprite::gamespeed_ -= 0.25f;
          if (Sprite::gamespeed_ < baseGameSpeed)
            Sprite::gamespeed_ = baseGameSpeed;
        break;
    }
  }
}

void handleResize(XInfo &xinfo, XEvent &event) 
{
  XResizeRequestEvent *rre = (XResizeRequestEvent *)&event;
  xinfo.width = rre->width;
	xinfo.height = rre->height;
}

void eventloop(XInfo &xinfo)
{
	XEvent event;
	KeySym key;
	char text[BufferSize];
	unsigned long lastRepaint = 0;

	while (true)
	{
    if (XPending(xinfo.display) > 0) 
    { 
	    XNextEvent( xinfo.display, &event);
		  switch (event.type)
		  {
			  case KeyPress:
          handleKeyPress(xinfo, event);
          break;
        case KeyRelease:
          handleKeyRelease(xinfo, event);
          break;
        case ClientMessage:
          if (event.xclient.data.l[0] == wmDeleteMessage)
          {
            quit(xinfo);  
          } 
          break;
      }
		}

		unsigned long end = now();
		if (end - lastRepaint > 1000000/FPS) 
		{
      update(xinfo);
      draw(xinfo);
			lastRepaint = now();
		}

		if (XPending(xinfo.display) == 0) 
		{
			usleep(1000000/FPS - (end - lastRepaint));
		}		
	}
}

int main (int argc, char *argv[])
{
	XInfo xinfo;
	initX(argc, argv, xinfo);

  ifstream infile("highscores.txt");  

  int i = 0;
  int s; string n;

  while (infile >> s >> n)
  {
    high_scores[i] = s; 
    high_score_names[i] = n;
    i++;
  } 

  infile.close();
	eventloop(xinfo);	
	return 0;
}

