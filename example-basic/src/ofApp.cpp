#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    tspsReceiver.connect(12000);
    ofEnableAlphaBlending();
    ofSetCircleResolution(60);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    vector<ofxTSPS::Person*> people = tspsReceiver.getPeople();
    
    for (int i=0; i<people.size(); i++){
        ofSetColor( min(255.0f, 150.0f + people[i]->age / 60.0f), 100 );
        
        // since all properties are normalized, must scale them up to screen
        ofPoint centroid = people[i]->centroid;
        centroid.x *= ofGetWidth();
        centroid.y *= ofGetHeight();
        
        float width = people[i]->boundingRect.width * ofGetWidth();
        
        ofCircle(centroid, width);
    }
}