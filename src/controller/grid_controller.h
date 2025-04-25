#include "ofMain.h"
#include "model/grid.h"
#include "model/ant.h"


#ifndef GRID_CONTROLLER
#define GRID_CONTROLLER

class GridController
{
public:
	static const int GRID_WIDTH = 50;
	static const int GRID_HEIGHT = 50;
	static const int ANT_COUNT = 200;

	float scaleX;
	float scaleY;
	int displayPosX;
	int displayPosY;
	int displayWidth;
	int displayHeight;
	Grid grid = Grid(GRID_WIDTH, GRID_HEIGHT);
	vector<Ant*> ants;

	//variable util au Undo/Redo
	std::stack<std::vector<std::pair<Cell*, std::pair<CellType, CellType>>>> Undo;
	std::stack<std::vector<std::pair<Cell*, std::pair<CellType, CellType>>>> Redo;
	vector<std::pair<Cell*, std::pair<CellType, CellType>>> tasCell;
	std::pair<CellType, CellType> newType;

	ofColor foodColor;

	std::vector<Cell*> cSelected;
	std::vector<std::pair<int, int>> CSposition;
	glm::vec2 posStart;
	int mouse_current_x;
	int mouse_current_y;
	int mouse_pressed_x;
	int mouse_pressed_y;
	bool drawZonePressed;
	bool drawZoneCreated;
	bool isSelected;
	ofRectangle zoneSelected;

	//variable pour selection multiple
	vector<pair<Cell*, CellType>> movedCellsBefore;
	vector<pair<Cell*, CellType>> movedCellsAfter;
	vector<pair<Cell*, pair<CellType, CellType>>> selectActionTemp;

	bool isDraggingSelection = false;
	int offsetDragX = 0;
	int offsetDragY = 0;
	
	void setup(int x, int y, int w, int h);
	void draw(Ant*);
	void exit();

	void drawZone(float x1, float y1, float x2, float y2);
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button, string cursor, CellType material, int drawSize, int eraserSize);
	void mousePressed(int x, int y, int button, string cursor);
	void mouseReleased(int x, int y, int button,string action, string cursor);
	void mouseScrolled(int x, int y, float scrollX, float scrollY);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void multipleSelection();
	void undo();
	void redo();
	bool insideZoneSelected(int x, int y);
	void processSelectionZone();
	bool alreadySelected(int x, int y);
	void update();
	void importGrid(ofFile file);
	void drawOnGrid(int x, int y, CellType material, int drawSize);
};

#endif