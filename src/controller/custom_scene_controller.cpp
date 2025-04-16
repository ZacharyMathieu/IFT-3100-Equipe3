#include "custom_scene_controller.h"



void CustomSceneController::setup()
{
    ofDisableArbTex(); // dans setup() AVANT le chargement des images

    ofEnableLighting();
    ofSetFrameRate(60);

    leftPos = rightTarget = glm::vec3(49, 0, 0);
    rightPos = leftTarget = glm::vec3(-49, 0, 0);
    posterPos = glm::vec3(0, 5, 25);
    posterTarget = glm::vec3(0, 5, -25);
    
    isMaterial = false;
    light.setup();
    light.setDirectional();
    light.setDiffuseColor(ofFloatColor(2.0, 2.0, 2.0));   // deux fois plus intense
    light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));  // plus fort
    light.setAmbientColor(ofFloatColor(0.2));

    cadre.load("images/cadre.png");
    ck = ConvolutionKernel::identity;

    // Load ton mod�le
    ant.loadModel("models/sci-fiAnt/ant-SciFi.gltf");
    redAnt.loadModel("models/sci-fiAnt2/ant-SciFi.gltf");
    greenAnt.loadModel("models/sci-fiAnt3/ant-SciFi.gltf");

    ants.push_back(&ant);
    ants.push_back(&redAnt);
    ants.push_back(&greenAnt);

    activeAnt = ants[0];
    

    shader.load("custom_ant_330_vs.glsl", "custom_ant_330_fs.glsl");
    
    antTexture = ant.getTextureForMesh(0);
    ofImage baseColorImg;
    baseColorImg.load("models/sci-fiAnt/texture/Image_0.png");  // Remplace par le vrai chemin
    baseColorTexture = baseColorImg.getTexture();

    ofImage normalImg;
    normalImg.load("models/sci-fiAnt/texture/Image_3.png");  // Remplace par le vrai chemin
    normalMapTexture = normalImg.getTexture();

    blueEye.load("models/sci-fiAnt/texture/Image_2.png");  // Remplace par le vrai chemin
    metallicTexture = blueEye.getTexture();

    redEye.load("models/sci-fiAnt/texture/Image_2_red.png");
    greenEye.load("models/sci-fiAnt/texture/Image_2_green.png");
    doubleColorEye.load("models/sci-fiAnt/texture/Image_2_gp.png");

    ofImage roughnessImg;
    roughnessImg.load("models/sci-fiAnt/texture/Image_1.png");  // Remplace par le vrai chemin
    roughnessTexture = roughnessImg.getTexture();

    imgPlateform.load("images/wood.jpg");
    wood.load("images/wood.jpg");
    brick.load("images/brick.jpg");
    

    texPlateform = imgPlateform.getTexture();

    activeAnt->setPosition(0, -45, 0); // L�g�rement au-dessus du sol

    // Cr�e les plans
    float boxSize = 100;
    float wallThickness = 1.0;

    // Sol
    floor.set(boxSize+20, boxSize+20);
    floor.rotateDeg(90, 1, 0, 0);
    floor.setPosition(0, -50, 0);

    // Plafond
    ceiling.set(boxSize+20, boxSize+20);
    ceiling.rotateDeg(90, 1, 0, 0);
    ceiling.setPosition(0, 50, 0);

    // Mur arri�re
    backWall.set(boxSize, boxSize);
    backWall.setPosition(0, 0, -boxSize / 2);

    cadrePlane.set(90, 70);
    cadrePlane.setPosition(0, 0, (-boxSize / 2) + 1);
    cadrePlane.rotateDeg(180, 0, 1, 0);

    poster.set(62, 47);

    poster.setPosition(0, -1, ( - boxSize / 2) +2); 
    poster.rotateDeg(180, 0, 1, 0); 

    image_height = posterImg.getHeight();
    image_width = posterImg.getWidth();

    
    posterFilter.allocate(image_width, image_height, OF_IMAGE_COLOR);

    // Mur gauche
    leftWall.set(boxSize+20, boxSize);
    leftWall.rotateDeg(90, 0, 1, 0);
    leftWall.setPosition(-boxSize / 2, 0, 0);
    planeMeshLeft = leftWall.getMesh();

    // Mur droit
    rightWall.set(boxSize+20, boxSize+5,2);
    rightWall.setResolution(100,100,1);
    rightWall.rotateDeg(90, 0, 1, 0);
    rightWall.setPosition(boxSize / 2, 0, 0);
    planeMeshRight = rightWall.getMesh();

    plateform.set(40, 5);
    plateform.setResolution(64, 1);
    plateform.setPosition(0, -48, 0);

    // Cam�ra

    firstPos = glm::vec3(0, -10, 100);
    firstTarget = glm::vec3(0, -10, 0);
    cam.setPosition(firstPos);
    cam.lookAt(firstTarget);

    //GUI avec materiaux
    gui.setup("With Materials");
    gui.setPosition(10, 10);

    useMaterial.setName("Use material");
    gui.add(useMaterial);
    useMaterial.addListener(this, &CustomSceneController::onUseMaterial);
    blueTint.setName("Blue eyes");
    gui.add(blueTint);
    blueTint.addListener(this, &CustomSceneController::onBlueChanged);
    redTint.setName("Red eyes");
    gui.add(redTint);
    redTint.addListener(this, &CustomSceneController::onRedChanged);
    greenTint.setName("Green eyes");
    gui.add(greenTint);
    greenTint.addListener(this, &CustomSceneController::onGreenChanged);
    
    //GUI sans materiaux
    tintGui.setup("Without Materials");
    tintGui.setPosition(gui.getWidth() + 10, 10);
    noMaterial.setName("No material");
    tintGui.add(noMaterial);
    noMaterial.addListener(this, &CustomSceneController::onNoMaterial);
    tintGui.add(upperColor.set("Upper part", ofColor(100, 100, 100), ofColor(0, 0, 0), ofColor(255, 255, 255)));
    tintGui.add(bottomColor.set("Bottom part", ofColor(0, 140, 155), ofColor(0, 0, 0), ofColor(255, 255, 255)));


    //GUI mur droit
    guiRight.setup("Texture wall");
    guiRight.setPosition(gui.getPosition());
    colorChoiceRight.setName("Color wall");
    guiRight.add(colorChoiceRight);
    colorChoiceRight.addListener(this, &CustomSceneController::onColorRightPick);
    colorChoiceRight = true;
    guiRight.add(colorPickerRight.set("color piker", ofColor(0, 255, 0), ofColor(0, 0, 0), ofColor(255, 255, 255)));
    posterChoiceRight.setName("Texture choice");
    guiRight.add(posterChoiceRight);
    posterChoiceRight.addListener(this, &CustomSceneController::onTexturePick);
    posterChoiceRight = false;
    reliefActivatedRight.setName("relief activated");
    guiRight.add(reliefActivatedRight);
    reliefActivatedRight.addListener(this, &CustomSceneController::onReliefSelected);
    reliefActivatedRight = false;
    woodPick.setName("Wood texture");
    guiRight.add(woodPick);
    woodPick.addListener(this, &CustomSceneController::onWoodPick);
    woodPick = true;
    brickPick.setName("Brick texture");
    guiRight.add(brickPick);
    brickPick.addListener(this, &CustomSceneController::onBrickPick);
    brickPick = false;
   
    
    //GUI mur gauche
    guiLeft.setup("Poster wall");
    guiLeft.setPosition(gui.getPosition());
    colorChoiceLeft.setName("Color wall");
    colorChoiceLeft.addListener(this, &CustomSceneController::onColorLeftPick);
    guiLeft.add(colorChoiceLeft);
    colorChoiceLeft = true;
    guiLeft.add(colorPickerLeft.set("color piker", ofColor(255, 0, 0), ofColor(0, 0, 0), ofColor(255, 255, 255)));
    posterPick.setName("Poster wall");
    guiLeft.add(posterPick);
    posterPick.addListener(this, &CustomSceneController::onPosterSet);
    posterPick = false;
    posterChoiceLeft.setName("Choice your poster");
    guiLeft.add(posterChoiceLeft);
    posterChoiceLeft = false;
    identite.setName("Identity filter");
    identite.addListener(this, &CustomSceneController::onIdentityChanged);
    guiLeft.add(identite);
    aiguiser.setName("Sharpen filter");
    aiguiser.addListener(this, &CustomSceneController::onSharpChanged);
    guiLeft.add(aiguiser);
    border.setName("Edge detect filter");
    border.addListener(this, &CustomSceneController::onBorderChanged);
    guiLeft.add(border);
    bosseler.setName("Emboss filter");
    bosseler.addListener(this, &CustomSceneController::onEmbossChanged);
    guiLeft.add(bosseler);
    flou.setName("blur filter");
    flou.addListener(this, &CustomSceneController::onBlurChanged);
    guiLeft.add(flou);

    identite = aiguiser = border = bosseler = flou = false;

    blueTint = true;
    redTint = false;
    greenTint = false;
    doubleTint = false;
    
    filterActivated = false;

    resetButton.set(ofGetWidth() / 2 - 50, 5,75, 40);
    cam.disableMouseInput();
    light.setPosition(0, 0, 0);
    light.lookAt(activeAnt->getPosition());


}

