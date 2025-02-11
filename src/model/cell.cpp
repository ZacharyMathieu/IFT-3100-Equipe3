#include "cell.h"

ofColor Cell::getCellColor()
{
    return ofColor(rand() % 255, rand() % 255, rand() % 255);
}