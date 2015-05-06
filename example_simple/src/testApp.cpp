#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    auReceiver.connect( 12000 );
    
    // this will add listeners to your app; this is optional!
    // you will need to add all three of these functions to do this:
    // void onPersonEntered( Augmenta::EventArgs & tspsEvent );
    // void onPersonUpdated( Augmenta::EventArgs & tspsEvent );
    // void onPersonWillLeave( Augmenta::EventArgs & tspsEvent );
    
    ofxAddAugmentaListeners(this);
}

//--------------------------------------------------------------
void testApp::update(){
    // here, you can loop through each person and do stuff
    vector<Augmenta::Person*> people = auReceiver.getPeople();
    for ( int i=0; i<people.size(); i++){
        // do stuff for each person!
        //people[i]->contour...
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    // debug draw!	
    auReceiver.draw(ofGetWidth(), ofGetHeight());
    if(debug){
        auReceiver.interactiveArea.draw();
    }
}

//--------------------------------------------------------------
void testApp::onPersonEntered( Augmenta::EventArgs & augmentaEvent ){
    ofLog(OF_LOG_NOTICE, "New person!");
    // you can access the person like this:
    // tspsEvent.person
}

//--------------------------------------------------------------
void testApp::onPersonUpdated( Augmenta::EventArgs & augmentaEvent ){
    ofLog(OF_LOG_NOTICE, "Person updated!");
    // you can access the person like this:
    // tspsEvent.person
    
}

//--------------------------------------------------------------
void testApp::onPersonWillLeave( Augmenta::EventArgs & augmentaEvent ){
    ofLog(OF_LOG_NOTICE, "Person left!");
    // you can access the person like this:
    // tspsEvent.person
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    //std::cout << key << std::endl;
    if(key == 100){
        debug = !debug;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if (debug){
        if (button == 0){
            float w = (float)x/(float)ofGetWidth()-originX;
            float h = (float)y/(float)ofGetHeight()-originY;
            if (w > 0 && h > 0){
                auReceiver.interactiveArea.set(originX, originY, w, h);
            } else if (w < 0 && h > 0){
                auReceiver.interactiveArea.set((float)x/(float)ofGetWidth(), originY, -w, h);
            } else if (h < 0 && w > 0){
                auReceiver.interactiveArea.set(originX, (float)y/(float)ofGetHeight(), w, -h);
            } else {
                auReceiver.interactiveArea.set((float)x/(float)ofGetWidth(), (float)y/(float)ofGetHeight(), -w, -h);
            }
            //std::cout << "Rect : " << auReceiver.interactiveArea.area.x << " " << auReceiver.interactiveArea.area.y << " " << auReceiver.interactiveArea.area.x+auReceiver.interactiveArea.area.width << " " << auReceiver.interactiveArea.area.y+auReceiver.interactiveArea.area.height << std::endl;
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (debug){
        if (button == 0){
            originX = (float)x/(float)ofGetWidth();
            originY = (float)y/(float)ofGetHeight();
        } else {
            auReceiver.interactiveArea.set(0.0f, 0.0f, 1.0f, 1.0f);
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}