void CustomSceneController::update()
{
    if (ofGetKeyPressed(OF_KEY_RIGHT))
    {
        newAngle -= turnSpeed;
    }

    if (ofGetKeyPressed(OF_KEY_LEFT))
    {
        newAngle += turnSpeed;
    }
    activeAnt->setPosition(0, -45, 0);
	activeAnt->setScale(0.15, 0.15, 0.15);
	activeAnt->setRotation(0, -90, 1, 0, 0);
	activeAnt->setRotation(1, newAngle, 0, 0, 1);
    plateform.setOrientation(glm::vec3(0, -newAngle, 0));

    activeAnt->update();
    activeAnt->getAnimation(0).play();

    
    light.lookAt(ant.getPosition());

    light.setGlobalPosition(glm::vec3(0,40, 0));

    if (isTransitioning) {
        float elapsed = ofGetElapsedTimef() - transitionStartTime;
        float t = ofClamp(elapsed / transitionDuration, 0.0f, 1.0f);

        glm::vec3 currentPos = glm::mix(startPos, endPos, t);
        glm::vec3 currentTarget = glm::mix(startTarget, endTarget, t);

        cam.setPosition(currentPos);
        cam.lookAt(currentTarget);

        if (t >= 1.0f) {
            isTransitioning = false;
        }
    }
    if (identite || aiguiser || bosseler || border || flou) {

    }
    else {
        filterActivated = false;
    }
    upperColorChoice = upperColor;
    bottomColorChoice = bottomColor;
    rightWallColor = colorPickerRight;
    leftWallColor = colorPickerLeft;
    
    if (posterChoiceLeft) openPosterChoicer();
}

