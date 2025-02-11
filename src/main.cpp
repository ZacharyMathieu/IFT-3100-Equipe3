#include "ofMain.h"
#include "application.h"

//========================================================================
int main( ){
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW;

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<Application>());
	ofRunMainLoop();
}