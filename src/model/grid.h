#include <tuple>
#include "cell.h"

#ifndef GRID
#define GRID

using namespace std;
class Grid
{
public:
    int w;
    int h;
    Cell **grid;

    Grid(int w, int h);
    Cell *at(int x, int y);
};

#endif