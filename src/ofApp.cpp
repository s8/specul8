#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

const float dyingTime = 1;

//--------------------------------------------------------------
void SpotFollower::setup(const cv::Rect& track){
    color.setHsb(ofRandom(0,255),255,255);
    cur = toOf(track).getCenter();
    smooth = cur;
}

//--------------------------------------------------------------
void SpotFollower::update(const cv::Rect& track){
    cur = toOf(track).getCenter();
    smooth.interpolate(cur, .5);
    all.addVertex(smooth);
}

//--------------------------------------------------------------
void SpotFollower::kill(){
    float curTime = ofGetElapsedTimef();
    if (startedDying == 0){
        startedDying = curTime;
    } else if (curTime - startedDying > dyingTime) {
        dead = true;
    }
}

//--------------------------------------------------------------
void SpotFollower::draw(const ofPolyline& p){
    ofPushStyle();
    float size = 16;
    ofSetColor(255);
    if (startedDying){
        ofSetColor(ofColor::red);
        size = ofMap(ofGetElapsedTimef()-startedDying, 0, dyingTime, size, 0, true);
    }
    ofNoFill();
    ofDrawCircle(cur,size);
    ofSetColor(color);
    all.draw();
    ofSetColor(255);
    ofDrawBitmapString(ofToString(label), cur);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::setup(){
    
//
// ofxCV setup
//
    gui.setup();
    gui.add(min.set("min", 30.0, 0.0, 300.0));
    gui.add(max.set("max", 3000.0, 0.0, 50000.0));
    gui.add(threshold.set("threshold", 100.0, 0.0, 200.0));
    gui.add(hole.set("hole", false));
    gui.add(boxes.set("boxes", false));

    contourFinder.getTracker().setPersistence(15);
    contourFinder.getTracker().setMaximumDistance(32);

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

//    ofDisableArbTex();
//    ofEnableNormalizedTexCoords();
    
    sender.setup(HOST, PORT);
        
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // bind light to the camera
    lightPosition = vec4(camera.getPosition(),1.0);
    contourFinder.setMinArea(min);
    contourFinder.setMaxArea(max);
    contourFinder.setThreshold(threshold);
    contourFinder.setFindHoles(hole);
    
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

    // put the contents of an FBO into pixels and load them into contourfinder
    shaderFbo.readToPixels(fboPixels);
    
    contourFinder.findContours(fboPixels);
//    tracker.track();
//    tracker = contourFinder.getTracker();
    tracker.track(contourFinder.getBoundingRects());
    
    }


//--------------------------------------------------------------
void ofApp::draw(){
    
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
                    bunny.draw();
                surfaceShader.end();
                ofDrawGrid(2.0,10, false, false, true, false);
            light.disable();
        camera.end();
        
    ofDisableDepthTest();
    
//    shaderFbo.draw(0,0, ofGetWidth()/4,ofGetHeight()/4);
    
    ofSetColor(ofColor::white);
    
    
//    for(int i = 0; i < contourFinder.size(); i++){
//
//        ofPoint center = toOf(contourFinder.getCenter(i));
//        int label = contourFinder.getLabel(i);
//        if (boxes){
//            contourFinder.draw();
//            ofDrawBitmapString(ofToString(label), ofGetWidth()-50, (i+1)*20);
//            ofPushMatrix();
//                ofTranslate(center.x, center.y);
//                string info = ofToString(label) + ":" + ofToString(tracker.getAge(label));
//                ofDrawBitmapString(info,0,0);
//                ofVec2f velocity = toOf(contourFinder.getVelocity(i));
//                ofScale(5, 5);
//                ofDrawLine(0, 0, velocity.x, velocity.y);
//            ofPopMatrix();
//        }
//    }
    
    vector<SpotFollower>& followers = tracker.getFollowers();
    for(int i = 0; i < followers.size(); i++) {
//        ofPolyline p = contourFinder.getPolyline(i);
        followers[i].draw(contourFinder.getPolyline(i));
    }
    
    
    for (ofPolyline p: contourFinder.getPolylines()){
        string p_info;

        message.clear();
        message.setAddress("/perimeter");
        message.addIntArg(p.getPerimeter());

        p_info = to_string(p.getPerimeter()/p.getArea());

        if (boxes){
            contourFinder.draw();
            ofDrawBitmapString(p_info, p.getCentroid2D());
        }

    };

//    sender.sendMessage(message);
//
//    sender.sendBundle(bundle);
//    bundle.clear();
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
