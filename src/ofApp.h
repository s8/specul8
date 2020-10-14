#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxOsc.h"

using namespace glm;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void setUniforms();
    
    ofxPanel gui;
    ofParameter <ofVec3f> uiPosition;
    
    ofFbo shaderFbo;

    ofLight light;
    
    ofEasyCam camera;
    ofVboMesh bunny;
    
    ofShader surfaceShader;
    ofFloatColor matAmbient;
    ofFloatColor matDiffuse;
    ofFloatColor matSpecular;
    float matShininess;
    vec4 lightPosition;
    ofFloatColor lightColor;
    
    
    //
    // ofxCV setup
    //
    
//    ofVideoGrabber webcam;
    ofxCv::ContourFinder contour;
    ofxCv::RectTracker tracker;
    
//    ofxPanel gui;
    ofParameter<float> min, max, threshold;
    ofParameter<bool> hole;
    ofParameter<bool> boxes;
    
    ofPixels fboPixels;
    
    ofxOscSender sender;
    ofxOscBundle bundle;
    ofxOscMessage message;
    
		
};
