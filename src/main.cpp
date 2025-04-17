#include "ofMain.h"
#include "application.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW;
	settings.setGLVersion(3, 3);
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	shared_ptr<Application> mainApp = make_shared<Application>();

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
}