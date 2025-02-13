#pragma once

#include "ofMain.h"
#include "grid_controller.h"
#include "button.h"
#include "ofxGui.h"

#ifndef APPLICATION
#define APPLICATION

enum CursorMode
{
	DEFAULT,
	DRAW,
	ERASE
};

class Application : public ofBaseApp
{
public:
	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 768;
	static const int MENU_HEIGHT = 50;
	static const int MENU_BUTTON_MARGIN = 10;
	static const int MENU_BUTTON_WIDTH = MENU_HEIGHT;

	static const int COLLAPSED_MENU_WIDTH = 30;
	static const int COLLAPSED_MENU_HEIGHT = 30;
	static const int OPENED_MENU_WIDTH = 220;
	static const int OPENED_MENU_HEIGHT_ERASE = 50;
	static const int OPENED_MENU_HEIGHT_DRAW = 160;
	static const int OPENED_MENU_HEIGHT_COLOR = 160;
	static const int WHEEL_RADIUS = 40;

	static const int COORD_MENU_X = 10;
	static const int COORD_MENU_Y = 60;
	static const int COORD_SLIDER_X = 20;

	static const int SLIDER_WIDTH = 200;

	bool isRunning = false;

	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
	void mouseEntered(int x, int y) override;
	void mouseExited(int x, int y) override;
	void windowResized(int w, int h) override;
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;
	void drawMenu(bool &showMenu, bool &isCollapsed, const string &title, int &sliderValue, int menuHeight);

private:
	CursorMode cursorMode = DEFAULT;
	GridController gridController;
	ofImage importedImage;
	bool imageLoaded = false;

	bool showEraserMenu = false;
	bool showDrawMenu = false;
	bool showColorMenu = false;

	bool isEraserMenuCollapsed = false;
	bool isDrawMenuCollapsed = false;
	bool isColorMenuCollapsed = false;
	bool menuHidden = false;
	
	// Taille du crayon et de la gomme
	ofParameter<int> eraserSize;
	ofParameter<int> drawCursorSize;

	// Couleur du crayon
	ofParameter<ofColor> colorPicker;

	// Interfaces graphiques (GUI)
	ofxPanel eraserGui; // GUI pour la gomme (taille uniquement)
	ofxPanel penGui;    // GUI pour le crayon (taille + couleur)
	ofxPanel colorGui;  // GUI pour la roue de couleur seule

	ofColor currentDrawColor = ofColor(255, 0, 0);

	ofImage importImageIcon;
	ofImage exportImageIcon;
	ofImage playIcon;
	ofImage pauseIcon;
	ofImage fastForwardIcon;
	ofImage eraseModeIcon;
	ofImage drawModeIcon;
	ofImage shapeModeIcon;
	ofImage penTypeChoiceIcon;
	ofImage shapeChoiceIcon;

	Button importImageButton;
	Button exportImageButton;
	Button playButton;
	Button pauseButton;
	Button fastForwardButton;
	Button eraseModeButton;
	Button drawModeButton;
	Button shapeModeButton;
	Button penTypeChoiceButton;
	Button shapeChoiceButton;
	vector<Button *> buttons;

	void setupButtons();
	void drawMenu();
	void drawCustomCursors();
	void onColorChanged(ofColor &color);
	void onDrawCursorSizeChanged(int &size);
	void onEraserSizeChanged(int &size);
	void importImage();
	void exportImage();
	void play();
	void fastForward();
	void eraseMode();
	void drawMode();
	void shapeMode();
	void penTypeChoice();
	void shapeChoice();
};

#endif