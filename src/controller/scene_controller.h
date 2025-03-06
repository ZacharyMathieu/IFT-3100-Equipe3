#pragma once
#include <of3dPrimitives.h>
#include <ofEasyCam.h>
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "ofLight.h"
#include <grid.h>
#include <vector>


class SceneController
{
public :
	static const int GRID_WIDTH = 100;
	static const int GRID_HEIGHT = 100;
	int SCENE_WIDTH;
	int SCENE_HEIGHT;
	int SCENE_x;
	int SCENE_y;
	float centre_x;
	float centre_y;
	int wallSize;

	Grid grid = Grid(GRID_WIDTH, GRID_HEIGHT);
	vector<glm::vec3> positions;
	vector<glm::vec3> antPositions;
	ofColor color_ambient;
	ofColor color_diffuse;
	ofBoxPrimitive boxCollider;

	void setup(int x, int y, int w, int h);
	void update();
	void draw();
	void keyPressed(int key);
	//void drawSecondWindow();
	
	
private:
	ofShader shader_ant;
	ofShader shader_normal;
	ofShader shader;
	ofLight light;
	ofxAssimpModelLoader ant;
	ofBoxPrimitive box;
	ofVboMesh boxMesh;
	ofSpherePrimitive ants;
	
	ofVboMesh vboBoxMeshAnt;
	glm::vec3 boundingSize;
	ofEasyCam cam;
	ofCamera camera;

	ofParameter<bool> checkPop;
	ofxPanel gui;
	ofRectangle sceneView;
	
	float scaleX;
	float scaleY;
	float scale_ant;
	float speed;
	float rotation;
	bool mainCamera;
	
	void drawScene();
	ofBoxPrimitive createBoundingBox(ofxAssimpModelLoader& model);
	bool checkCollision(glm::vec3 newPos);
	
};
