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
	Grid grid = Grid(GRID_WIDTH, GRID_HEIGHT);
	vector<glm::vec3> positions;
	ofColor color_ambient;
	ofColor color_diffuse;
	ofMesh mesh;
	
	void setup(int x, int y, int w, int h);
	void update();
	void draw();
	void keyPressed(int key);
private:
	ofBoxPrimitive box;
	ofVboMesh boxMesh;
	ofEasyCam cam;
	ofCamera camera;
	ofRectangle sceneView;
	ofShader shader_ant;
	ofShader shader_normal;
	ofShader shader;
	ofLight light;
	ofxAssimpModelLoader ant;
	float scale_ant;
	float speed;
	float rotation;
};
