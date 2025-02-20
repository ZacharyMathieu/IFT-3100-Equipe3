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
    ofColor *WALL_COLOR = new ofColor(0);
    ofColor *PHEROMONE_COLOR = new ofColor(0, 0, 255);
    ofColor *EMPTY_COLOR = new ofColor(255, 255, 255);
    const float MAX_VALUE = 256;
    const float DECAY_VALUE = 2;
    float value;

    CellType type;

    Cell(CellType = PHEROMONE, float = 0.0);

    void update();
    ofColor getCellColor();
    void addAntValue(float);

private:
    ofColor adjustColor(ofColor *fullColor, ofColor *emptyColor);
};

#endif