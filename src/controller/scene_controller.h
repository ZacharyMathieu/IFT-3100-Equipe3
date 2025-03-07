#pragma once
#include <of3dPrimitives.h>
#include <ofEasyCam.h>
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "ofLight.h"
#include <grid.h>
#include <vector>
#include <ant.h>
#include <grid_controller.h>


class SceneController
{
public:
	static const int GRID_WIDTH = 100;
	static const int GRID_HEIGHT = 100;
	int SCENE_WIDTH;
	int SCENE_HEIGHT;
	int SCENE_X;
	int SCENE_Y;
	float centreX;
	float centreY;
	int wallSize;
	GridController* gridController;

	ofColor COLOR_AMBIENT = ofColor(255, 0, 0);
	ofColor COLOR_DIFFUSE = ofColor(0, 0, 255);
	ofBoxPrimitive boxCollider;
	Ant* ant;

	void setup(int x, int y, int w, int h, GridController* gridController);
	void update();
	void draw();
	void keyPressed(int key);
	void updateWallPositions();
	void updateAntPositions();

private:
	ofShader shader_ant;
	ofShader shader_normal;
	ofShader shader;
	ofLight light;
	ofxAssimpModelLoader antModelLoader;
	ofBoxPrimitive box;
	ofVboMesh boxMesh;
	ofSpherePrimitive antSphere;
	ofVboMesh vboBoxMeshAnt;
	ofSpherePrimitive pheromoneSphere;
	ofVboMesh vboPheromone;
	glm::vec3 boundingSize;
	ofEasyCam mainCamera;
	ofEasyCam topCamera;
	ofEasyCam freeCamera;
	ofEasyCam* activeCam;
	vector<ofEasyCam*> cameras;
	int numCam;
	ofParameter<bool> checkPop;
	ofxPanel gui;
	ofRectangle sceneView;

	float scale_ant;
	bool mainCameraMode;
	vector<glm::vec3> wallPositions;
	vector<glm::vec3> antPositions;
	vector<glm::vec3> pheromonePositions;
	Ant* ant;

	void drawScene();
	ofBoxPrimitive createBoundingBox(ofxAssimpModelLoader& model);
	bool checkCollision(glm::vec3 newPos);
	float conversionColorToScale(Cell* cell);
	ofPoint conversionPixelToGrid(float x, float y); 
	bool objectBehindCam(glm::vec3 pos, int dist);
};
