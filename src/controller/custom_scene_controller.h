#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include <ofxAssimpModelLoader.h>

class CustomSceneController : public ofBaseApp
{
private:
	ofEasyCam cam;
	ofLight light;

	void openPosterChoicer();
	bool posterSet;

public:
	ofxAssimpModelLoader ant;
	ofPlanePrimitive rightWall, leftWall, backWall, ceiling,floor, poster;
	ofImage posterImg;
	ofTexture posterTex;
	ofxPanel gui;
	ofParameter<bool> posterChoice;
	void setup() override ;
	void update() override;
	void draw() override;
};

