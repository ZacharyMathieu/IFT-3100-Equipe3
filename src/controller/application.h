#pragma once

#include "ofMain.h"
#include "grid_controller.h"
#include "button.h"
#include "ofxGui.h"
#include "scene_controller.h"
#include "custom_scene_controller.h"
#include <GLFW/glfw3.h>
#include "ofAppGLFWWindow.h"


#ifndef APPLICATION
#define APPLICATION

enum CursorMode
{
	DEFAULT,
	DRAW,
	ERASE,
	SELECT
};

class Application : public ofBaseApp
{
public:
	static const int WINDOW_WIDTH = 800 * 2;
	static const int WINDOW_HEIGHT = 800;
	static const int MENU_HEIGHT = 50;
	static const int MENU_BUTTON_MARGIN = 10;
	static const int MENU_BUTTON_WIDTH = MENU_HEIGHT;
	static const int COLLAPSED_MENU_WIDTH = 30;
	static const int COLLAPSED_MENU_HEIGHT = 30;
	static const int OPENED_MENU_WIDTH = 220;
	static const int OPENED_MENU_HEIGHT_ERASE = 40;
	static const int OPENED_MENU_HEIGHT_DRAW = 40;
	static const int OPENED_MENU_HEIGHT_COLOR = 130;
	static const int WHEEL_RADIUS = 40;
	static const int COORD_MENU_X = 10;
	static const int COORD_MENU_Y = 60;
	static const int COORD_SLIDER_X = 20;
	static const int SLIDER_WIDTH = 200;
	static const int FAST_FORWARD_AMOUNT = 250;

	bool isRunning = false;
	SceneController sceneController;
	CustomSceneController customSceneController;

	shared_ptr<ofAppBaseWindow> antWindow;
    shared_ptr<CustomSceneController> antApp;


	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
	void mouseEntered(int x, int y) override;
	void mouseExited(int x, int y) override;
	void windowResized(int w, int h) override;
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;

	GLFWimage sablier;
	ofImage sablierimg;
	GLFWcursor* sablierCursor;

private:
	CursorMode cursorMode = DEFAULT;
	GridController gridController;
	ofImage importedImage;

	bool imageLoaded = false;
	bool showEraserMenu = false;
	bool showDrawMenu = false;
	bool showColorMenu = false;
	bool showTextureMenu = false;
	bool showTextureParameterMenu = false;
	bool showCameraMenu = false;
	bool showSelectedIcon = false;
	bool isEraserMenuCollapsed = false;
	bool isDrawMenuCollapsed = false;
	bool isColorMenuCollapsed = false;
	bool isTextureMenuCollapsed = false;
	bool isCameraMenuCollapsed = false;
	bool menuHidden = false;
	

	// Taille du crayon et de la gomme
	ofParameter<int> eraserSize;
	ofParameter<int> drawCursorSize;

	// Couleur du crayon
	ofParameterGroup penChoice;
	ofParameter<bool> wallPenChoice;
	ofParameter<bool> foodPenChoice;

	//Choix de texture
	ofParameter<bool> brickPick;
	ofParameter<bool> rockPick;
	ofParameter<bool> metalPick;
	ofParameter<bool> blueMetalPick;

	//GUi texture
	ofxPanel guiTextureParameter;
	ofParameter<ofColor> material_color_ambient;
	ofParameter<ofColor> material_color_diffuse;
	ofParameter<ofColor> material_color_specular;
	ofParameter<float> material_brightness;
	ofParameter<float> material_metallic;
	ofParameter<float> material_roughness;
	ofParameter<float> material_occlusion;
	ofParameter<ofPoint> material_fresnel_ior;

	//param�tre CameraGui
	ofParameter<bool> checkPop;
	ofParameterGroup cameraChoice;
	ofParameter<bool> mainCamera;
	ofParameter<bool> topCamera;
	ofParameter<bool> freeCamera;
	ofParameter<bool> POVCamera;

	// Interfaces graphiques (GUI)
	ofxPanel eraserGui; // GUI pour la gomme (taille uniquement)
	ofxPanel penGui;	// GUI pour le crayon (taille + couleur)
	ofxPanel colorGui;	// GUI pour la roue de couleur seule
	ofxPanel textureGui;
	ofxPanel cameraGui;
	ofxPanel gui;
	ofColor currentDrawColor = ofColor(255, 0, 0);

	ofImage importImageIcon;
	ofImage exportImageIcon;
	ofImage playIcon;
	ofImage pauseIcon;
	ofImage fastForwardIcon;
	ofImage eraseModeIcon;
	ofImage drawModeIcon;
	ofImage penTypeChoiceIcon;
	ofImage selectIcon;
	ofImage textureIcon;
	ofImage redoIcon;
	ofImage undoIcon;
	ofImage cameraIcon;
	ofImage antIcon;

	Button importImageButton;
	Button exportImageButton;
	Button playButton;
	Button pauseButton;
	Button fastForwardButton;
	Button eraseModeButton;
	Button drawModeButton;
	Button penTypeChoiceButton;
	Button selectButton;
	Button textureButton;
	Button undoButton;
	Button redoButton;
	Button cameraButton;
	Button antButton;
	vector<Button*> buttons;
	vector<ofParameter<bool>*> textureSelection;
	vector<ofParameter<bool>*> cameraSelection;

	ofParameter<ofColor> color_picker_ambient;
	ofParameter<ofColor> color_picker_diffuse;
	ofParameter<ofColor> tempColor;

	void setupButtons();
	void drawMenu();
	void drawCustomCursors();
	void onColorChanged(ofColor& color);
	void onDrawCursorSizeChanged(int& size);
	void onEraserSizeChanged(int& size);
	void importImage();
	void exportImage();
	void play();
	void fastForward();
	void eraseMode();
	void drawMode();
	void penTypeChoice();
	void multipleSelection();
	void textureChoice();
	void texturesToFalse();
	void camerasToFalse();
	void onTextureSelected(bool &value);
	void onCameraSelected(bool& value);
	void onWallSelected(bool& value);
	void onFoodSelected(bool& value);
	void undo();
	void redo();
	void cameraMode();
	void createColorCanva(string filepath);
	void onCheckPopChanged(bool& value);
	void changeCameraSelected(int num);
	void customAnt();
	void onAntWindowClosed(ofEventArgs& args);
	std::string cellTypeToString(CellType type);
	bool shouldResetAntApp = false;

};

#endif