#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include <ofxAssimpModelLoader.h>
//pour le filtre d'image
enum class ConvolutionKernel
{
	identity,
	emboss,
	sharpen,
	edge_detect,
	blur
};

// kernel de convolution (3x3) : identité
const std::array<float, 9> convolution_kernel_identity
{
  0.0f,  0.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  0.0f,  0.0f
};

// kernel de convolution (3x3) : aiguiser
const std::array<float, 9> convolution_kernel_sharpen
{
  0.0f, -1.0f,  0.0f,
 -1.0f,  5.0f, -1.0f,
  0.0f, -1.0f,  0.0f
};

// kernel de convolution (3x3) : détection de bordure
const std::array<float, 9> convolution_kernel_edge_detect
{
  0.0f,  1.0f,  0.0f,
  1.0f, -4.0f,  1.0f,
  0.0f,  1.0f,  0.0f
};

// kernel de convolution (3x3) : bosseler
const std::array<float, 9> convolution_kernel_emboss
{
 -2.0f, -1.0f,  0.0f,
 -1.0f,  1.0f,  1.0f,
  0.0f,  1.0f,  2.0f
};

// kernel de convolution (3x3) : flou
const std::array<float, 9> convolution_kernel_blur
{
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f
};

class CustomSceneController : public ofBaseApp
{
private:
	ConvolutionKernel ck;
	ofEasyCam cam;
	ofLight light;
	ofCylinderPrimitive plateform;

	
	void openPosterChoicer();
	bool posterSet;
	bool colorChanged;
	float newAngle = 115.0f;
	int turnSpeed = 3;
	bool filterActivated;

	uint64_t lastClickTime = 0;
	const int doubleClickDelay = 200;

	glm::vec3 startPos, endPos;
	glm::vec3 leftPos, rightPos, firstPos, posterPos;
	glm::vec3 startTarget, endTarget;
	glm::vec3 leftTarget, rightTarget, firstTarget, posterTarget;

	float transitionStartTime;
	float transitionDuration = 2.0f; // secondes
	bool isTransitioning = false;

	ofRectangle resetButton, rightButton, leftButton;


public:
	int image_height, image_width;
	ofxAssimpModelLoader ant;
	ofxAssimpModelLoader redAnt;
	ofxAssimpModelLoader greenAnt;
	vector<ofxAssimpModelLoader*> ants;
	ofxAssimpModelLoader* activeAnt;

	ofPlanePrimitive rightWall, leftWall, backWall, ceiling,floor, poster, cadrePlane;
	ofImage posterImg;
	ofImage posterFilter;
	ofTexture posterTex;
	ofxPanel gui;
	ofParameter<bool> posterChoice;
	ofParameter<bool> blueTint;
	ofParameter<bool> redTint;
	ofParameter<bool> greenTint;
	ofParameter<bool> doubleTint;
	ofParameter<bool> identite;
	ofParameter<bool> aiguiser;
	ofParameter<bool> border;
	ofParameter<bool> bosseler;
	ofParameter<bool> flou;
	
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
	void mousePressed(int x, int y, int button) override;
	void startCameraTransition(glm::vec3 newPos, glm::vec3 newTarget);
	void resetCamera();
	void onBlueChanged(bool& value);
	void onRedChanged(bool& value);
	void onGreenChanged(bool& value);
	void onDoubleChanged(bool& value);
	void onIdentityChanged(bool& value);
	void onSharpChanged(bool& value);
	void onBorderChanged(bool& value);
	void onEmbossChanged(bool& value);
	void onBlurChanged(bool& value);

	void filter(ofImage& imgSrc);
	
};



