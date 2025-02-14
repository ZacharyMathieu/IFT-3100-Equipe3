#include "application.h"

//--------------------------------------------------------------
void Application::setup()
{
    ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
    gridController.setup(0, MENU_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - MENU_HEIGHT);
    setupButtons();
}

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void Application::drawMenu()
{
    ofSetBackgroundColor(255);
    ofSetColor(0);
    for (Button *b : buttons)
    {
        ofSetColor(255);
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
    gridController.draw();

    // Dessiner les curseurs personnalisés
    ofSetColor(0);

    // Curseur pour le dessin
    if (cursorMode == DRAW)
    {
        int cursorSize = drawCursorSize;
        int thickness = 1;
        ofSetColor(currentDrawColor);
        ofSetLineWidth(thickness);
        ofDrawLine(ofGetMouseX() - cursorSize, ofGetMouseY(), ofGetMouseX() + cursorSize, ofGetMouseY());
        ofDrawLine(ofGetMouseX(), ofGetMouseY() - cursorSize, ofGetMouseX(), ofGetMouseY() + cursorSize);
    }

    // Curseur pour l’efface
    else if (cursorMode == ERASE)
    {
        ofNoFill();
        ofDrawCircle(ofGetMouseX(), ofGetMouseY(), eraserSize);
        ofFill();
    }

    // Affichage du curseur normal pour naviguer les menus
    if (showEraserMenu || showDrawMenu || showColorMenu)
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

    // Afficher le menu de l’efface
    if (showEraserMenu)
    {
        int menuWidth = isEraserMenuCollapsed ? COLLAPSED_MENU_WIDTH : OPENED_MENU_WIDTH;
        int menuHeight = isEraserMenuCollapsed ? COLLAPSED_MENU_HEIGHT : OPENED_MENU_HEIGHT_ERASE;
        ofSetColor(200);
        ofDrawRectangle(COORD_MENU_X, COORD_MENU_Y, menuWidth, menuHeight);

        // Dessiner la flèche associée
        int arrowX = COORD_MENU_X + menuWidth - 20;
        int arrowY = COORD_MENU_Y + 10;
        ofSetColor(0);
        if (isEraserMenuCollapsed)
        {
            ofDrawTriangle(arrowX, arrowY, arrowX + 10, arrowY, arrowX + 5, arrowY + 10);
        }
        else
        {
            ofDrawTriangle(arrowX, arrowY + 10, arrowX + 10, arrowY + 10, arrowX + 5, arrowY);
        }

        if (!isEraserMenuCollapsed)
        {
            ofDrawBitmapString("Taille de l'efface", COORD_MENU_X + 10, COORD_MENU_Y + 20);
            int sliderX = COORD_MENU_X + 10;
            int sliderY = COORD_MENU_Y + 35;
            ofDrawLine(sliderX, sliderY, sliderX + SLIDER_WIDTH, sliderY);
            ofDrawCircle(sliderX + (eraserSize * SLIDER_WIDTH / 50), sliderY, 5);
        }
    }

    // Afficher le menu du crayon (taille + couleur)
    if (showDrawMenu)
    {
        int menuWidth = isDrawMenuCollapsed ? COLLAPSED_MENU_WIDTH : OPENED_MENU_WIDTH;
        int menuHeight = isDrawMenuCollapsed ? COLLAPSED_MENU_HEIGHT : OPENED_MENU_HEIGHT_DRAW;
        ofSetColor(200);
        ofDrawRectangle(COORD_MENU_X, COORD_MENU_Y, menuWidth, menuHeight);

        // Dessiner la flèche associée
        int arrowX = COORD_MENU_X + menuWidth - 20;
        int arrowY = COORD_MENU_Y + 10;
        ofSetColor(0);
        if (isDrawMenuCollapsed)
        {
            ofDrawTriangle(arrowX, arrowY, arrowX + 10, arrowY, arrowX + 5, arrowY + 10);
        }
        else
        {
            ofDrawTriangle(arrowX, arrowY + 10, arrowX + 10, arrowY + 10, arrowX + 5, arrowY);
        }

        if (!isDrawMenuCollapsed)
        {
            ofDrawBitmapString("Taille du crayon", COORD_MENU_X + 10, COORD_MENU_Y + 20);
            int sliderX = COORD_MENU_X + 10;
            int sliderY = COORD_MENU_Y + 35;
            ofDrawLine(sliderX, sliderY, sliderX + SLIDER_WIDTH, sliderY);
            ofDrawCircle(sliderX + (drawCursorSize * SLIDER_WIDTH / 50), sliderY, 5);

            ofDrawBitmapString("Couleur du crayon", COORD_MENU_X + 10, COORD_MENU_Y + 60);
            int wheelCenterX = COORD_MENU_X + menuWidth / 2;
            int wheelCenterY = COORD_MENU_Y + 110;

            for (int angle = 0; angle < 360; angle += 5)
            {
                float rad = ofDegToRad(angle);
                float x = wheelCenterX + cos(rad) * WHEEL_RADIUS;
                float y = wheelCenterY + sin(rad) * WHEEL_RADIUS;
                ofSetColor(ofColor::fromHsb(angle / 360.0 * 255, 255, 255));
                ofDrawCircle(x, y, 4);
            }

            float selectedAngle = (currentDrawColor.getHue() / 255.0) * 360.0;
            float selectedRad = ofDegToRad(selectedAngle);
            float selectedX = wheelCenterX + cos(selectedRad) * WHEEL_RADIUS;
            float selectedY = wheelCenterY + sin(selectedRad) * WHEEL_RADIUS;
            ofSetColor(0);
            ofDrawCircle(selectedX, selectedY, 5);
        }
    }

    // Affichage du menu de sélection de couleur
    if (showColorMenu)
    {
        int menuWidth = isColorMenuCollapsed ? COLLAPSED_MENU_WIDTH : OPENED_MENU_WIDTH;
        int menuHeight = isColorMenuCollapsed ? COLLAPSED_MENU_HEIGHT : OPENED_MENU_HEIGHT_COLOR;
        ofSetColor(200);
        ofDrawRectangle(COORD_MENU_X, COORD_MENU_Y, menuWidth, menuHeight);

        // Dessiner la flèche associée
        int arrowX = COORD_MENU_X + menuWidth - 20;
        int arrowY = COORD_MENU_Y + 10;
        ofSetColor(0);
        if (isColorMenuCollapsed)
        {
            ofDrawTriangle(arrowX, arrowY, arrowX + 10, arrowY, arrowX + 5, arrowY + 10);
        }
        else
        {
            ofDrawTriangle(arrowX, arrowY + 10, arrowX + 10, arrowY + 10, arrowX + 5, arrowY);
        }

        if (!isColorMenuCollapsed)
        {
            ofDrawBitmapString("Sélectionner une couleur", COORD_MENU_X + 10, COORD_MENU_Y + 20);
            int wheelCenterX = COORD_MENU_X + menuWidth / 2;
            int wheelCenterY = COORD_MENU_Y + 70;

            for (int angle = 0; angle < 360; angle += 5)
            {
                float rad = ofDegToRad(angle);
                float x = wheelCenterX + cos(rad) * WHEEL_RADIUS;
                float y = wheelCenterY + sin(rad) * WHEEL_RADIUS;
                ofSetColor(ofColor::fromHsb(angle / 360.0 * 255, 255, 255));
                ofDrawCircle(x, y, 4);
            }

            float selectedAngle = (currentDrawColor.getHue() / 255.0) * 360.0;
            float selectedRad = ofDegToRad(selectedAngle);
            float selectedX = wheelCenterX + cos(selectedRad) * WHEEL_RADIUS;
            float selectedY = wheelCenterY + sin(selectedRad) * WHEEL_RADIUS;
            ofSetColor(0);
            ofDrawCircle(selectedX, selectedY, 5);
        }
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

        if (buttonNumber >= 0 && buttonNumber < buttons.size())
        {
            buttons[buttonNumber]->mousePressed(x, y, button);
        }
    }

    if (showEraserMenu && y >= 85 && y <= 95)
    {
        if (x >= COORD_SLIDER_X && x <= COORD_SLIDER_X + SLIDER_WIDTH)
        {
            eraserSize = (x - COORD_SLIDER_X) * 50 / SLIDER_WIDTH;
        }
    }

    if (showDrawMenu)
    {
        if (y >= 85 && y <= 95)
        {
            if (x >= COORD_SLIDER_X && x <= COORD_SLIDER_X + SLIDER_WIDTH)
            {
                drawCursorSize = (x - COORD_SLIDER_X) * 50 / SLIDER_WIDTH;
            }
        }

        int centerX = 110, centerY = 170, radius = 60;
        int dx = ofGetMouseX() - centerX;
        int dy = ofGetMouseY() - centerY;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= radius)
        {
            float angle = atan2(dy, dx) * RAD_TO_DEG;
            if (angle < 0)
                angle += 360;
            currentDrawColor = ofColor::fromHsb(angle / 360.0 * 255, 255, 255);
        }
        if (y > MENU_HEIGHT)
        {
            float scaleX = ((float)gridController.displayWidth) / gridController.grid.w;
            float scaleY = ((float)gridController.displayHeight) / gridController.grid.h;

            int xOrigine = (x / scaleX) * scaleX;
            int yOrigine = (y / scaleY) * scaleY;

            int gridX = (xOrigine - gridController.displayPosX) / scaleX;
            int gridY = (yOrigine - gridController.displayPosY) / scaleY;

            gridController.grid.at(gridX, gridY)->type = WALL;

        }
        
    }   


    if (showColorMenu)
    {
        int centerX = 110, centerY = 190, radius = 60;
        int dx = ofGetMouseX() - centerX;
        int dy = ofGetMouseY() - centerY;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= radius)
        {
            float angle = atan2(dy, dx) * RAD_TO_DEG;
            if (angle < 0)
                angle += 360;
            currentDrawColor = ofColor::fromHsb(angle / 360.0 * 255, 255, 255);
        }
    }

    // Vérifier si on clique sur la flèche du menu du crayon
    if (showDrawMenu && x >= (10 + (isDrawMenuCollapsed ? 20 : 200)) - 10 &&
        x <= (20 + (isDrawMenuCollapsed ? 30 : 200)) &&
        y >= 60 && y <= 75)
    {
        isDrawMenuCollapsed = !isDrawMenuCollapsed;
        return;
    }

    // Vérifier si on clique sur la flèche du menu de l’effaceur
    if (showEraserMenu && x >= (10 + (isEraserMenuCollapsed ? 20 : 200)) - 10 &&
        x <= (20 + (isEraserMenuCollapsed ? 30 : 200)) &&
        y >= 60 && y <= 75)
    {
        isEraserMenuCollapsed = !isEraserMenuCollapsed;
        return;
    }

    // Vérifier si on clique sur la flèche du menu de sélection de couleur
    if (showColorMenu && x >= (10 + (isColorMenuCollapsed ? 20 : 200)) - 10 &&
        x <= (20 + (isColorMenuCollapsed ? 30 : 200)) &&
        y >= 60 && y <= 75)
    {
        isColorMenuCollapsed = !isColorMenuCollapsed;
        return;
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
    std::string defaultPath = ofFilePath::getUserHomeDir() + "/capture.png";
    ofFileDialogResult saveFile = ofSystemSaveDialog(defaultPath, "save grid");

    if (saveFile.bSuccess)
    {
        std::string path = saveFile.getPath();
        path += ".png";
        ofImage screenshot;
        screenshot.grabScreen(0, MENU_HEIGHT, ofGetWidth(), ofGetHeight() - MENU_HEIGHT);

        screenshot.save(path);
    }

    cout << "exportImage\n";
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
    ofShowCursor();
}

void Application::shapeChoice()
{
    // TODO
    cursorMode = DEFAULT;
}