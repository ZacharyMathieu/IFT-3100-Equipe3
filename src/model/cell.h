#include "ofMain.h"

#ifndef CELL
#define CELL

using namespace std;

constexpr float CELL_MAX_VALUE = 256;
constexpr float CELL_DECAY_VALUE = 1;
constexpr float CELL_DECAY_FACTOR = 1;

ofColor* getWallColor()
{
	return new ofColor(0, 0, 0);
}

ofColor* getPheromoneColor()
{
	return new ofColor(0, 0, 255);
}

ofColor* getFoodColor()
{
	return new ofColor(117, 255, 107);
}

ofColor* getEmptyColor()
{
	return new ofColor(255, 255, 255);
}

enum CellType
{
	WALL,
	PHEROMONE,
	FOOD
};

class Cell
{
public:
	ofColor* WALL_COLOR = getWallColor();
	ofColor* PHEROMONE_COLOR = getPheromoneColor();
	ofColor FOOD_COLOR = ofColor(117, 255, 107);
	ofColor* EMPTY_COLOR = getEmptyColor();
	float value;
	CellType type;
	Cell(CellType = PHEROMONE, float = 0.0);

	void update();
	ofColor getCellColor(int alpha = 255);
	float getValueFactor();
	void setFoodColor(ofColor newColor);
	void addAntValue(float);
	ofColor getFoodColor();
	bool isSelected;
private:
	ofColor adjustColor(ofColor* fullColor, ofColor* emptyColor);
};

#endif