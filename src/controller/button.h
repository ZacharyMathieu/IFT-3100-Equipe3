#include <algorithm>

#include "ofMain.h"

#ifndef BUTTON
#define BUTTON

class Button
{
public:
    int x;
    int y;
    int w;
    int h;
    void(*callback)();

    Button(int x, int y, int w, int h, void(*callback)());
    void update();
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