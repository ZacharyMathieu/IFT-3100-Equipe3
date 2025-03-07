#include "cell.h"

Cell::Cell(CellType type, float value)
{
	this->type = type;
	this->value = value;
	isSelected = false;
}

ofColor Cell::getCellColor()
{
	switch (type)
	{
	case WALL:
		return *WALL_COLOR;
	case PHEROMONE:
		return adjustColor(PHEROMONE_COLOR, EMPTY_COLOR);
	default:
		return ofColor(255, 0, 0);
	}
}

float Cell::getValueFactor() {
	return value / CELL_MAX_VALUE;
}

ofColor Cell::adjustColor(ofColor* fullColor, ofColor* emptyColor)
{
	float r = value / CELL_MAX_VALUE;
	float rInverse = 1 - value / CELL_MAX_VALUE;
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