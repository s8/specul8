#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    //
    // lighting gui setup
    //
//    gui.setup();
//    gui.add(uiPosition.set("position",ofVec3f(0,0,0),ofVec3f(-30,-30,-30),ofVec3f(30,30,30)));
    
//
// ofxCV setup
//
    gui.setup();
    gui.add(min.set("min", 30.0, 0.0, 300.0));
    gui.add(max.set("max", 3000.0, 0.0, 50000.0));
    gui.add(threshold.set("threshold", 100.0, 0.0, 200.0));
    gui.add(hole.set("hole", false));
    gui.add(boxes.set("boxes", false));

    contour.getTracker().setPersistence(15);
    contour.getTracker().setMaximumDistance(32);

    shaderFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
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
    
    sender.setup(HOST, PORT);
        
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // fix light in a permanent spot
    // light.setPosition(uiPosition->x, uiPosition->y,uiPosition->z);
    
    // control light with UI
    // lightPosition = vec4(uiPosition->x, uiPosition->y,uiPosition->z, 1.0);
    
    // bind light to the camera
    lightPosition = vec4(camera.getPosition(),1.0);
    contour.setMinArea(min);
    contour.setMaxArea(max);
    contour.setThreshold(threshold);
    contour.setFindHoles(hole);

    }


//--------------------------------------------------------------
void ofApp::draw(){
//
// Render into an FBO
//
    shaderFbo.begin();
        ofEnableDepthTest();
            light.enable();
                camera.begin();
                    surfaceShader.begin();

                        matAmbient = ofFloatColor(0.01,0.01,0.0);
                        matDiffuse = ofFloatColor(0.01,0.01,0.01);
                        matSpecular = ofFloatColor(5.0,5.0,5.0);
                        setUniforms();
    
                        ofClear(ofFloatColor(0.1f,0.2f,0.3f));
                        bunny.draw();

                    surfaceShader.end();
                camera.end();
            light.disable();
        ofDisableDepthTest();
    shaderFbo.end();

//
// Render into the frame buffer
//
    
    ofEnableDepthTest();
        camera.begin();
            light.enable();
                surfaceShader.begin();
                    matAmbient = ofFloatColor(0.1,0.1,0.2);
                    matDiffuse = ofFloatColor(0.1,0.1,0.1);
                    matSpecular = ofFloatColor(1.0,1.0,0.0);
                    setUniforms();
    //                surfaceShader.setUniformTexture("specularTexture", shaderFbo.getTexture(),1);
                    bunny.draw();
                surfaceShader.end();
                ofDrawGrid(2.0,10, false, false, true, false);
            light.disable();
        camera.end();
    shaderFbo.readToPixels(fboPixels);
    
    contour.findContours(fboPixels);
    tracker = contour.getTracker();
    
//    shaderFbo.draw(0,0, ofGetWidth()/4,ofGetHeight()/4);
        
    ofDisableDepthTest();
    
    ofSetColor(ofColor::white);
    
    
    for(int i = 0; i < contour.size(); i++){
        
        ofPoint center = toOf(contour.getCenter(i));
        int label = contour.getLabel(i);
        
        float pan = (center.x / ofGetWidth() * 2) - 1;
        
        message.clear();
        message.setAddress("/pan");
        message.addFloatArg(pan);
//        bundle.addMessage(message);
//        message.clear();
        message.setAddress("/perimeter");
        message.addFloatArg(contour.getPolyline(i).getPerimeter());
        bundle.addMessage(message);
        
        ofDrawBitmapString(ofToString(label), ofGetWidth()-50, (i+1)*20);
        
        
        ofPushMatrix();
            ofTranslate(center.x, center.y);
            
            string info = ofToString(label) + ":" + ofToString(pan) + ":" + ofToString(tracker.getAge(label));
            ofDrawBitmapString(info,0,0);
            ofVec2f velocity = toOf(contour.getVelocity(i));
            ofScale(5, 5);
            ofDrawLine(0, 0, velocity.x, velocity.y);
        ofPopMatrix();
    }
    
    
//    for (ofPolyline p: contour.getPolylines()){
//        string p_info;
//
//        message.clear();
//        message.setAddress("/perimeter");
//        message.addIntArg(p.getPerimeter());
//
//        p_info = to_string(p.getPerimeter()/p.getArea());
//
//        if (boxes){
//            contour.draw();
//            ofDrawBitmapString(p_info, p.getCentroid2D());
//        }
//
//    };

//    sender.sendMessage(message);
    
    sender.sendBundle(bundle);
    bundle.clear();
    gui.draw();
    

}

//--------------------------------------------------------------
void ofApp::setUniforms(){
    surfaceShader.setUniform4f("matAmbient", matAmbient);
    surfaceShader.setUniform4f("matDiffuse", matDiffuse);
    surfaceShader.setUniform4f("matSpecular", matSpecular);
    surfaceShader.setUniform1f("matShininess", matShininess);
    surfaceShader.setUniform4f("lightPosition", lightPosition);
    surfaceShader.setUniform4f("lightColor", lightColor);
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
