#include "grid_controller.h"

//--------------------------------------------------------------
void GridController::setup(int x, int y, int w, int h)
{
    displayPosX = x;
    displayPosY = y;
    displayWidth = w;
    displayHeight = h;

    gridImage.allocate(grid.h, grid.w, OF_IMAGE_COLOR);

    // TODO: Better init of ants
    for (int i = 0; i < 200; i++)
    {
        ants.push_back(new Ant(rand() % grid.w, rand() % grid.h, 0));
    }
}

//--------------------------------------------------------------
void GridController::draw()
{
    
    float scaleX = ((float)displayWidth) / grid.w;
    float scaleY = ((float)displayHeight) / grid.h;

    int y = 0;
    for (vector<Cell *> line : grid.grid)
    {
        int x = 0;
        for (Cell *cell : line)
        {
            if (cell->type == WALL)
            {
                
            }
            auto color = cell->getCellColor();
            ofSetColor(color);
            ofDrawRectangle(
                x * scaleX + displayPosX,
                y * scaleY + displayPosY,
                ceil(scaleX),
                ceil(scaleY));
            x++;
        }
        y++;
    }

    for (Ant *ant : ants)
    {
        ofSetColor(ant->COLOR);
        ofDrawCircle(ofPoint(ant->pos.x * scaleX + displayPosX, ant->pos.y * scaleY + displayPosY), 2);
    }

    ofSetColor(0, 0, 0, 150);
    ofSetLineWidth(2);

    for (int x = 0; x < grid.w; x++)
    {
        ofDrawLine(x * scaleX + displayPosX, displayPosY, x * scaleX, displayPosY + displayHeight);
    }
    for (int y = 0; y < grid.h; y++)
    {
        ofDrawLine(displayPosX, y * scaleY + displayPosY, displayPosX + displayWidth, y * scaleY + displayPosY);
    }
    
}

//--------------------------------------------------------------
void GridController::exit()
{
}

//--------------------------------------------------------------
void GridController::keyPressed(int key)
{
    if (key == 'z')
    {
        if (!Undo.empty())
            undo();
    }
    if (key == 'y')
    {
        if (!Redo.empty())
            redo();
    }
}

//--------------------------------------------------------------
void GridController::keyReleased(int key)
{
}

//--------------------------------------------------------------
void GridController::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void GridController::mouseDragged(int x, int y, int button, string cursor, int drawSize, int eraserSize)
{
    if (y > displayPosY)
    {
        float scaleX = ((float)displayWidth) / grid.w;
        float scaleY = ((float)displayHeight) / grid.h;
        vector<Cell *> tasCell;

        // changer pheromone en mur
        if (cursor == "DRAW")
        {
            if ((x - drawSize) >= displayPosX &&
                (y - drawSize) >= displayPosY &&
                (x + drawSize) < displayWidth + displayPosX &&
                (y + drawSize) < displayHeight + displayPosY)
            {
                int xOrigine = ((x - drawSize) / scaleX) * scaleX;
                int yOrigine = ((y - drawSize) / scaleY) * scaleY;

                // int nbIteX = (xOrigine + (drawSize * 2) / scaleX) * scaleX;
                // int nbIteY = (yOrigine + (drawSize * 2) / scaleY) * scaleY;

                for (int i = xOrigine; i < xOrigine + (drawSize * 2); i += scaleX)
                {
                    for (int j = yOrigine; j < yOrigine + (drawSize * 2); j += scaleY)
                    {
                        int gridX = (i - displayPosX) / scaleX;
                        int gridY = (j - displayPosY) / scaleY;
                        if (grid.at(gridX, gridY))
                        {
                            if (grid.at(gridX, gridY)->type != WALL)
                            {
                                grid.at(gridX, gridY)->type = WALL;
                                tasCell.push_back(grid.at(gridX, gridY));
                                while (!Redo.empty())
                                {
                                    Redo.pop();
                                }
                            }
                        }
                    }
                }
                if (!tasCell.empty())
                    Undo.push({"DRAW", tasCell});
            }
        }
        else if (cursor == "ERASE") // changer mur en pheromone
        {
            if ((x - eraserSize) >= displayPosX &&
                (y - eraserSize) >= displayPosY &&
                (x + eraserSize - scaleX) < displayWidth &&
                (y + eraserSize - scaleY) < displayHeight)
            {
                int xOrigine = ((x - eraserSize) / scaleX) * scaleX;
                int yOrigine = ((y - eraserSize) / scaleY) * scaleY;

                for (int i = xOrigine; i < xOrigine + (eraserSize * 2); i += scaleX)
                {
                    for (int j = yOrigine; j < yOrigine + (eraserSize * 2); j += scaleY)
                    {
                        int gridX = (i - displayPosX) / scaleX;
                        int gridY = (j - displayPosY) / scaleY;
                        if (grid.at(gridX, gridY))
                        {
                            if (grid.at(gridX, gridY)->type != PHEROMONE)
                            {
                                grid.at(gridX, gridY)->type = PHEROMONE;
                                tasCell.push_back(grid.at(gridX, gridY));
                                while (!Redo.empty())
                                {
                                    Redo.pop();
                                }
                            }
                        }
                    }
                }
                if (!tasCell.empty())
                    Undo.push({"ERASE", tasCell});
            }
        }
    }
}

//--------------------------------------------------------------
void GridController::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void GridController::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void GridController::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
}

//--------------------------------------------------------------
void GridController::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void GridController::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void GridController::windowResized(int w, int h)
{
}

void GridController::undo()
{

    if (Undo.top().first == "DRAW")
    {
        Redo.push(Undo.top());
        for (auto c : Undo.top().second)
        {
            c->type = PHEROMONE;
        }
        Undo.pop();
    }
    else
    {
        Redo.push(Undo.top());

        for (auto c : Undo.top().second)
        {
            c->type = WALL;
        }
        Undo.pop();
    }
}

void GridController::redo()
{

    if (Redo.top().first == "DRAW")
    {
        Undo.push(Redo.top());
        for (auto c : Redo.top().second)
        {
            c->type = WALL;
        }
        Redo.pop();
    }
    else
    {
        Undo.push(Redo.top());

        for (auto c : Redo.top().second)
        {
            c->type = PHEROMONE;
        }
        Redo.pop();
    }
}

void GridController::update()
{
    grid.update();
    for (Ant *ant : ants)
    {
        ant->update(&grid);
    }
    for (Ant *ant : ants)
    {
        grid.at(ant->pos)->addAntValue(ant->pheromoneLevel);
    }
}
