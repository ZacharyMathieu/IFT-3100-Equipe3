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
			if (cell->type == FOOD)
				cell->setFoodColor(foodColor);
			auto color = cell->getCellColor();
			ofSetColor(color);
			ofDrawRectangle(
				x * scaleX + displayPosX,
				y * scaleY + displayPosY,
				ceil(scaleX),
				ceil(scaleY));
			if (cell->isSelected)
			{
				ofFill();
				glm::vec4 color1 = glm::vec4(color.r, color.g, color.b, color.a) / 255.0f;
				glm::vec4 color2 = glm::vec4(255, 0, 0, 150) / 255.0f;

				glm::vec4 mixedColor = glm::mix(color1, color2, 0.35f);
				ofColor colorMix = ofColor(mixedColor.r * 255, mixedColor.g * 255, mixedColor.b * 255, mixedColor.a * 255);

				ofSetColor(colorMix);
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
	if (isDraggingSelection) {
		for (auto pos : CSposition) {
			int newX = pos.first + offsetDragX;
			int newY = pos.second + offsetDragY;
			if (newX >= 0 && newX < grid.w && newY >= 0 && newY < grid.h) {
				ofFill();
				ofSetColor(30, 255, 255,175);
				ofSetLineWidth(2);
				ofDrawRectangle(
					newX * scaleX + displayPosX,
					newY * scaleY + displayPosY,
					scaleX,
					scaleY);
				ofFill();
			}
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
						newType.first = grid.at(gridX, gridY)->type;
						newType.second = material;
						grid.at(gridX, gridY)->type = material;
						tasCell.push_back({ grid.at(gridX, gridY), newType });
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
								newType.first = grid.at(gridX, gridY)->type;
								newType.second = PHEROMONE;
								grid.at(gridX, gridY)->type = PHEROMONE;
								tasCell.push_back({ grid.at(gridX, gridY), newType });
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
		if (cursor == "SELECT" && !CSposition.empty()) {
			float dist = glm::distance(glm::vec2(x, y), posStart);
			if (dist > 10) {
				isDraggingSelection = true;

				int newGridX = (x - displayPosX) / scaleX;
				int newGridY = (y - displayPosY) / scaleY;

				offsetDragX = newGridX - CSposition[0].first;
				offsetDragY = newGridY - CSposition[0].second;

				int minX = CSposition[0].first;
				int maxX = CSposition[0].first;
				int minY = CSposition[0].second;
				int maxY = CSposition[0].second;

				for (auto& pos : CSposition) {
					minX = std::min(minX, pos.first);
					maxX = std::max(maxX, pos.first);
					minY = std::min(minY, pos.second);
					maxY = std::max(maxY, pos.second);
				}

				// Clamp sur X
				if (minX + offsetDragX < 0)
					offsetDragX = -minX;
				if (maxX + offsetDragX >= grid.w)
					offsetDragX = grid.w - 1 - maxX;

				// Clamp sur Y
				if (minY + offsetDragY < 0)
					offsetDragY = -minY;
				if (maxY + offsetDragY >= grid.h)
					offsetDragY = grid.h - 1 - maxY;
			}
		}
	}
}

//--------------------------------------------------------------
void GridController::mousePressed(int x, int y, int button, string cursor)
{
	mouse_pressed_x = x;
	mouse_pressed_y = y;
	mouse_current_x = x;
	mouse_current_y = y;

	float scaleX = ((float)displayWidth) / grid.w;
	float scaleY = ((float)displayHeight) / grid.h;

	int gridX = (x - displayPosX) / scaleX;
	int gridY = (y - displayPosY) / scaleY;

	if (cursor == "SELECT" && y > 50)
	{
		if (gridX >= 0 && gridX < grid.w && gridY >= 0 && gridY < grid.h)
		{
			Cell* clickedCell = grid.at(gridX, gridY);
			if (clickedCell->isSelected)
			{
				// Clique sur la sélection actuelle  préparer à déplacer
				isDraggingSelection = true;
				offsetDragX = 0;
				offsetDragY = 0;
			}
			else
			{
				// Clique ailleurs annuler ancienne sélection
				for (auto& pos : CSposition) {
					grid.at(pos.first, pos.second)->isSelected = false;
				}
				CSposition.clear();
				isSelected = false;

				// Démarrer une nouvelle sélection
				drawZonePressed = true;
			}
		}
	}

}

//--------------------------------------------------------------
void GridController::mouseReleased(int x, int y, int button, string action, string cursor)
{
	mouse_current_x = x;
	mouse_current_y = y;

	if (drawZonePressed) {
		drawZonePressed = false;
		processSelectionZone();
	}

	bool movedSomething = false; // ajouter ce flag !

	if (cursor == "SELECT" && isDraggingSelection)
	{
		vector<pair<Cell*, pair<CellType, CellType>>> selectAction;
		vector<pair<int, int>> newCSPosition;

		for (auto pos : CSposition) {
			int oldX = pos.first;
			int oldY = pos.second;
			int newX = oldX + offsetDragX;
			int newY = oldY + offsetDragY;

			if (newX < 0 || newX >= grid.w || newY < 0 || newY >= grid.h)
				continue;
			if (grid.at(newX, newY)->type != PHEROMONE)
				continue;

			Cell* src = grid.at(oldX, oldY);
			Cell* dst = grid.at(newX, newY);

			selectAction.push_back({ src, { src->type, PHEROMONE } });
			selectAction.push_back({ dst, { dst->type, src->type } });

			dst->type = src->type;
			dst->isSelected = false;  //  IMPORTANT : ne plus rester sélectionné
			src->type = PHEROMONE;
			src->isSelected = false;

			newCSPosition.push_back({ newX, newY });
		}

		offsetDragX = offsetDragY = 0;
		isDraggingSelection = false;

		CSposition.clear();  //  Vider la liste
		isSelected = false;  //  Plus rien de sélectionné

		if (!selectAction.empty()) {
			Undo.push(selectAction);
			while (!Redo.empty()) Redo.pop();
			movedSomething = true;
		}
	}

	// Si on n'a pas fait de déplacement, et qu'on avait une sélection visuelle
	if (!movedSomething && isSelected) {
		for (auto& pos : CSposition) {
			grid.at(pos.first, pos.second)->isSelected = false;
		}
		CSposition.clear();
		isSelected = false;
	}

	// Pour le mode DRAW / ERASE
	if (!tasCell.empty()) {
		Undo.push(tasCell);
		tasCell.clear();
	}
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
	if (Undo.empty()) return;

	auto action = Undo.top();
	Undo.pop();
	Redo.push(action);

	for (auto element : action) {
		element.first->type = element.second.first;
	}
		
}

void GridController::redo()
{
	if (Redo.empty()) return;

	auto action = Redo.top();
	Redo.pop();
	Undo.push(action);

	for (auto element : action) {
		element.first->type = element.second.second;
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
				CellType type = grid.at(i, j)->type;
				if (type == WALL || type == FOOD) {
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
			CellType c;
			if (type == "WALL") c = WALL;
			if (type == "PHEROMONE") c = PHEROMONE;
			if (type == "FOOD") c = FOOD;

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



