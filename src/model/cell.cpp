#include "cell.h"

Cell::Cell(CellType type, float value)
{
	this->type = type;
	this->value = value;
	isSelected = false;
}

ofColor& Cell::getCellColor(int alpha)
{
	ofColor c;
	switch (type)
	{
	case WALL:
		c = *WALL_COLOR;
		break;
	case PHEROMONE:
		c = adjustColor(PHEROMONE_COLOR, EMPTY_COLOR);
		break;
	case FOOD:
		c = FOOD_COLOR;
		break;
	default:
		c = ofColor(255, 0, 0);
		break;
	}

	c.a = alpha;

	return c;
}

float Cell::getValueFactor() {
	return value / CELL_MAX_VALUE;
}

void Cell::setFoodColor(ofColor newColor)
{
	FOOD_COLOR = newColor;
}

ofColor Cell::adjustColor(ofColor* fullColor, ofColor* emptyColor)
{
	float r = value / CELL_MAX_VALUE;
	float rInverse = 1 - r;
	return (fullColor->operator*(r)) + (emptyColor->operator*(rInverse));
}

void Cell::update()
{
	if (value >= CELL_DECAY_VALUE)
	{
		value -= CELL_DECAY_VALUE;
	}
	value *= CELL_DECAY_FACTOR;
}

void Cell::addAntValue(float antValue)
{
	value += antValue;
	value = max(min(value, CELL_MAX_VALUE), 0.0f);
}
ofColor Cell::getFoodColor()
{
	return Cell::FOOD_COLOR;
}