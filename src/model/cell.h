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
    CellType type;
    float value;

    ofColor getCellColor();
};

#endif