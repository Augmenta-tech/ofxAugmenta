#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    AugmentaReceiver.connect(12000);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(ofColor::black);
    
    vector<Augmenta::Person*> people = AugmentaReceiver.getPeople();
    
    for (int i=0; i<people.size(); i++){
        
        ofPoint centroid = people[i]->centroid;
        centroid.x *= ofGetWidth();
        centroid.y *= ofGetHeight();
        
        ofCircle(centroid, 10);
    }
}
