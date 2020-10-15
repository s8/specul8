//
//  spotFollower.hpp
//  shader_fbo
//
//  Created by Konstantin Leonenko on 15/10/2020.
//

#ifndef spotFollower_hpp
#define spotFollower_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxOsc.h"


class SpotFollower : public ofxCv::RectFollower{
    public:
    void setup();
    void update();
    void kill();
};


#endif /* spotFollower_hpp */
