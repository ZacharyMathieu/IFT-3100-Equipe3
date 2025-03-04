#include "ofMain.h"
#include "model/grid.h"
#include "model/ant.h"
#include <scene_controller.h>



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
    SceneController SceneController;
    std::vector<Cell*> cSelected;
    std::vector<std::pair<int, int>> CSposition;
    glm::vec2 posStart;
    int mouse_current_x;
    int mouse_current_y;
    int mouse_pressed_x;
    int mouse_pressed_y;
    bool drawZonePressed;
    bool drawZoneCreated;
    bool isSelected;
    ofRectangle zoneSelected;
    void setup(int x, int y, int w, int h);
    void draw();
    void exit();

    void drawZone(float x1, float y1, float x2, float y2);
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button, string cursor, int drawSize, int eraserSize);
    void mousePressed(int x, int y, int button, string cursor);
    void mouseReleased(int x, int y, int button);
    void mouseScrolled(int x, int y, float scrollX, float scrollY);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void multipleSelection();
    void undo();
    void redo();
    bool insideZoneSelected(int x, int y);
    void processSelectionZone();
    bool alreadySelected(int x, int y);
    void update();
    void importGrid();
};

#endif