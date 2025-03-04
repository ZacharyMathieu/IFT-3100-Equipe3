#include "grid_controller.h"

//--------------------------------------------------------------
void GridController::setup(int x, int y, int w, int h)
{
    displayPosX = x;
    displayPosY = y;
    displayWidth = w;
    displayHeight = h;
    drawZonePressed = false;
    isSelected = false;;

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
            auto color = cell->getCellColor();
            ofSetColor(color);
            ofDrawRectangle(
                x * scaleX + displayPosX,
                y * scaleY + displayPosY,
                ceil(scaleX),
                ceil(scaleY));
            if (cell->isSelected)
            {
                ofNoFill();
                ofSetColor(255, 0, 0);
                ofSetLineWidth(7);
                ofDrawRectangle(
                    (int)x * scaleX + displayPosX,
                    (int)y * scaleY + displayPosY,
                    (int)scaleX,
                    (int)scaleY);
                ofFill();
            }
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
    
    if (drawZonePressed && !isSelected)
    {
        drawZone(mouse_pressed_x, mouse_pressed_y, mouse_current_x, mouse_current_y);

    }

}

//--------------------------------------------------------------
void GridController::exit()
{
}

void GridController::drawZone(float x1, float y1, float x2, float y2)
{
    float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth()/2);
    float y2_clamp = min(max(50.0f, y2), (float)ofGetHeight());

    ofSetLineWidth(20);
    ofSetColor(255,0,0,50);
    ofFill();
    ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
    

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
    mouse_current_x = x;
    mouse_current_y = y; 
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
            if(!tasCell.empty()) Undo.push({ "ERASE",tasCell });
        } 
        if (cursor == "SELECT")
        {
            if (!CSposition.empty()) {
                float dist = glm::distance(glm::vec2(x, y), posStart);
                if (dist > 15)
                {
                    int newGridX = (x - displayPosX) / scaleX;
                    int newGridY = (y - displayPosY) / scaleY;

                    int offsetX = newGridX - CSposition[0].first;
                    int offsetY = newGridY - CSposition[0].second;
                    vector<pair<int, int>> newPosition;

                    bool isValidMove = true;

                    
                    for (auto pos : CSposition)
                    {
                        int oldPosX = pos.first;
                        int oldPosY = pos.second;

                        int newPosX = oldPosX + offsetX;
                        int newPosY = oldPosY + offsetY;

                        if (newPosX < 0 || newPosX >= grid.w || newPosY < 0 || newPosY >= grid.h)
                        {
                            isValidMove = false;
                            break;  
                        }
                    }
                    
                    
                    if (isSelected) {
                        
                    
                        if (isValidMove)
                        {
                            vector<pair<int, int>> oldPositions;
                            vector<pair<int, int>> newPositions;

                            for (auto pos : CSposition)
                            {
                                int oldPosX = pos.first;
                                int oldPosY = pos.second;

                                int newPosX = oldPosX + offsetX;
                                int newPosY = oldPosY + offsetY;

                                if (newPosX < 0 || newPosX >= grid.w || newPosY < 0 || newPosY >= grid.h) {
                                    isValidMove = false;
                                    break;
                                }

                                if (grid.at(newPosX, newPosY)->type == WALL && !alreadySelected(newPosX, newPosY)) {
                                    isValidMove = false;
                                    break;
                                }

                                oldPositions.push_back({ oldPosX, oldPosY });
                                newPositions.push_back({ newPosX, newPosY });
                            }

                            if (isValidMove)
                            {
                                for (size_t i = 0; i < oldPositions.size(); i++)
                                {
                                    int oldPosX = oldPositions[i].first;
                                    int oldPosY = oldPositions[i].second;
                                    int newPosX = newPositions[i].first;
                                    int newPosY = newPositions[i].second;

                                    grid.at(oldPosX, oldPosY)->type = PHEROMONE;
                                    grid.at(oldPosX, oldPosY)->isSelected = false;
                                    grid.at(newPosX, newPosY)->type = WALL;
                                    grid.at(newPosX, newPosY)->isSelected = true;
                                }
                                CSposition = newPositions;
                            }
                            else
                            {
                                ofLog() << "Déplacement annulé : Une ou plusieurs cellules sont bloquées.";
                            }
                        }

                   }
                }
            }
        }

        }
   }


