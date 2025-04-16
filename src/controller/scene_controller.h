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
#include <unordered_map>
#include <glm/gtx/hash.hpp> 
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
namespace std {
	template <>
	struct hash<glm::vec3> {
		size_t operator()(const glm::vec3& v) const {
			return hash<float>()(v.x) ^ hash<float>()(v.y) ^ hash<float>()(v.z);
		}
	};
}

constexpr float FREE_CAMERA_SPEED = 0.75;

class SceneController
{
public:
	static const int GRID_WIDTH = 100;
	static const int GRID_HEIGHT = 100;
	int SCENE_WIDTH;
	int SCENE_HEIGHT;
	int SCENE_X;
	int SCENE_Y;
	float RENDER_DISTANCE_WALLS = 300;
	float RENDER_DISTANCE_PHEROMONES = 300;
	float RENDER_DISTANCE_ANTS = 300;
	float centreX;
	float centreY;
	float boxSize;
	bool animation = false;
	bool checkPop;
	bool playMode;
	std::unordered_map < glm::vec3, float> pheromoneColorCache;

	GridController* gridController;
	Ant* ant;
	
	ofColor COLOR_AMBIENT = ofColor(255, 0, 0);
	ofColor COLOR_DIFFUSE = ofColor(0, 0, 255);
	ofBoxPrimitive boxCollider;

	ofImage wood;
	ofImage crackWall;
	ofImage rock;
	ofImage fire;
	ofImage glitter;
	ofImage paint;
	ofImage antTexture;

	//texture image
	ofImage albedo;
	ofImage normalMap;
	ofImage metallic;
	ofImage roughness;
	
	vector<ofTexture> wallTextures;
	ofTexture texture;
	ofTexture textureAnt;

	ofEasyCam* activeCam;
	vector<ofEasyCam*> cameras;
	int numCam;
	ofPoint freeCamPos = ofPoint();

	void setup(int x, int y, int w, int h, GridController* gridController);
	void update();
	void draw();
	void keyPressed(int key);
	void updateCellPositions();
	void updateAntPositions();
	void updateGridController(GridController*);
	ofxAssimpModelLoader& getAntModel();

private:
	ofShader shader_ant;
	ofShader shader_obj;
	ofShader shader_texture_wall;
	ofShader shader;
	ofLight light;
	ofxAssimpModelLoader antModelLoader;
	ofxAssimpModelLoader ants;
	ofxAssimpModelLoader slimes;
	ofVboMesh slimesMesh;
	ofBoxPrimitive box;
	ofVboMesh boxMesh;
	ofSpherePrimitive antSphere;
	ofMesh antMesh;
	ofVboMesh vboAntMesh;
	ofVboMesh vboAntModelLoader;
	ofSpherePrimitive pheromoneSphere;
	ofVboMesh vboPheromone;
	glm::vec3 boundingSize;
	ofEasyCam mainCamera;
	ofEasyCam topCamera;
	ofEasyCam freeCamera;
	
	ofEasyCam* popUpCam;
	ofEasyCam POV;
	
	ofCubeMap cubeMap;

	ofTexture a;
	ofTexture n;
	ofTexture m;

	ofRectangle sceneView;

	float scale_ant;
	bool mainCameraMode;
	vector<glm::vec3> wallPositions;
	vector<glm::vec3> antPositions;
	vector<tuple<glm::vec3, Cell*>> pheromonePositions;
	vector<float> antAngles;
	
	void drawScene();
	ofBoxPrimitive createBoundingBox(ofxAssimpModelLoader& model);
	bool checkCollision(glm::vec3 newPos);
	bool objectVisible(glm::vec3 pos, float);
	void move();
	void moveFreeCam();
};
