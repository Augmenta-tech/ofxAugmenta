#pragma once

#include "ofMain.h"
#include "ofxAugmenta.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        Augmenta::Receiver AugmentaReceiver;
};
