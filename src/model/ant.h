#include "ofMain.h"
#include "grid.h"

#ifndef ANT
#define ANT

using namespace std;

constexpr float ANT_MOVE_SPEED = 0.1;
constexpr float ANT_SEARCH_RADIUS = 5;
constexpr float ANT_TURN_SPEED = ANT_MOVE_SPEED / ANT_SEARCH_RADIUS * PI / 2;
constexpr float ANT_MAX_PHEROMONE_LEVEL = 40;
constexpr float ANT_NORMAL_PHEROMONE_LEVEL = 1;
constexpr float ANT_PHEROMONE_LEVEL_DECAY_VALUE = 0.1;

class Ant
{
public:
	ofColor COLOR = ofColor(255, 0, 0, 255);
	ofColor MAIN_ANT_COLOR = ofColor(255, 0, 255, 255);
	ofPoint pos;
	float a;
	float pheromoneLevel;

	Ant(float x, float y, float a);
	void update(Grid* grid);
private:
	//ofPoint futurePos;

	void turnToDiff(float);
	float angleTo(ofPoint);
	float angleDiff(float);
};

#endif