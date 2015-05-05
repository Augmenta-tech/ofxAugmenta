#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    tspsReceiver.connect( 12000 );
    
    // this will add listeners to your app; this is optional!
    // you will need to add all three of these functions to do this:
    // void onPersonEntered( ofxTSPS::EventArgs & tspsEvent );
    // void onPersonUpdated( ofxTSPS::EventArgs & tspsEvent );
    // void onPersonWillLeave( ofxTSPS::EventArgs & tspsEvent );
    
    ofxAddTSPSListeners(this);
}

//--------------------------------------------------------------
void testApp::update(){
    // here, you can loop through each person and do stuff
    vector<ofxTSPS::Person*> people = tspsReceiver.getPeople();
    for ( int i=0; i<people.size(); i++){
        int personID = people[i]->pid;
        
        // do we have a trail here?
        if ( trails.count(personID) == 0 ){
            // add one if not
            // note: this isn't really necessary for a map, but is left here
            // for clarity's sake
            trails[personID] = ofPolyline();
        }
        
        // ignore highest point if it's < 0 (i.e. not set yet)
        if ( people[i]->highest.x < 0 ) continue;
        
        //ofPoint highest = people[i]->highest;
        
        // another option is to use the centroid!
        ofPoint highest = people[i]->centroid;
        
        // scale up highest point to screen
        highest.x *= ofGetWidth();
        highest.y *= ofGetHeight();
        
        // add highest point to trail
        trails[personID].addVertex( highest );
        
        // limit size of trails
        if ( trails[personID].size() > 500 ){
            vector<ofPoint> & verts = trails[personID].getVertices();
            verts.erase(verts.begin());
        }
    }
    
    // clean up trails if people have left. because ofEvents come in asynchronously,
    // you must either use an ofMutex and lock() in onPersonWillLeave OR just delete
    // them in update like so:
    
    map<int,ofPolyline>::iterator it = trails.begin();
    
    for (it; it != trails.end(); it++){
        if ( tspsReceiver.personWithID(it->first) == NULL ){
            // they're gone!
            trails.erase(it->first);
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    // draw trails
    map<int,ofPolyline>::iterator it = trails.begin();
    for (it; it != trails.end(); it++){
        it->second.draw();
    }
}

//--------------------------------------------------------------
void testApp::onPersonEntered( ofxTSPS::EventArgs & tspsEvent ){
    ofLog(OF_LOG_NOTICE, "New person!");
    // you can access the person like this:
    // tspsEvent.person
}

//--------------------------------------------------------------
void testApp::onPersonUpdated( ofxTSPS::EventArgs & tspsEvent ){
    ofLog(OF_LOG_NOTICE, "Person updated!");
    // you can access the person like this:
    // tspsEvent.person
    
}

//--------------------------------------------------------------
void testApp::onPersonWillLeave( ofxTSPS::EventArgs & tspsEvent ){
    ofLog(OF_LOG_NOTICE, "Person left!");
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