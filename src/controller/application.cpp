#include "application.h"

void Application::setup()
{
    ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
    grid.setup(0, MENU_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
    setupButtons();
}

void Application::setupButtons()
{
    importImageIcon.load("images/importImage.png");
    exportImageIcon.load("images/exportImage.png");
    playIcon.load("images/play.png");
    pauseIcon.load("images/pause.png");
    fastForwardIcon.load("images/fastForward.png");
    eraseModeIcon.load("images/eraseMode.png");
    drawModeIcon.load("images/drawMode.png");
    shapeModeIcon.load("images/shapeMode.png");
    penTypeChoiceIcon.load("images/penTypeChoice.png");
    shapeChoiceIcon.load("images/shapeChoice.png");
    
    vector<std::tuple<Button *, void (Application::*)(), ofImage *>> buttonMap = {
        std::tuple(&importImageButton, &Application::importImage, &importImageIcon),
        std::tuple(&exportImageButton, &Application::exportImage, &exportImageIcon),
        std::tuple(&playButton, &Application::play, &playIcon),
        std::tuple(&fastForwardButton, &Application::fastForward, &fastForwardIcon),
        std::tuple(&eraseModeButton, &Application::eraseMode, &eraseModeIcon),
        std::tuple(&drawModeButton, &Application::drawMode, &drawModeIcon),
        std::tuple(&shapeModeButton, &Application::shapeMode, &shapeModeIcon),
        std::tuple(&penTypeChoiceButton, &Application::penTypeChoice, &penTypeChoiceIcon),
        std::tuple(&shapeChoiceButton, &Application::shapeChoice, &shapeChoiceIcon)};

    int i = 0;
    for (auto bTuple : buttonMap)
    {
        int xPos = i * (MENU_BUTTON_WIDTH + MENU_BUTTON_MARGIN);
        std::get<0>(bTuple)->setup(xPos, 0, MENU_BUTTON_WIDTH, MENU_HEIGHT, this, std::get<1>(bTuple), std::get<2>(bTuple));
        buttons.push_back(std::get<0>(bTuple));
        i++;
    }
}

void Application::drawMenu()
{
    ofSetBackgroundColor(255);
    ofSetColor(0);
    for (Button *b : buttons)
    {
        b->draw();
    }
    ofDrawLine(0, MENU_HEIGHT, WINDOW_WIDTH, MENU_HEIGHT);
}

//--------------------------------------------------------------
void Application::update()
{
}

//--------------------------------------------------------------
void Application::draw()
{
    drawMenu();

    if (imageLoaded)
    {
        ofSetColor(255);
        importedImage.draw(0, MENU_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - MENU_HEIGHT);
    }
    grid.draw();

    // Dessiner le curseur personnalisé
    ofSetColor(0);

    // Curseur pour le dessin
    if (cursorMode == DRAW) 
    {
        int cursorSize = drawCursorSize;
        int thickness = 1;
        ofSetColor(currentDrawColor);  // Appliquer la couleur sélectionnée
        ofSetLineWidth(thickness);
        ofDrawLine(ofGetMouseX() - cursorSize, ofGetMouseY(), ofGetMouseX() + cursorSize, ofGetMouseY());
        ofDrawLine(ofGetMouseX(), ofGetMouseY() - cursorSize, ofGetMouseX(), ofGetMouseY() + cursorSize);
    }

    // Curseur pour l’effacement
    else if (cursorMode == ERASE) 
    {
        ofNoFill();
        ofDrawCircle(ofGetMouseX(), ofGetMouseY(), eraserSize);
        ofFill();
    }

    if (showEraserMenu || showDrawMenu) 
    {
        ofShowCursor();
    } 
    else if (cursorMode == DRAW || cursorMode == ERASE) 
    {
        ofHideCursor();
    } 
    else 
    {
        ofShowCursor();
    }

    // Afficher le menu de l’effaceur
    if (showEraserMenu) 
    {
        ofSetColor(200);
        ofDrawRectangle(10, 60, 200, 50);

        ofSetColor(0);
        ofDrawBitmapString("Taille de l'efface", 20, 80);
        
        int sliderX = 20;
        int sliderY = 90;
        int sliderWidth = 150;
        
        ofDrawLine(sliderX, sliderY, sliderX + sliderWidth, sliderY);
        ofDrawCircle(sliderX + (eraserSize * sliderWidth / 50), sliderY, 5);
    }

    if (showColorMenu || showEraserMenu || showDrawMenu) 
    {
        ofShowCursor();
    }

    // Afficher le menu du crayon
    if (showDrawMenu) 
    {
        ofSetColor(200);
        ofDrawRectangle(10, 60, 200, 50);

        ofSetColor(0);
        ofDrawBitmapString("Taille du crayon", 20, 80);
        
        int sliderX = 20;
        int sliderY = 90;
        int sliderWidth = 150;
        
        ofDrawLine(sliderX, sliderY, sliderX + sliderWidth, sliderY);
        ofDrawCircle(sliderX + (drawCursorSize * sliderWidth / 50), sliderY, 5);
    }

    // Affichage du menu de sélection de couleur
    if (showColorMenu) 
    {
        ofSetColor(200);
        ofDrawRectangle(10, 120, 220, 170);
        ofSetColor(0);
        ofDrawBitmapString("Sélectionner une couleur", 20, 140);
        
        // Roulette de couleur
        int centerX = 110, centerY = 210, radius = 60;
        for (int angle = 0; angle < 360; angle += 5) 
        {
            float rad = ofDegToRad(angle);
            float x = centerX + cos(rad) * radius;
            float y = centerY + sin(rad) * radius;
            ofSetColor(ofColor::fromHsb(angle / 360.0 * 255, 255, 255));
            ofDrawCircle(x, y, 5);
        }

        // Marqueur noir pour la couleur sélectionnée
        float selectedAngle = (currentDrawColor.getHue() / 255.0) * 360.0;
        float selectedRad = ofDegToRad(selectedAngle);
        float selectedX = centerX + cos(selectedRad) * radius;
        float selectedY = centerY + sin(selectedRad) * radius;
        
        ofSetColor(0);
        ofDrawCircle(selectedX, selectedY, 5);
    }
}

//--------------------------------------------------------------
void Application::exit()
{
}

//--------------------------------------------------------------
void Application::keyPressed(int key)
{
}

//--------------------------------------------------------------
void Application::keyReleased(int key)
{
}

//--------------------------------------------------------------
void Application::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void Application::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void Application::mousePressed(int x, int y, int button)
{
    if (y < MENU_HEIGHT) 
    {
        showEraserMenu = false;
        showDrawMenu = false;
        showColorMenu = false;
    
        int buttonNumber = x / (MENU_BUTTON_WIDTH + MENU_BUTTON_MARGIN);

        if (buttonNumber == 7)
        {
            showColorMenu = !showColorMenu;
            return;
        }
        
        if (buttonNumber >= 0 && buttonNumber < buttons.size()) {
            buttons[buttonNumber]->mousePressed(x, y, button);
        }
    }

    if (showEraserMenu && y >= 85 && y <= 95) 
    { 
        int sliderX = 20;
        int sliderWidth = 150;
        
        if (x >= sliderX && x <= sliderX + sliderWidth) 
        {
            eraserSize = (x - sliderX) * 50 / sliderWidth;
        }
    }

    if (showDrawMenu && y >= 85 && y <= 95) 
    { 
        int sliderX = 20;
        int sliderWidth = 150;
        
        if (x >= sliderX && x <= sliderX + sliderWidth) 
        {
            drawCursorSize = (x - sliderX) * 50 / sliderWidth;
        }
    }

    if (showColorMenu) 
    {
        int centerX = 110, centerY = 210, radius = 60;
        int dx = x - centerX;
        int dy = y - centerY;
        float distance = sqrt(dx * dx + dy * dy);
    
        if (distance <= radius) { 
            float angle = atan2(dy, dx) * RAD_TO_DEG;
            if (angle < 0) angle += 360;
            currentDrawColor = ofColor::fromHsb(angle / 360.0 * 255, 255, 255);
        }
    }
     
}

//--------------------------------------------------------------
void Application::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void Application::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
}

