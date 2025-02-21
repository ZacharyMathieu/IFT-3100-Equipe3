#pragma once
#include <of3dPrimitives.h>
#include <ofEasyCam.h>
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "ofLight.h"
class scene_controller
{
public :
	int SCENE_WIDTH;
	int SCENE_HEIGHT;
	int SCENE_x;
	int SCENE_y;
	float centre_x;
	float centre_y;
	void setup(int x, int y, int w, int h);
	void update();
	void draw();
	void mouseDragged(ofMouseEventArgs& args);
private:
	//ofBoxPrimitive box;
	ofEasyCam cam;
	ofCamera camera;
	ofRectangle sceneView;
	ofShader shader_ant;
	ofShader shader;
	ofLight light;
	ofxAssimpModelLoader ant;
	float scale_ant;




	
};

