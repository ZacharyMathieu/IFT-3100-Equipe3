#pragma once

#include "ofMain.h"
#include "grid.h"
#include "button.h"

#ifndef APPLICATION
#define APPLICATION

enum CursorMode { DEFAULT, DRAW, ERASE };

class Application : public ofBaseApp
{
public:
	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;
	const int MENU_HEIGHT = 50;
	const int MENU_BUTTON_MARGIN = 10;
	const int MENU_BUTTON_WIDTH = MENU_HEIGHT;

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

private:
	CursorMode cursorMode = DEFAULT;

	Grid grid;

	ofImage importedImage;
    bool imageLoaded = false;

	bool showEraserMenu = false;
	bool showDrawMenu = false;

	int eraserSize = 20;
	int drawCursorSize = 15;

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