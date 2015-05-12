#include "ofApp.h"

#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    
    try {
        AugmentaReceiver.connect(12000);
    } catch (std::exception&e) {
        std::cerr << "Error : " << e.what() << endl;
    }
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
