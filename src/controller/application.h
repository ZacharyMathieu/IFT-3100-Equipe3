#pragma once

#include "ofMain.h"
#include "grid.h"
#include "button.h"

#ifndef APPLICATION
#define APPLICATION

class Application : public ofBaseApp
{
public:
	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;
	const int MENU_HEIGHT = 100;
	const int MENU_BUTTON_WIDTH = MENU_HEIGHT;

	vector<Button> buttons;
	Grid grid;
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
	void drawMenu();
	void callback();
	void callback2();
};

#endif
