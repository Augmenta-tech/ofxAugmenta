#pragma once

#include "ofMain.h"
#include "AugmentaReceiver.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
        Augmenta::Receiver auReceiver;
        bool debug = false;
        float originX = 0.0f, originY = 0.0f;
    
        // event listeners
        void onPersonEntered( Augmenta::EventArgs & augmentaEvent );
        void onPersonUpdated( Augmenta::EventArgs & augmentaEvent );
        void onPersonWillLeave( Augmenta::EventArgs & augmentaEvent );
};
