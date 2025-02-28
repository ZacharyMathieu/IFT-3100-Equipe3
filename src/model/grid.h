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
    vector<vector<Cell *>> grid;

    Grid(int w, int h);
    Cell *at(int, int);
    Cell *at(ofPoint);
    void update();
};

#endif