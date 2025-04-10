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
	ofPlanePrimitive rightWall, leftWall, backWall, ceiling,floor, poster, cadrePlane;
	ofImage posterImg;
	ofTexture posterTex;
	ofxPanel gui;
	ofParameter<bool> posterChoice;
	ofParameter<bool> blueTint;
	ofParameter<bool> redTint;
	ofParameter<bool> greenTint;
	ofParameter<bool> doubleTint;
	ofMaterial* mat;

	ofColor antColor;
	ofColor normalColor;
	ofColor metallicColor;
	ofColor roughnessColor;

	ofImage redEye;
	ofImage blueEye;
	ofImage greenEye;
	ofImage doubleColorEye;
	ofImage cadre;

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
	void onBlueChanged(bool& value);
	void onRedChanged(bool& value);
	void onGreenChanged(bool& value);
	void onDoubleChanged(bool& value);
	
};

