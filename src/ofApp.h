#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxOsc.h"
//#include "spotFollower.hpp"

// send host (aka ip address)
#define HOST "127.0.0.1"

/// send port
#define PORT 57120


using namespace glm;


class SpotFollower : public ofxCv::RectFollower{
private:
    ofxOscMessage m;
    ofColor color;
    ofVec3f cur, smooth;
    float startedDying;
    ofPolyline all;
public:
    SpotFollower()
        :startedDying(0){
    }
    void setup(const cv::Rect& track) override;
    void update(const cv::Rect& track) override;
    void kill() override;
    void draw(const ofPolyline& p);
    
    const ofxOscMessage& getMessage() const;
//    void draw();
    
    
};

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
    
    //
    // FBO
    //
    
    ofFbo shaderFbo;
    ofPixels fboPixels;

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
    // openCV
    //
    
    ofxCv::ContourFinder contourFinder;
    ofxCv::RectTrackerFollower<SpotFollower> tracker;
    vector<SpotFollower> followers;
    vector<ofPolyline> polylines;
    
    //
    // GUI
    //
    ofxPanel gui;
    ofParameter <ofVec3f> uiPosition;
    ofParameter<float> min, max, threshold;
    ofParameter<bool> hole;
    ofParameter<bool> boxes;
    
    
    //
    // OSC
    //
    ofxOscSender sender;
    ofxOscBundle bundle;
    ofxOscMessage message;

    
		
};
