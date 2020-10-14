#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //
    // lighting gui setup
    //
    gui.setup();
//    gui.add(uiPosition.set("position",ofVec3f(0,0,0),ofVec3f(-30,-30,-30),ofVec3f(30,30,30)));
    
//
// ofxCV setup
//
//    webcam.setup(640,480);
    gui.setup();
    gui.add(min.set("min", 30.0, 0.0, 300.0));
    gui.add(max.set("max", 3000.0, 0.0, 50000.0));
    gui.add(threshold.set("threshold", 100.0, 0.0, 200.0));
    gui.add(hole.set("hole", false));
    gui.add(boxes.set("boxes", false));
//

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
        
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // fix light in a permanent spot
    // light.setPosition(uiPosition->x, uiPosition->y,uiPosition->z);
    
    // control light with UI
    // lightPosition = vec4(uiPosition->x, uiPosition->y,uiPosition->z, 1.0);
    
    // bind light to the camera
    lightPosition = vec4(camera.getPosition(),1.0);
        
        
    //
    // ofxCV basic updates
    //
    //    webcam.update();
    //    if (webcam.isFrameNew()){
//            contour.setMinArea(min);
//            contour.setMaxArea(max);
//            contour.setThreshold(threshold);
//            contour.setFindHoles(hole);
//            contour.findContours(webcam);
    //  }

    }

    //
    // basic ofxCV webcam draw function
    //
    //
    //void ofApp::draw(){
    //    webcam.draw(0,0);
    //    contour.draw();
    //    gui.draw();
    //}

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
    
    contour.setMinArea(min);
    contour.setMaxArea(max);
    contour.setThreshold(threshold);
    contour.setFindHoles(hole);
    
    contour.findContours(fboPixels);
    
    if (boxes){
        contour.draw();
    }
    
//    shaderFbo.draw(0,0, ofGetWidth()/4,ofGetHeight()/4);
        
    ofDisableDepthTest();
    
    gui.draw();
    
    
    ofSetColor(ofColor::white);
    
    int i = 1;
//    for (cv::Rect rect: contour.getBoundingRects()){
//        string rect_info;
////        rect_info = to_string(rect.area()) + " " + to_string(rect.height);
//        rect_info = to_string(rect.area());
//        ofDrawBitmapString(rect_info, rect.x, rect.y-5);
//        i++;
//    };
    
    for (ofPolyline p: contour.getPolylines()){
        string p_info;
        p_info = to_string((int)p.getArea()) + " " + to_string((int)p.getPerimeter());
        ofDrawBitmapString(p_info, p.getCentroid2D());
        i++;
    };
    
    
    
    

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
