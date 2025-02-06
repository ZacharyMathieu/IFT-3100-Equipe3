#include "ofMain.h"

#ifndef GRID
#define GRID

class Grid
{
public:
    int displayPosX;
    int displayPosY;
    int displayWidth;
    int displayHeight;

    void setup(int x, int y, int w, int h);
    void draw();
	void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
};

#endif