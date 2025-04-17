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
struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};

class CustomSceneController : public ofBaseApp
{
private:
	ConvolutionKernel ck;
	ofEasyCam cam;
	ofLight light;
	ofCylinderPrimitive plateform;

	
	
	
	bool colorChanged;
	float newAngle = 115.0f;
	int turnSpeed = 3;
	bool filterActivated;

	bool isMaterial;

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

	//miroir
	ofFbo mirrorFbo;
	ofCamera mirrorCam;

	int image_height, image_width;
	ofxAssimpModelLoader ant;
	ofxAssimpModelLoader redAnt;
	ofxAssimpModelLoader greenAnt;
	vector<ofxAssimpModelLoader*> ants;
	ofxAssimpModelLoader* activeAnt;

	float reliefStrength = 5.0f;
	ofMesh planeMeshRight;
	ofMesh planeMeshLeft;

	ofPlanePrimitive  leftWall, backWall, ceiling,floor, poster, cadrePlane;
	ofBoxPrimitive rightWall;
	ofImage posterImg;
	ofImage posterFilter;
	ofTexture posterTex;

	//GUI avec materiaux
	ofxPanel gui;
	ofParameter<bool> useMaterial;
	ofParameter<bool> blueTint;
	ofParameter<bool> redTint;
	ofParameter<bool> greenTint;
	ofParameter<bool> doubleTint;


	//Gui sans materiaux
	ofxPanel tintGui;
	ofParameter<ofColor> upperColor;
	ofParameter<ofColor> bottomColor;
	ofParameter<bool> noMaterial;
	ofColor upperColorChoice;
	ofColor bottomColorChoice;

	//Gui mur droit
	ofxPanel guiRight;
	ofParameter<bool> posterChoiceRight;
	bool textureActivated;
	ofParameter<bool> colorChoiceRight;
	ofParameter<ofColor> colorPickerRight;
	ofColor rightWallColor;
	ofParameter<bool> woodPick;
	ofImage wood;
	ofParameter<bool> brickPick;
	ofImage brick;
	ofParameter<bool> reliefActivatedRight;
	bool reliefIsActivated;
		
	//GUI mur gauche
	ofxPanel guiLeft;
	ofParameter<bool> colorChoiceLeft;
	ofParameter<ofColor> colorPickerLeft;
	ofColor leftWallColor;
	ofParameter<bool> posterPick;
	bool posterSet;
	ofParameter<bool> posterChoiceLeft;
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
	ofTexture wallTexture;
	ofImage img;

	ofImage imgPlateform;
	ofTexture texPlateform;
	void setup() ;
	void update() ;
	void draw();
	void drawScene();
	void mousePressed(int x, int y, int button) override;
	void startCameraTransition(glm::vec3 newPos, glm::vec3 newTarget);
	void resetCamera();
	void onUseMaterial(bool& value);
	void onNoMaterial(bool& value);
	void onBlueChanged(bool& value);
	void onRedChanged(bool& value);
	void onGreenChanged(bool& value);
	void onDoubleChanged(bool& value);

	void onIdentityChanged(bool& value);
	void onSharpChanged(bool& value);
	void onBorderChanged(bool& value);
	void onEmbossChanged(bool& value);
	void onBlurChanged(bool& value);

	void onColorRightPick(bool& value);
	void onTexturePick(bool& value);
	void onWoodPick(bool& value);
	void onBrickPick(bool& value);
	void onReliefSelected(bool& value);

	void onColorLeftPick(bool& value);
	void onPosterSet(bool& value);


	void applyFilterToImage(ofImage& sourceImage, ofImage& outputImage, const std::vector<float>& kernel, int kernelSize);
	void openPosterChoicer();
	void drawGUI();
	void activatedRelief(ofTexture& imgTexture, ofBoxPrimitive& box, ofMesh& boxMesh, ofImage grayscaleImg);

	const std::vector<float>& getKernelFromEnum(ConvolutionKernel kernelType);

	Ray createRayFromMouse(ofEasyCam& cam, int mouseX, int mouseY);
	bool intersectRay(const Ray& ray, ofPlanePrimitive& plane, glm::vec3& hitPoint);
};



