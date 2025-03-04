#include "ofMain.h"

#ifndef CELL
#define CELL

using namespace std;

enum CellType
{
    WALL,
    PHEROMONE
};

class Cell
{
public:
    const ofColor WALL_COLOR = ofColor(0);
    const ofColor PHEROMONE_COLOR = ofColor(0, 0, 255);
    static const int MAX_VALUE = 255;
    static const int CELL_HEIGHT = 300;
    CellType type;
    bool isSelected;
    Cell(CellType = PHEROMONE, float = 0.0);

    ofColor getCellColor();

private:
    float value;

    ofColor adjustColor(ofColor color);
};

#endif