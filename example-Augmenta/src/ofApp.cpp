#include "ofApp.h"

#include <iostream>

#define OSC_PORT 12000

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Init
    
    m_bDebug = false;
    m_MouseClick.x = 0.0f;
    m_MouseClick.y = 0.0f;
    m_iPort = OSC_PORT;
    
    ofBackground(ofColor::black);
    
    m_auReceiver.connect(m_iPort);
    
    ofxAddAugmentaListeners(this);
    
    ofEnableAlphaBlending();
    ofSetCircleResolution(60);
    ofEnableBlendMode(OF_BLENDMODE_ADD);

#ifdef __APPLE__
    m_oSyphonServer.setName("Augmenta");
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Get the person data
    vector<Augmenta::Person*> people = m_auReceiver.getPeople();
    
    // For each person...
    for (int i=0; i<people.size(); i++){

        ////////////////////////////////////////////////////////
        // You can do stuff here :)
        ////////////////////////////////////////////////////////
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // Get the person data
    vector<Augmenta::Person*> people = m_auReceiver.getPeople();
    
    ////////////////////////////////////////////////////////
    
    // You can draw stuff here !
    
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(3);
    ofSetColor(ofColor::blue);
    for(int i=0; i<people.size(); ++i) {
        ofDrawCircle(people[i]->centroid.x* ofGetWidth(), people[i]->centroid.y* ofGetHeight(), 15);
    }
    ofPopStyle();
    
    ////////////////////////////////////////////////////////
    
    // Draw the interactive area
    if(m_bDebug){
        
        // Draw debug data
        m_auReceiver.getInteractiveArea()->draw();
        m_auReceiver.draw(ofGetWidth(), ofGetHeight());

#ifdef __APPLE__
        m_oSyphonServer.publishScreen();
#endif
        
        ofDrawBitmapString("[drag mouse] to set interactive area\n[right click] to reset", 10,20);

    } else {

#ifdef __APPLE__
        m_oSyphonServer.publishScreen();
#endif
        
        ofPushStyle();
        ofDrawBitmapString("OSC port : \n[d] to show debug", 10, 20);
        if(m_auReceiver.isConnected()){
            ofSetColor(ofColor::green);
        } else {
            ofSetColor(ofColor::red);
        }
        ofDrawBitmapString(ofToString(m_iPort), 100, 20);
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::onPersonEntered( Augmenta::EventArgs & augmentaEvent ){
    
    ofLog(OF_LOG_NOTICE, "New person!");
    
    // You can access the person like this :
    // augmentaEvent.person
}

//--------------------------------------------------------------
void ofApp::onPersonUpdated( Augmenta::EventArgs & augmentaEvent ){
    
    //ofLog(OF_LOG_NOTICE, "Person updated!");
    
    // You can access the person like this :
    // augmentaEvent.person
}

//--------------------------------------------------------------
void ofApp::onPersonWillLeave( Augmenta::EventArgs & augmentaEvent ){
    
    ofLog(OF_LOG_NOTICE, "Person left!");

    // You can access the person like this :
    // augmentaEvent.person
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == 'd' || key == 'D'){
        
        m_bDebug = !m_bDebug;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if (m_bDebug){
        
        // Set interactive area size
        if (button == 0){
            float w = (float)x/(float)ofGetWidth()-m_MouseClick.x;
            float h = (float)y/(float)ofGetHeight()-m_MouseClick.y;
            
            if (w > 0 && h > 0){
                m_auReceiver.getInteractiveArea()->set(m_MouseClick.x, m_MouseClick.y, w, h);
            } else if (w < 0 && h > 0){
                m_auReceiver.getInteractiveArea()->set((float)x/(float)ofGetWidth(), m_MouseClick.y, -w, h);
            } else if (h < 0 && w > 0){
                m_auReceiver.getInteractiveArea()->set(m_MouseClick.x, (float)y/(float)ofGetHeight(), w, -h);
            } else {
                m_auReceiver.getInteractiveArea()->set((float)x/(float)ofGetWidth(), (float)y/(float)ofGetHeight(), -w, -h);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    if (m_bDebug){
        
        // Left Click
        if (button == OF_MOUSE_BUTTON_1){
            
            // Save origin mouse click
            m_MouseClick.x = (float)x/(float)ofGetWidth();
            m_MouseClick.y = (float)y/(float)ofGetHeight();
        } else {

            // Reset interactive area to full window
            m_auReceiver.getInteractiveArea()->set(0.0f, 0.0f, 1.0f, 1.0f);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
