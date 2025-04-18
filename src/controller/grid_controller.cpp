#include "grid_controller.h"

//--------------------------------------------------------------
void GridController::setup(int x, int y, int w, int h)
{
	displayPosX = x;
	displayPosY = y;
	displayWidth = w;
	displayHeight = h;
	drawZonePressed = false;
	isSelected = false;
	scaleX = ((float)displayWidth) / grid.w;
	scaleY = ((float)displayHeight) / grid.h;
	for (int i = 0; i < ANT_COUNT; i++)
	{
		ants.push_back(new Ant(rand() % grid.w, rand() % grid.h, 0));
	}
}

//--------------------------------------------------------------
void GridController::draw(Ant* mainAnt)
{
	float scaleX = ((float)displayWidth) / grid.w;
	float scaleY = ((float)displayHeight) / grid.h;

	int y = 0;
	for (vector<Cell*> line : grid.grid)
	{
		int x = 0;
		for (Cell* cell : line)
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

	for (Ant* ant : ants)
	{
		if (ant != mainAnt)
		{
			ofSetColor(ant->COLOR);
			ofDrawCircle(ofPoint(ant->pos.x * scaleX + displayPosX, ant->pos.y * scaleY + displayPosY), 2);
		}
	}

	ofSetColor(mainAnt->MAIN_ANT_COLOR);
	ofDrawCircle(ofPoint(mainAnt->pos.x * scaleX + displayPosX, mainAnt->pos.y * scaleY + displayPosY), 8);

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
	ofSetColor(255, 0, 0);
	ofSetLineWidth(10);
	ofDrawLine(displayWidth, 50, displayWidth, displayHeight + 50);
	ofDrawLine(0, 50, displayWidth * 2, 50);
}

//--------------------------------------------------------------
void GridController::exit()
{
}

void GridController::drawZone(float x1, float y1, float x2, float y2)
{
	float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth() / 2);
	float y2_clamp = min(max(50.0f, y2), (float)ofGetHeight());

	ofSetLineWidth(20);
	ofSetColor(255, 0, 0, 50);
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

void GridController::drawOnGrid(int x, int y, CellType material, int drawSize)
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
					if (grid.at(gridX, gridY)->type != material)
					{
						grid.at(gridX, gridY)->type = material;
						tasCell.push_back(grid.at(gridX, gridY));
						while (!Redo.empty())
						{
							Redo.pop();
						}
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void GridController::mouseDragged(int x, int y, int button, string cursor, CellType material, int drawSize, int eraserSize)
{
	mouse_current_x = x;
	mouse_current_y = y;
	if (y > displayPosY)
	{
		float scaleX = ((float)displayWidth) / grid.w;
		float scaleY = ((float)displayHeight) / grid.h;
		
		// changer pheromone en mur
		if (cursor == "DRAW")
		{
			drawOnGrid(x, y, material, drawSize);
		}
		else if (cursor == "ERASE" ) // changer mur en pheromone
		{
			if ((x - eraserSize) >= displayPosX &&
				(y - eraserSize) >= displayPosY &&
				(x + eraserSize) < displayWidth &&
				(y + eraserSize) < displayHeight + displayPosY)
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
				
			}
			
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
								ofLog() << "D�placement annul� : Une ou plusieurs cellules sont bloqu�es.";
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

		int gridX = (x - displayPosX) / scaleX;
		int gridY = (y - displayPosY) / scaleY;
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

	vector<Cell*> cells;
	if (!tasCell.empty()) {
		cells = tasCell;
		if (cells[0]->type == WALL) {
			Undo.push({ "DRAW", cells });
		}
		else {
			Undo.push({ "ERASE", cells });
		}
	}
	tasCell.clear();
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
	auto action = Undo.top();
	Undo.pop();
	Redo.push(action);
	ofLog() << "undo action : " << Redo.top().first;
	if (action.first == "DRAW")
	{
		for (auto c : action.second)
		{
			c->type = PHEROMONE;
		}
		
	}
	else
	{

		for (auto c : action.second)
		{
			c->type = WALL;
		}
	}
		
}

void GridController::redo()
{
	auto action = Redo.top();
	Redo.pop();
	Undo.push(action);
	
	if (action.first == "DRAW")
	{
		
		for (auto c : action.second)
		{
			c->type = WALL;
			
		}
		
	}
	else
	{
		for (auto c : action.second)
		{
			c->type = PHEROMONE;
		}
		
	}
}

bool GridController::insideZoneSelected(int x, int y)
{
	float scaleX = ((float)displayWidth) / grid.w;
	float scaleY = ((float)displayHeight) / grid.h;
	int xOrigine = (x / scaleX) * scaleX;
	int yOrigine = (y / scaleY) * scaleY;
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


	for (int i = minX; i <= maxX; i++)
	{
		for (int j = minY; j <= maxY; j++)
		{
			if (i >= 0 && i < grid.w && j >= 0 && j < grid.h) {
				if (grid.at(i, j)->type == WALL) {
					grid.at(i, j)->isSelected = true;
					CSposition.push_back({ i, j });
				}
			}
		}
	}

	drawZoneCreated = false; // D�sactive la zone apr�s s�lection
}

bool GridController::alreadySelected(int x, int y)
{
	for (auto pos : CSposition)
	{
		if (pos.first == x && pos.second == y) return true;
	}
	return false;
}

void GridController::update()
{
	grid.update();
	for (Ant* ant : ants)
	{
		ant->update(&grid);
	}
	for (Ant* ant : ants)
	{
		grid.at(ant->pos)->addAntValue(ant->pheromoneLevel);
	}
}

void GridController::importGrid(ofFile file)
{
	ofBuffer buffer = file.readToBuffer();
	ants.clear();
	bool readingAnts = false;

	for (auto line : buffer.getLines())
	{
		if (line == "ant") {
			readingAnts = true;
			continue;
		}
		vector<string> tokens = ofSplitString(line, ",");
		if (!readingAnts)
		{

			if (tokens.size() < 4) continue;

			int x = ofToInt(tokens[0]);
			int y = ofToInt(tokens[1]);
			float v = ofToFloat(tokens[2]);
			string type = ofToString(tokens[3]);
			CellType c = type == "WALL" ? WALL : PHEROMONE;

			if (x >= 0 && x < grid.w && y >= 0 && y < grid.h)
			{
				grid.at(x, y)->type = c;
				grid.at(x, y)->value = v;
			}
		}
		else
		{
			if (tokens.size() < 2) continue;

			float x = ofToFloat(tokens[0]);
			float y = ofToFloat(tokens[1]);

			ants.push_back(new Ant(x, y, 0));
		}
	}
}