void CustomSceneController::draw()
{
    //gui.draw();
    ofEnableDepthTest();
    ofBackground(0);
    cam.begin();
    // Lumi�re
    light.enable();

    // Dessine les murs (avec couleurs distinctes)
    ofSetColor(180); // Sol gris clair
    floor.draw();

    ofSetColor(255); // Plafond blanc
    ceiling.draw();

    ofSetColor(200); // Mur arri�re gris
    backWall.draw();

    cadre.getTexture().bind();
    cadrePlane.draw();
    cadre.getTexture().unbind();
    
    ofPushMatrix();
    ofTranslate(leftWall.getPosition());
    ofRotateDeg(90, 0, 1, 0);
    ofSetColor(255);
    if (posterSet) {
        if (filterActivated)
        {
            posterFilter.getTexture().bind();
            planeMeshLeft.draw();
            posterFilter.getTexture().unbind();
        }
        else {
            posterImg.getTexture().bind();
            planeMeshLeft.draw();
            posterImg.getTexture().unbind();
        }
        
    }
    else {
        ofSetColor(leftWallColor);
        planeMeshLeft.draw();
    }
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(rightWall.getPosition());
    ofRotateDeg(90, 0, 1, 0);
    ofSetColor(255);
    if (textureActivated) {
        wallTexture.bind();
        planeMeshRight.draw();
        wallTexture.unbind();
    }
    else {
        ofSetColor(rightWallColor);
        planeMeshRight.draw();
    }
    
    ofPopMatrix();

    ofSetColor(100);
    texPlateform.bind();
    plateform.draw();
    texPlateform.unbind();

    if (!isMaterial) {
        activeAnt->disableMaterials();
        shader.begin();

        // Matrices
        glm::mat4 modelMatrix = activeAnt->getModelMatrix();
        glm::mat4 viewMatrix = ofGetCurrentViewMatrix();
        glm::mat4 projectionMatrix = cam.getProjectionMatrix();
        glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

        shader.setUniformMatrix4f("modelMatrix", modelMatrix);
        shader.setUniformMatrix4f("modelViewProjectionMatrix", modelViewProjectionMatrix);
        shader.setUniformMatrix3f("normalMatrix", normalMatrix);

        // Lumi�re
        shader.setUniform3f("lightPos", light.getGlobalPosition());
        shader.setUniform3f("viewPos", cam.getGlobalPosition());

        // Textures
        shader.setUniformTexture("baseColorMap", baseColorTexture, 0);
        shader.setUniformTexture("normalMap", normalMapTexture, 1);
        shader.setUniformTexture("metallicMap", metallicTexture, 2);
        shader.setUniformTexture("roughnessMap", roughnessTexture, 3);

        // Teinte dynamique (exemple avec ofColor dans ton GUI)
        shader.setUniform3f("upperTint", upperColorChoice.r / 255.0f,
            upperColorChoice.g / 255.0f,
            upperColorChoice.b / 255.0f);
        shader.setUniform3f("bottomTint", bottomColorChoice.r / 255.0f,
            bottomColorChoice.g / 255.0f,
            bottomColorChoice.b / 255.0f);

        activeAnt->drawFaces();
        shader.end();

    }

        /*shader.begin();

        shader.setUniformTexture("baseColorMap", baseColorTexture, 0);
        shader.setUniformTexture("normalMap", normalMapTexture, 1);
        shader.setUniformTexture("metallicMap", metallicTexture, 2);
        shader.setUniformTexture("roughnessMap", roughnessTexture, 3);

        shader.setUniform3f("lightPos", light.getGlobalPosition());
        shader.setUniform3f("viewPos", cam.getGlobalPosition());

        shader.setUniform3f("tintUpper", upperColorChoice.r/255.0f, upperColorChoice.g / 255.0f, upperColorChoice.b / 255.0f);
        shader.setUniform3f("tintLower", bottomColorChoice.r / 255.0f, bottomColorChoice.g / 255.0f, bottomColorChoice.b / 255.0f) ;

        activeAnt->drawFaces();

        shader.end();*/
    

    else {
        activeAnt->enableMaterials();
        activeAnt->enableTextures();
        activeAnt->drawFaces();
    }



    cam.end();

    light.disable();

    ofDisableDepthTest(); 
    if (cam.getPosition() == leftPos || cam.getPosition() == rightPos || cam.getPosition() == posterPos) {
        ofFill();
        ofSetColor(0); // gris clair
        ofDrawRectangle(resetButton);
        ofNoFill();
        ofSetColor(100);
        ofDrawRectangle(resetButton);
        ofSetColor(255); // texte noir
        ofDrawBitmapString("Reset", resetButton.x + 10, resetButton.y + 25);
    }

    drawGUI();

}
void CustomSceneController::mousePressed(int x, int y, int button)
{
    uint64_t now = ofGetElapsedTimeMillis();
    if (button == OF_MOUSE_BUTTON_LEFT) {
        if (x < ofGetWidth() / 3 && x > 0 && (now - lastClickTime) < doubleClickDelay && cam.getPosition() == firstPos) {
            
            startCameraTransition(leftPos, leftTarget);

        }
        else if (x > 2*(ofGetWidth() / 3) && x < ofGetWidth() && (now - lastClickTime) < doubleClickDelay && cam.getPosition() == firstPos)
        {
            startCameraTransition(rightPos, rightTarget);
        }
        else if (x > ofGetWidth() / 2 - 100 && x < ofGetWidth() / 2 + 100 && y > ofGetHeight() / 2 - 100 && y < ofGetHeight() / 2 + 100 && cam.getPosition() == firstPos) {
            
            startCameraTransition(posterPos, posterTarget);
        }
        if (resetButton.inside(x, y)) {
            if (now - lastClickTime < doubleClickDelay) {
                // Action de reset
                ofLogNotice() << "Reset triggered!";
                resetCamera();
            }
        }
    }
   
    lastClickTime = now;
}

