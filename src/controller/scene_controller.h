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
	int SCENE_WIDTH;
	int SCENE_HEIGHT;
	int SCENE_X;
	int SCENE_Y;
	float RENDER_DISTANCE_WALLS = 300;
	float RENDER_DISTANCE_PHEROMONES = 50;
	float RENDER_DISTANCE_FOOD = 300;
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
	ofPlanePrimitive pheromoneSquare;
	ofVboMesh pheromoneMesh;

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
	ofImage metallicRoughnessMap;

	vector<ofTexture> wallTextures;
	ofTexture texture;
	ofTexture textureAnt;

	ofEasyCam* activeCam;
	vector<ofEasyCam*> cameras;
	int numCam;
	ofPoint freeCamPos = ofPoint();

	void setup(int x, int y, int w, int h, GridController* gridController);
	void reloadShaders();
	void update();
	void draw();
	void keyPressed(int key);
	void updateCellPositions();
	void updateAntPositions();
	void bindAntTextures();
	void unbindAntTextures();
	void updateGridController(GridController*);
	ofxAssimpModelLoader& getAntModel();
	bool textureSelected = false;

	ofShader shader_texture_wall;
	ofImage texture_albedo;
	ofImage texture_normal;
	ofImage texture_arm;

	ofFbo miniViewportFbo;
	ofEasyCam miniViewportCam;
	ofBoxPrimitive miniBox;

	vector<string> brickTexture;
	vector<string> rockTexture;
	vector<string> metalTexture;
	vector<string> blueMetalTexture;


private:

	//texture wall
	
	float material_metallic;
	float material_roughness;
	float material_occlusion;
	float material_brightness;
	ofColor material_color_ambient;
	ofColor material_color_diffuse;
	ofColor material_color_specular;
	glm::vec3 material_fresnel_ior;
	ofColor light_color;
	float tone_mapping_exposure;
	float tone_mapping_gamma;
	bool tone_mapping_toggle;


	ofPlanePrimitive tilePheromone;
	ofVboMesh tilePheromoneMesh;
	vector<glm::mat4> pheromoneMatrix;

	ofShader shader_ant;
	ofShader shader_obj;
	
	ofShader* shader;
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
	ofSpherePrimitive foodSphere;
	ofVboMesh foodSphereMesh;
	//ofVboMesh vboPheromone;
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
	vector<glm::vec3> foodPositions;
	vector<float> antAngles;

	void drawScene();
	ofBoxPrimitive createBoundingBox(ofxAssimpModelLoader& model);
	bool checkCollision(glm::vec3 newPos);
	bool objectVisible(glm::vec3 pos, float);
	void move();
	void moveFreeCam();
	ofPoint& movePOV();

};
