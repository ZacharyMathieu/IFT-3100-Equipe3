#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include <ofxAssimpModelLoader.h>

class CustomSceneController : public ofBaseApp
{
private:
	ofEasyCam cam;
	ofLight light;
	ofCylinderPrimitive plateform;

	void openPosterChoicer();
	bool posterSet;
	bool colorChanged;
	float newAngle = 115.0f;
	int turnSpeed = 3;


public:
	ofxAssimpModelLoader ant;
	ofPlanePrimitive rightWall, leftWall, backWall, ceiling,floor, poster;
	ofImage posterImg;
	ofTexture posterTex;
	ofxPanel gui;
	ofParameter<bool> posterChoice;
	ofParameter<bool> defaultAnt;
	ofParameter<ofColor> baseTint;
	ofParameter<ofColor> normalTint;
	ofParameter<ofColor> metallicTint;
	ofParameter<ofColor> roughnessTint;
	ofMaterial* mat;

	ofColor antColor;
	ofColor normalColor;
	ofColor metallicColor;
	ofColor roughnessColor;

	ofTexture baseColorTexture;
	ofTexture normalMapTexture;
	ofTexture metallicTexture;
	ofTexture roughnessTexture;
	ofTexture aoTexture;

	ofShader shader;
	ofTexture antTexture;
	ofTexture imgTexture;
	ofImage img;

	ofImage imgPlateform;
	ofTexture texPlateform;
	void setup() ;
	void update() ;
	void draw();
	void onColorChanged(ofColor& color);
	void onNormalColorChanged(ofColor& color);
	void onMetallicColorChanged(ofColor& color);
	void onRoughnessColorChanged(ofColor& color);
	void onDefaultSelect(bool& value);
};