void CustomSceneController::startCameraTransition(glm::vec3 newPos, glm::vec3 newTarget) {
    startPos = cam.getPosition();
    startTarget = cam.getTarget().getPosition();
    endPos = newPos;
    endTarget = newTarget;

    transitionStartTime = ofGetElapsedTimef();
    isTransitioning = true;
}
void CustomSceneController::resetCamera()
{
    startCameraTransition(firstPos, firstTarget);
}
void CustomSceneController::onUseMaterial(bool& value)
{
    isMaterial = value;
    if (value) {
        activeAnt = ants[0];
        blueTint = false;
        redTint = false;
        greenTint = false;
        doubleTint = false;
        noMaterial = false;
    }

}
void CustomSceneController::onNoMaterial(bool& value)
{
    noMaterial = value;
    if (value) {
        activeAnt = ants[0];
        isMaterial = false;
    }
}
void CustomSceneController::onBlueChanged(bool& value)
{
    antColor = value;
    if (value) {
        activeAnt = ants[0];
        redTint = false;
        greenTint = false;
        doubleTint = false;
        useMaterial = false;
    }
    
}
void CustomSceneController::onRedChanged(bool& value)
{
    normalColor = value;
    if (value) {
        activeAnt = ants[1];
        blueTint = false;
        greenTint = false;
        doubleTint = false;
        useMaterial = false;
    }
    
}
void CustomSceneController::onGreenChanged(bool& value)
{
    metallicColor = value;
    if (value) {
        activeAnt = ants[2];
        blueTint = false;
        redTint = false;
        doubleTint = false;
        useMaterial = false;
    }
    
}
void CustomSceneController::onDoubleChanged(bool& value)
{
    roughnessColor = value;
    if (value) {
        blueTint = false;
        redTint = false;
        useMaterial = false;
        greenTint = false;
    }
    
}
void CustomSceneController::onIdentityChanged(bool& value)
{
    identite = value;
    if (value) {
        aiguiser = border = bosseler = flou = false;
        ck = ConvolutionKernel::identity;
        applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
        filterActivated = true;
    }
        
}
void CustomSceneController::onSharpChanged(bool& value)
{
    aiguiser = value;
    if (value) {
        identite = border = bosseler = flou = false;
        ck = ConvolutionKernel::sharpen;
        applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
        filterActivated = true;
    }
        
}
void CustomSceneController::onBorderChanged(bool& value)
{
    border = value;
    if (value) {
        aiguiser = identite = bosseler = flou = false;
        ck = ConvolutionKernel::edge_detect;
        applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
        filterActivated = true;
    }
        
}
void CustomSceneController::onEmbossChanged(bool& value)
{
    bosseler = value;
    if (value) {
        aiguiser = border = identite = flou = false;
        ck = ConvolutionKernel::emboss;
        applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
        filterActivated = true;
    }
       
}
void CustomSceneController::onBlurChanged(bool& value)
{
    flou = value;
    if (value) {
        aiguiser = border = bosseler = identite = false;
        ck = ConvolutionKernel::blur;
        applyFilterToImage(posterImg, posterFilter, getKernelFromEnum(ck), 3);
        filterActivated = true;
    }
        
}
void CustomSceneController::onColorRightPick(bool& value)
{
    colorChoiceRight = value;
    if (value)
    {
        posterChoiceRight = false;
    }
}
void CustomSceneController::onColorLeftPick(bool& value)
{
    colorChoiceLeft = value;
    if (value)
    {
        posterPick = false;
    }
}
void CustomSceneController::onPosterSet(bool& value)
{
    posterSet = posterPick = value;
    if (value) {
        colorChoiceLeft = false;
    }
}
void CustomSceneController::onTexturePick(bool& value)
{
    textureActivated = posterChoiceRight = value;
    if (value)
    {
        colorChoiceRight = false;
    }
}
void CustomSceneController::onWoodPick(bool& value)
{
    woodPick = value;
    if (value) {
        wallTexture = wood.getTexture();
        brickPick = false;
    }
}
void CustomSceneController::onBrickPick(bool& value)
{
    brickPick = value;
    if (value) {
        wallTexture = brick.getTexture();
        woodPick = false;
    }
}
void CustomSceneController::onReliefSelected(bool& value)
{
    reliefIsActivated = reliefActivatedRight = value;
    if (value)
    {
        activatedRelief(wallTexture, rightWall, planeMeshRight, woodPick ? wood: brick);
    }
    else {
        planeMeshRight = rightWall.getMesh();
    }
}
void CustomSceneController::applyFilterToImage(ofImage& sourceImage, ofImage& outputImage, const std::vector<float>& kernel, int kernelSize)
{
    if (!sourceImage.isAllocated()) return;

    int width = sourceImage.getWidth();
    int height = sourceImage.getHeight();
    int offset = kernelSize / 2;

    ofPixels& srcPixels = sourceImage.getPixels();
    ofPixels dstPixels;
    dstPixels.allocate(width, height, OF_IMAGE_COLOR);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float r = 0.0f, g = 0.0f, b = 0.0f;

            for (int ky = -offset; ky <= offset; ++ky)
            {
                for (int kx = -offset; kx <= offset; ++kx)
                {
                    int px = ofClamp(x + kx, 0, width - 1);
                    int py = ofClamp(y + ky, 0, height - 1);
                    int kernelIndex = (ky + offset) * kernelSize + (kx + offset);

                    ofColor c = srcPixels.getColor(px, py);
                    float k = kernel[kernelIndex];

                    r += c.r * k;
                    g += c.g * k;
                    b += c.b * k;
                }
            }

            dstPixels.setColor(x, y, ofColor(ofClamp(r, 0, 255), ofClamp(g, 0, 255), ofClamp(b, 0, 255)));
        }
    }

    outputImage.setFromPixels(dstPixels);
}

