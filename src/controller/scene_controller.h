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

	//Grid grid = Grid(GRID_WIDTH, GRID_HEIGHT);
	ofColor color_ambient;
	ofColor color_diffuse;
	ofBoxPrimitive boxCollider;

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
	glm::vec3 boundingSize;
	ofEasyCam mainCamera;
	ofCamera topCamera;

	ofParameter<bool> checkPop;
	ofxPanel gui;
	ofRectangle sceneView;

	float scale_ant;
	float speed;
	float rotation;
	bool mainCameraMode;
	vector<glm::vec3> wallPositions;
	vector<glm::vec3> antPositions;
	Ant* ant;

	void drawScene();
	ofBoxPrimitive createBoundingBox(ofxAssimpModelLoader& model);
	bool checkCollision(glm::vec3 newPos);
};
