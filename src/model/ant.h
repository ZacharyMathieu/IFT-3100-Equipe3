#include "ofMain.h"
#include "grid.h"

#ifndef ANT
#define ANT

using namespace std;

class Ant
{
public:
    ofColor COLOR = ofColor(255, 0, 0, 255);
    const float MOVE_SPEED = 0.25;
    const float TURN_SPEED = 0.25;
    const float SEARCH_RADIUS = 25;
    const float MAX_PHEROMONE_LEVEL = 10;
    ofPoint pos;
    float a;
    float pheromoneLevel;

    Ant(float x, float y, float a);
    void update(Grid *grid);
    void turnTorwards(ofPoint);
};

#endif