void CustomSceneController::openPosterChoicer()
{
    ofFileDialogResult result = ofSystemLoadDialog("Choisir une image");
    if (result.bSuccess) {
        posterImg.load(result.getPath());
        leftWall.mapTexCoordsFromTexture(posterImg.getTexture());
        posterSet = posterPick = true;
    }
    posterChoiceLeft = false;
}

void CustomSceneController::drawGUI()
{
    //cam.getPosition() == leftPos || cam.getPosition() == rightPos || cam.getPosition() == posterPos;
    if (cam.getPosition() == leftPos)
    {
        guiLeft.draw();
    }
    else if (cam.getPosition() == rightPos) {
        guiRight.draw();
    }
    else if (cam.getPosition() == firstPos) {
        gui.draw();
        tintGui.draw();
    }
}

void CustomSceneController::activatedRelief(ofTexture& imgTexture, ofBoxPrimitive& box, ofMesh& boxMesh, ofImage grayscaleImg)
{
    grayscaleImg.setImageType(OF_IMAGE_GRAYSCALE);
    // Applique les coordonn�es UV � la bo�te (pour la texture)
    box.mapTexCoordsFromTexture(imgTexture);

    // R�cup�re les dimensions pour bien mapper les coordonn�es
    float width = box.getWidth();
    float height = box.getHeight();

    // R�cup�re une copie du mesh de la bo�te
    boxMesh = box.getMesh();

    for (int i = 0; i < boxMesh.getNumVertices(); ++i) {
        glm::vec3 v = boxMesh.getVertex(i);

        // Conversion des coordonn�es du sommet en coordonn�e d�image (u,v)
        float u = ofMap(v.x, -width / 2.0f, width / 2.0f, 0, grayscaleImg.getWidth() - 1);
        float vImg = ofMap(v.y, -height / 2.0f, height / 2.0f, 0, grayscaleImg.getHeight() - 1);

        // R�cup�re la luminosit� � cette position (image en niveaux de gris)
        float brightness = grayscaleImg.getColor(u, vImg).getLightness() / 255.0f;

        // Applique un d�placement selon la profondeur Z (face frontale)
        float bumpAmount = brightness * reliefStrength;

        v.z += bumpAmount * 0.5f;

        // Met � jour le sommet
        boxMesh.setVertex(i, v);
    }

    // Recalcule les UV
    boxMesh.clearTexCoords();
    boxMesh.addTexCoords(box.getMesh().getTexCoords());
}
const std::vector<float>& CustomSceneController::getKernelFromEnum(ConvolutionKernel kernelType)
{
    static const std::vector<float> identity(convolution_kernel_identity.begin(), convolution_kernel_identity.end());
    static const std::vector<float> sharpen(convolution_kernel_sharpen.begin(), convolution_kernel_sharpen.end());
    static const std::vector<float> edge_detect(convolution_kernel_edge_detect.begin(), convolution_kernel_edge_detect.end());
    static const std::vector<float> emboss(convolution_kernel_emboss.begin(), convolution_kernel_emboss.end());
    static const std::vector<float> blur(convolution_kernel_blur.begin(), convolution_kernel_blur.end());

    switch (kernelType)
    {
    case ConvolutionKernel::identity:
        return identity;
    case ConvolutionKernel::sharpen:
        return sharpen;
    case ConvolutionKernel::edge_detect:
        return edge_detect;
    case ConvolutionKernel::emboss:
        return emboss;
    case ConvolutionKernel::blur:
        return blur;
    default:
        return identity;
    }
}
