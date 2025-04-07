#include "custom_scene_controller.h"

void CustomSceneController::setup()
{

    light.setup();
    light.setPosition(0, 300, 300);
    light.enable();

    // Load ton modèle
    ant.loadModel("models/sci-fiAnt/sci_fi_ant_unit.glb");
    ofLog() << "Has texcoords: " << ant.getMesh(0).hasTexCoords();

    shader.load("custom_ant_330_vs.glsl", "custom_ant_330_fs.glsl");
    
    antTexture = ant.getTextureForMesh(0);
    
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

   
    poster.set(50, 50);

    poster.setPosition(0, 0, ( - boxSize / 2) +1); 
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
    defaultAnt.setName("Default ant color");
    gui.add(defaultAnt);
    defaultAnt.addListener(this, &CustomSceneController::onDefaultSelect);
    antTint.set("Ant tint", ofColor(2500), ofColor(0, 0), ofColor(255, 255));
    gui.add(antTint);
    antTint.addListener(this, &CustomSceneController::onColorChanged);
    antColor.set(0);

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
    if (colorChanged) defaultAnt = false;
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
    ofSetColor(255); // Reset couleur

    if (!defaultAnt) {
       
        ant.disableMaterials();
        shader.begin();
        shader.setUniformTexture("tex0", antTexture, 0);
        shader.setUniform4f("tintColor", antColor);
        ant.drawFaces();
        shader.end();
    }
    else {
        ant.enableMaterials();
        ant.drawFaces();
    }
    
    
    cam.end();

    ofDisableDepthTest(); 
    gui.draw();


}
void CustomSceneController::onColorChanged(ofColor& color)
{
    colorChanged = true;
    antColor = color;
}
void CustomSceneController::onDefaultSelect(bool& value)
{
    defaultAnt = value;

    if (value) colorChanged = false;
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







