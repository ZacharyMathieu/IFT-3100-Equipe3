#include "ofMain.h"
#include "grid.h"

#ifndef ANT
#define ANT

using namespace std;

constexpr float ANT_MOVE_SPEED = 0.2;
constexpr float ANT_SEARCH_RADIUS = 10;
constexpr float ANT_TURN_SPEED = ANT_MOVE_SPEED / ANT_SEARCH_RADIUS * TWO_PI;
constexpr float ANT_MAX_PHEROMONE_LEVEL = 64;
constexpr float ANT_NORMAL_PHEROMONE_LEVEL = 0.5;
constexpr float ANT_PHEROMONE_LEVEL_DECAY_VALUE = 0.5;
constexpr float ANT_RANDOM_TURN_MAX = ANT_TURN_SPEED;
constexpr float ANT_FOOD_HUNGER_VALUE = 100;

class Ant
{
public:
	ofColor COLOR = ofColor(255, 0, 0, 255);
	ofColor MAIN_ANT_COLOR = ofColor(255, 0, 255, 255);
	ofPoint pos;
	float a;
	float pheromoneLevel;
	int hunger = 0;

	Ant(float x, float y, float a);
	void update(Grid* grid);
private:
	void turnToDiff(float);
	float angleTo(ofPoint);
	float angleDiff(float);
};

#endif