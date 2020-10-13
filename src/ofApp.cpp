#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    gui.setup();
    gui.add(uiPosition.set("position",ofVec3f(0,0,0),ofVec3f(-30,-30,-30),ofVec3f(30,30,30)));

    shaderFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    shaderFbo.begin();
    ofClear(ofFloatColor(0.1f,0.2f,0.3f));
    shaderFbo.end();

    ofBackground (0);
    camera.setPosition(vec3(0.0,8.0,24.0));
    camera.setTarget(vec3(0.0,4.0,0.0));
    camera.setFov(32.0);
    camera.setNearClip(0.05);
    camera.setFarClip(500.0);
    camera.setAutoDistance(false);

    bunny.load("bunny.ply");

    surfaceShader.load("simplePhong");
    matAmbient = ofFloatColor(0.1,0.1,0.2);
    matDiffuse = ofFloatColor(0.1,0.1,0.1);
    matSpecular = ofFloatColor(3.5,4.0,5.0);
    matShininess = 500.0;
    lightPosition = vec4(10.0,10.0,20.0,1.0);
    lightColor = ofFloatColor(1.0,1.0,0.8);
//    matReflectivity = 0.7;

//    ofDisableArbTex();
//    ofEnableNormalizedTexCoords();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // fix light in a permanent spot
//    light.setPosition(uiPosition->x, uiPosition->y,uiPosition->z);
    // control light with UI
//    lightPosition = vec4(uiPosition->x, uiPosition->y,uiPosition->z, 1.0);
    // bind light to the camera
    lightPosition = vec4(camera.getPosition(),1.0);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableDepthTest();
    light.enable();
    camera.begin();
    
//    ofDrawBox(0,0,0,128);
    
    //
    // Render into an FBO
    //
    
//    shaderFbo.begin();
    
    surfaceShader.begin();
    
//    ofDrawSphere(uiPosition->x, uiPosition->y,uiPosition->z,2);
    
//    matAmbient = ofFloatColor(0.1,0.9,0.0);
//    matDiffuse = ofFloatColor(0.1,0.1,0.1);
//    matSpecular = ofFloatColor(3.5,4.0,5.0);
//
//    surfaceShader.setUniform4f("matAmbient", matAmbient);
//    surfaceShader.setUniform4f("matDiffuse", matDiffuse);
//    surfaceShader.setUniform4f("matSpecular", matSpecular);
//    surfaceShader.setUniform1f("matShininess", matShininess);
//    surfaceShader.setUniform4f("lightPosition", lightPosition);
//    surfaceShader.setUniform4f("lightColor", lightColor);
//    
//    surfaceShader.setUniformTexture("specularTexture", shaderFbo.getTexture(),1);
//
//    bunny.draw();
//
//    surfaceShader.end();
//
//    shaderFbo.end();
    
//    shaderFbo.draw(0,0);
    
    //
    // Render into the frame buffer
    //
    
    surfaceShader.begin();
    
//    ofDrawSphere(uiPosition->x, uiPosition->y,uiPosition->z,2);
    
    matAmbient = ofFloatColor(0.1,0.1,0.2);
    matDiffuse = ofFloatColor(0.1,0.1,0.1);
    matSpecular = ofFloatColor(15.5,15.0,0.0);


    surfaceShader.setUniform4f("matAmbient", matAmbient);
    surfaceShader.setUniform4f("matDiffuse", matDiffuse);
    surfaceShader.setUniform4f("matSpecular", matSpecular);
    surfaceShader.setUniform1f("matShininess", matShininess);
    surfaceShader.setUniform4f("lightPosition", lightPosition);
    surfaceShader.setUniform4f("lightColor", lightColor);
    
    surfaceShader.setUniformTexture("specularTexture", shaderFbo.getTexture(),1);

    bunny.draw();
    
    surfaceShader.end();
    
    
    
    
    
    
    
    
    ofDrawGrid(2.0,10, false, false, true, false);

    camera.end();
    
    light.disable();
    ofDisableDepthTest();
    
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
