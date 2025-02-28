#include "ofMain.h"
#include "model/grid.h"
#include "model/ant.h"


#ifndef GRID_CONTROLLER
#define GRID_CONTROLLER

class GridController
{
public:
    static const int GRID_WIDTH = 100;
    static const int GRID_HEIGHT = 100;
    int displayPosX;
    int displayPosY;
    int displayWidth;
    int displayHeight;
    Grid grid = Grid(GRID_WIDTH, GRID_HEIGHT);
    vector<Ant *> ants;
    std::stack<std::pair<std::string,vector<Cell*>>> Undo;
    std::stack<std::pair<std::string, vector<Cell*>>> Redo;
    ofImage gridImage;
    ofShader shader;

    void setup(int x, int y, int w, int h);
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button, string cursor, int drawSize, int eraserSize);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void undo();
    void redo();

    void update();
};

#endif