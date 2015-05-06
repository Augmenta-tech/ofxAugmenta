#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    receiver.connect( 12000 );
    
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
    vector<Augmenta::Person*> people = receiver.getPeople();
    for ( int i=0; i<people.size(); i++){
        // do stuff for each person!
        //people[i]->contour...
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    // debug draw!	
    receiver.draw(ofGetWidth(), ofGetHeight());
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

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

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