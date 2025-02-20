#include "cell.h"

Cell::Cell(CellType type, float value)
{
    this->type = type;
    this->value = value;
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

ofColor Cell::adjustColor(ofColor *fullColor, ofColor *emptyColor)
{
    float r = value / MAX_VALUE;
    float rInverse = 1 - r;
    return (fullColor->operator*(r)) + (emptyColor->operator*(rInverse));
}

void Cell::update()
{
    if (value >= DECAY_VALUE)
    {
        value -= DECAY_VALUE;
    }
}

void Cell::addAntValue(float antValue)
{
    value += antValue;
    value = min(value, MAX_VALUE);
    // value = max(value, (float)0.0);
}