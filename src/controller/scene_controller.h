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

namespace std {
	template <>
	struct hash<glm::vec3> {
		size_t operator()(const glm::vec3& v) const {
			return hash<float>()(v.x) ^ hash<float>()(v.y) ^ hash<float>()(v.z);
		}
	};
}

constexpr float BOUND = 200;
constexpr float MAX_X = BOUND;
constexpr float MIN_X = -BOUND;
constexpr float MAX_Y = BOUND;
constexpr float MIN_Y = -BOUND;
constexpr float MAX_Z = BOUND;
constexpr float MIN_Z = -BOUND;

class SceneController
{
public:
	float CELL_SIZE = 10;
	static const int GRID_WIDTH = 100;
	static const int GRID_HEIGHT = 100;
	int SCENE_WIDTH;
	int SCENE_HEIGHT;
	int SCENE_X;
	int SCENE_Y;
	float RENDER_DISTANCE_WALLS = 300;
	float RENDER_DISTANCE_PHEROMONES = 300;
	float RENDER_DISTANCE_ANTS = 300;
	int wallSize;
	std::unordered_map < glm::vec3, float> pheromoneColorCache;
	GridController* gridController;
	Ant* ant;
	ofPoint globalLightDirection = ofPoint(0.2, 0.5, 0.3);

	ofColor COLOR_AMBIENT = ofColor(255, 0, 0);
	ofColor COLOR_DIFFUSE = ofColor(0, 0, 255);
	ofBoxPrimitive boxCollider;
	ofPlanePrimitive *plane;
	ofImage wood;
	void setup(int x, int y, int w, int h, GridController* gridController);
	void update();
	void draw();
	void keyPressed(int key);
	void updateCellPositions();
	void updateAntPositions();
	void updateGridController(GridController*);
	void loadShaders();

private:
	ofShader shader_ant;
	ofShader shader_obj;
	ofLight light;
	ofxAssimpModelLoader antModelLoader;
	ofxAssimpModelLoader ants;
	ofxAssimpModelLoader slimes;
	ofVboMesh slimesMesh;
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
	ofEasyCam* popUpCam;
	ofEasyCam POV;
	vector<ofEasyCam*> cameras;
	int numCam;
	ofCubeMap cubeMap;

	ofParameter<bool> checkPop;
	ofxPanel gui;
	ofRectangle sceneView;

	float scale_ant;
	vector<glm::vec3> wallPositions;
	vector<tuple<ofPoint, float>> antData;
	vector<tuple<glm::vec3, Cell*>> pheromonePositions;

	void drawScene();
	ofBoxPrimitive createBoundingBox(ofxAssimpModelLoader& model);
	bool checkCollision(glm::vec3 newPos);
	bool objectVisible(glm::vec3 pos, float);
	ofPlanePrimitive* genPlane(float w, float h, ofPoint pos, ofPoint lookAt);
};
