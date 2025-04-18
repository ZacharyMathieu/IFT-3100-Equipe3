#include "grid.h"

Grid::Grid(int w, int h)
{
    this->w = w;
    this->h = h;
    for (int y = 0; y < h; y++)
    {
        vector<Cell *> line = {};
        for (int x = 0; x < w; x++)
        {
            line.push_back(new Cell());
        }
        grid.push_back(line);
    }
}

Cell *Grid::at(int x, int y)
{
    return grid[y][x];
}

Cell *Grid::at(ofPoint pos)
{
    return grid[(int)pos.y][(int)pos.x];
}

void Grid::update()
{
    for (auto line : grid)
    {
        for (Cell *cell : line)
        {
            cell->update();
        }
    }
}
