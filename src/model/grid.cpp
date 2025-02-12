#include "grid.h"

Grid::Grid(int w, int h)
{
    this->w = w;
    this->h = h;
    // this->grid = (Cell **)malloc(sizeof(Cell) * w * h);
    for (int y = 0; y < h; y++)
    {
        vector<Cell *> line = {};
        for (int x = 0; x < w; x++)
        {
            // cout << "assigning cell!\n";
            // this->grid[y][x] = Cell();
            line.push_back(new Cell());
        }
        grid.push_back(line);
    }
}

Cell *Grid::at(int x, int y)
{
    return grid[y][x];
}
