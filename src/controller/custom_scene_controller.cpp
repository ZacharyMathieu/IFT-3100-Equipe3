#include "custom_scene_controller.h"

void CustomSceneController::setup()
{
    ofEnableLighting();
    ofSetFrameRate(60);
    light.setup();
    light.setSpotlight();
    light.setSpotlightCutOff(25);
    light.setSpotConcentration(20);
    light.setDiffuseColor(ofFloatColor(0.8, 0.8, 0.8));   // Lumière blanche douce
    light.setSpecularColor(ofFloatColor(0.1, 0.1, 0.1));  // Reflet doux
    light.setAmbientColor(ofFloatColor(0.05));

    cadre.load("images/cadre.png");
    

    // Load ton modèle
    ant.loadModel("models/sci-fiAnt/ant-SciFi.gltf");
    ofLog() << "Has texcoords: " << ant.getMesh(0).hasTexCoords();
    

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

    img.load("images/glitter.jpg");

    imgTexture = img.getTexture();

    imgPlateform.load("images/wood.jpg");

    texPlateform = imgPlateform.getTexture();

    ant.setPosition(0, -45, 0); // Légèrement au-dessus du sol

    // Crée les plans
    float boxSize = 100;
    float wallThickness = 1.0;

    // Sol
    floor.set(boxSize, boxSize);
    floor.rotateDeg(90, 1, 0, 0);
    floor.setPosition(0, -50, 0);

    // Plafond
    ceiling.set(boxSize, boxSize);
    ceiling.rotateDeg(90, 1, 0, 0);
    ceiling.setPosition(0, 50, 0);

    // Mur arrière
    backWall.set(boxSize, boxSize);
    backWall.setPosition(0, 0, -boxSize / 2);

    cadrePlane.set(100, 70);
    cadrePlane.setPosition(0, 0, (-boxSize / 2) + 1);
    cadrePlane.rotateDeg(180, 0, 1, 0);

    poster.set(62, 47);

    poster.setPosition(0, -1, ( - boxSize / 2) +2); 
    poster.rotateDeg(180, 0, 1, 0); 

    // Mur gauche
    leftWall.set(boxSize+20, boxSize);
    leftWall.rotateDeg(90, 0, 1, 0);
    leftWall.setPosition(-boxSize / 2, 0, 0);

    // Mur droit
    rightWall.set(boxSize+20, boxSize);
    rightWall.rotateDeg(90, 0, 1, 0);
    rightWall.setPosition(boxSize / 2, 0, 0);

    plateform.set(40, 5);
    plateform.setResolution(64, 1);
    plateform.setPosition(0, -48, 0);

    // Caméra
    cam.setDistance(100);
    cam.lookAt(glm::vec3(0, -10, 0));

    gui.setup("Poster");
    posterChoice.setName("Back poster");
    posterChoice = false;
    gui.add(posterChoice);
    gui.setPosition(10, 10);
    //defaultAnt.setName("Default ant color");
   // gui.add(defaultAnt);
   // defaultAnt.addListener(this, &CustomSceneController::onDefaultSelect);
    blueTint.setName("Base tint");
    gui.add(blueTint);
    blueTint.addListener(this, &CustomSceneController::onBlueChanged);
    redTint.setName("Normal tint");
    gui.add(redTint);
    redTint.addListener(this, &CustomSceneController::onRedChanged);
    greenTint.setName("Metallic tint");
    gui.add(greenTint);
    greenTint.addListener(this, &CustomSceneController::onGreenChanged);
    doubleTint.setName("Roughness tint");
    gui.add(doubleTint);
    doubleTint.addListener(this, &CustomSceneController::onDoubleChanged);

    blueTint = true;
    redTint = false;
    greenTint = false;
    doubleTint = false;
    

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

	ant.setScale(0.15, 0.15, 0.15);
	ant.setRotation(0, -90, 1, 0, 0);
	ant.setRotation(1, newAngle, 0, 0, 1);
    plateform.setOrientation(glm::vec3(0, -newAngle, 0));

    if (posterChoice)
    {
        openPosterChoicer();
    }

    ant.update();
    ant.getAnimation(0).play();

    
    light.lookAt(ant.getPosition());

    light.setGlobalPosition(glm::vec3(0,40, 0));

}

void CustomSceneController::draw()
{
    gui.draw();
    ofEnableDepthTest();
    ofBackground(0);
    cam.begin();
    // Lumière
    light.enable();

    // Dessine les murs (avec couleurs distinctes)
    ofSetColor(180); // Sol gris clair
    floor.draw();

    ofSetColor(255); // Plafond blanc
    ceiling.draw();

    ofSetColor(200); // Mur arrière gris
    backWall.draw();

    cadre.getTexture().bind();
    cadrePlane.draw();
    cadre.getTexture().unbind();

    if (posterSet) {
        posterImg.getTexture().bind();
        poster.draw();
        posterImg.getTexture().unbind();
    }
    

    ofSetColor(255, 0, 0); // Mur gauche rouge
    leftWall.draw();

    ofSetColor(0, 255, 0); // Mur droit vert
    rightWall.draw();

    ofSetColor(100);
    texPlateform.bind();
    plateform.draw();
    texPlateform.unbind();
    // Modèle
       
  /* shader.begin();
   shader.setUniformTexture("baseColorMap", baseColorTexture, 0);
   shader.setUniformTexture("normalColorMap", normalMapTexture, 1);
   shader.setUniformTexture("metallicColorMap",metallicTexture, 2);
   shader.setUniformTexture("roughnessColorMap", roughnessTexture, 3);
    
    ant.drawFaces();
    shader.end();*/

    ant.drawFaces();
   
    ofPushMatrix();
    ofTranslate(light.getPosition());
    ofDrawAxis(20);
    ofPopMatrix();

    cam.end();

    ofDisableDepthTest(); 
    gui.draw();


}
void CustomSceneController::onBlueChanged(bool& value)
{
    antColor = value;
    if (value) {
        redTint = false;
        greenTint = false;
        doubleTint = false;
    }
    
}
void CustomSceneController::onRedChanged(bool& value)
{
    normalColor = value;
    if (value) {
         
        blueTint = false;
        greenTint = false;
        doubleTint = false;
    }
    
}
void CustomSceneController::onGreenChanged(bool& value)
{
    metallicColor = value;
    if (value) {
        metallicTexture = greenEye.getTexture();
        blueTint = false;
        redTint = false;
        doubleTint = false;
    }
    
}
void CustomSceneController::onDoubleChanged(bool& value)
{
    roughnessColor = value;
    if (value) {
        metallicTexture = doubleColorEye.getTexture();
        blueTint = false;
        redTint = false;
        greenTint = false;
    }
    
}
void CustomSceneController::openPosterChoicer()
{
    ofFileDialogResult result = ofSystemLoadDialog("Choisir une image");
    if (result.bSuccess) {
        posterImg.load(result.getPath());
        poster.mapTexCoordsFromTexture(posterImg.getTexture());
        posterSet = true;
    }
    posterChoice = false;
}







