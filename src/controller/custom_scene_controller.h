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
	bool colorChanged;

public:
	ofxAssimpModelLoader ant;
	ofPlanePrimitive rightWall, leftWall, backWall, ceiling,floor, poster;
	ofImage posterImg;
	ofTexture posterTex;
	ofxPanel gui;
	ofParameter<bool> posterChoice;
	ofParameter<bool> defaultAnt;
	ofParameter<ofColor> antTint;

	ofColor antColor;

	ofShader shader;
	ofTexture antTexture;
	ofTexture imgTexture;
	ofImage img;
	ofMaterial antMaterial;
	void setup() override ;
	void update() override;
	void draw() override;
	void onColorChanged(ofColor& color);
	void onDefaultSelect(bool& value);
};

