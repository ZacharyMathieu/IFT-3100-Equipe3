#include "grid.h"

Grid::Grid(int w, int h)
{
    this->w = w;
    this->h = h;
    this->grid = (Cell **)malloc(sizeof(Cell) * w * h);
}

Cell *Grid::at(int x, int y)
{
    return &grid[y][x];
}