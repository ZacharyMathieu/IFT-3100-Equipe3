#include "ofMain.h"

#ifndef CELL
#define CELL

using namespace std;

constexpr float CELL_MAX_VALUE = 256;
constexpr float CELL_DECAY_VALUE = 1;
constexpr float CELL_DECAY_FACTOR = 1;

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
    float value;
    CellType type;
    Cell(CellType = PHEROMONE, float = 0.0);

    void update();
    ofColor getCellColor();
    void addAntValue(float);
    bool isSelected;
private:
    ofColor adjustColor(ofColor *fullColor, ofColor *emptyColor);
};

#endif