//--------------------------------------------------------------
void GridController::mousePressed(int x, int y, int button, string cursor)
{
    
    if (cursor == "SELECT" && y > 50) {
        isSelected = insideZoneSelected(x, y);
        drawZonePressed = true;
    }
    
    posStart = glm::vec2(x, y);
    if (y > 50)
    {
    float scaleX = ((float)displayWidth) / grid.w;
    float scaleY = ((float)displayHeight) / grid.h;

    int xOrigine = ((x) / scaleX) * scaleX;
    int yOrigine = ((y) / scaleY) * scaleY;
    int gridX = (xOrigine - displayPosX) / scaleX;
    int gridY = (yOrigine - displayPosY) / scaleY;
    if (cursor == "SELECT")
    {

        if (grid.at(gridX, gridY)->type == WALL) {
            grid.at(gridX, gridY)->isSelected = true;
            CSposition.push_back({ gridX,gridY });
        }
    }
    
    }
    
    
}

//--------------------------------------------------------------
void GridController::mouseReleased(int x, int y, int button)
{
    mouse_current_x = x;
    mouse_current_y = y;

    if (drawZonePressed) {
        drawZonePressed = false;
 
        processSelectionZone();
    }
    if (isSelected) {
        for (auto& pos : CSposition) {
            grid.at(pos.first, pos.second)->isSelected = false;
        }
        CSposition.clear();
    }
    isSelected = false;
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

void GridController::multipleSelection()
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

bool GridController::insideZoneSelected(int x, int y)
{
    float scaleX = ((float)displayWidth) / grid.w;
    float scaleY = ((float)displayHeight) / grid.h;
    int xOrigine = (x  / scaleX) * scaleX;
    int yOrigine = (y/ scaleY) * scaleY;
    int gridX = (xOrigine - displayPosX) / scaleX;
    int gridY = (yOrigine - displayPosY) / scaleY;

    Cell* cell = grid.at(gridX, gridY);
    return cell->isSelected;
 
}
void GridController::processSelectionZone()
{
    float scaleX = ((float)displayWidth) / grid.w;
    float scaleY = ((float)displayHeight) / grid.h;

    int minX = (int)((min(mouse_pressed_x, mouse_current_x) - displayPosX) / scaleX);
    int maxX = (int)((max(mouse_pressed_x, mouse_current_x) - displayPosX) / scaleX);
    int minY = (int)((min(mouse_pressed_y, mouse_current_y) - displayPosY) / scaleY);
    int maxY = (int)((max(mouse_pressed_y, mouse_current_y) - displayPosY) / scaleY);

    // Debugging pour vérifier les valeurs
    ofLog() << "Mouse Pressed: (" << mouse_pressed_x << ", " << mouse_pressed_y << ")";
    ofLog() << "Mouse Released: (" << mouse_current_x << ", " << mouse_current_y << ")";
    ofLog() << "Selection Grid Coordinates: (" << minX << "," << minY << ") to (" << maxX << "," << maxY << ")";

    for (int i = minX; i <= maxX; i++) {
        for (int j = minY; j <= maxY; j++) {
            if (i >= 0 && i < grid.w && j >= 0 && j < grid.h) {
                if (grid.at(i, j)->type == WALL) {
                    grid.at(i, j)->isSelected = true;
                    CSposition.push_back({ i, j });
                }
            }
        }
    }

    drawZoneCreated = false; // Désactive la zone après sélection
}

bool GridController::alreadySelected(int x, int y)
{
    for (auto pos : CSposition) {
        if (pos.first == x && pos.second == y) return true;
    }
    return false;
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

void GridController::importGrid()
{
    ofImage img;
    ofFileDialogResult filename = ofSystemLoadDialog("Importer une image");
    if (!img.load(filename.getPath())) {
        ofLog() << "Erreur : Impossible de charger l'image " << filename;
        return;
    }

    // Vérifie si l'image correspond aux dimensions de la grille
    if (img.getWidth() != displayWidth || img.getHeight() != displayHeight) {
        ofLog() << "Erreur : Dimensions de l'image incorrectes";
        return;
    }

    float scaleX = ((float)displayWidth) / grid.w;
    float scaleY = ((float)displayHeight) / grid.h;

    // Parcours chaque cellule et récupère la couleur correspondante
    for (int y = 0; y < grid.h; y++) {
        for (int x = 0; x < grid.w; x++) {
            int px = x * scaleX;
            int py = y * scaleY;
            ofColor color = img.getColor(px, py);

            // Définir les cellules en fonction des couleurs détectées
            if (color == ofColor(0, 0, 0)) {
                grid.at(x, y)->type = WALL;  // Noir = Mur
            }
            else {
                grid.at(x, y)->type = PHEROMONE;  // Autre couleur = espace libre
            }
        }
    }

    ofLog() << "Grille importée depuis PNG : " << filename;
}
