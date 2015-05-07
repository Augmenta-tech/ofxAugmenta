#pragma once

#include "ofMain.h"
#include "ofxTSPSReceiver.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        ofxTSPS::Receiver tspsReceiver;
};
