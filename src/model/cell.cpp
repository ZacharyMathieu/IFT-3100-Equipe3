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
        return WALL_COLOR;
    case PHEROMONE:
        return adjustColor(PHEROMONE_COLOR);
    default:
        return ofColor(255, 0, 0);
    }
}

ofColor Cell::adjustColor(ofColor color)
{
    color.a *= value / MAX_VALUE;
    return color;
}