//--------------------------------------------------------------
void Application::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void Application::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void Application::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void Application::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void Application::dragEvent(ofDragInfo dragInfo)
{
}

void Application::importImage()
{
    ofFileDialogResult result = ofSystemLoadDialog("Importer une image");
    if (result.bSuccess)
    {
        string filePath = result.getPath();
        ofLog() << "Tentative de chargement de l'image : " << filePath;

        if (importedImage.load(filePath))
        {
            imageLoaded = true;
            ofLog() << "Image importée avec succès : " << filePath;
            ofLog() << "Taille de l'image : " << importedImage.getWidth() << "x" << importedImage.getHeight();
        }
        else
        {
            ofLogError() << "Échec du chargement de l'image.";
            imageLoaded = false;
        }
    }
}

void Application::exportImage()
{
    cursorMode = DEFAULT;
}

void Application::play()
{
    cursorMode = DEFAULT;
    isRunning = !isRunning;
    if (isRunning)
    {
        playButton.setIcon(&pauseIcon);
    }
    else
    {
        playButton.setIcon(&playIcon);
    }
}

void Application::fastForward()
{
    cursorMode = DEFAULT;
}

void Application::eraseMode()
{
    cursorMode = ERASE;
    ofHideCursor();

    showEraserMenu = true;
    showDrawMenu = false;
}

void Application::drawMode()
{
    cursorMode = DRAW;
    ofHideCursor();

    showDrawMenu = true;
    showEraserMenu = false;
}

void Application::shapeMode()
{
    cursorMode = DEFAULT;
    ofShowCursor();
}

void Application::penTypeChoice()
{
    cursorMode = DRAW;
    showColorMenu = !showColorMenu;
    showEraserMenu = false;
    showDrawMenu = false;
    ofShowCursor(); // Assure que la souris apparaît
}

void Application::shapeChoice()
{
    // TODO
    cursorMode = DEFAULT;
}