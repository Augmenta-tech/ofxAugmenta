#include "testApp.h"

#define OSC_PORT 12000

//--------------------------------------------------------------
void testApp::setup(){
    
    // Init
    
    m_bDebug = false;
    m_MouseClick.x = 0.0f;
    m_MouseClick.y = 0.0f;
    
    ofBackground(ofColor::black);
    m_auReceiver.connect(OSC_PORT);
    
    ofxAddAugmentaListeners(this);
}

//--------------------------------------------------------------
void testApp::update(){
    
    // Get the person data
    vector<Augmenta::Person*> people = m_auReceiver.getPeople();
    
    // For each person...
    for (int i=0; i<people.size(); i++){

        // You can do stuff here :)
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // Get the person data
    vector<Augmenta::Person*> people = m_auReceiver.getPeople();
    
    // You can draw stuff here !
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(4);
    ofSetColor(ofColor::blue);
    for(int i=0; i<people.size(); ++i) {
        ofCircle(people[i]->centroid.x* ofGetWidth(), people[i]->centroid.y* ofGetHeight(), 15);
    }
    ofPopStyle();
    
    // Draw the interactive area
    if(m_bDebug){
        
        ofDrawBitmapString("[drag mouse] to set interactive area\n[right click] to reset", ofPoint(10,10));
        
        // Draw debug data
        m_auReceiver.getInteractiveArea()->draw();
        m_auReceiver.draw(ofGetWidth(), ofGetHeight());

    } else {
        ofDrawBitmapString("[d] to show debug", ofPoint(10,10));
    }
}

//--------------------------------------------------------------
void testApp::onPersonEntered( Augmenta::EventArgs & augmentaEvent ){
    
    ofLog(OF_LOG_NOTICE, "New person!");
    
    // You can access the person like this :
    // augmentaEvent.person
}

//--------------------------------------------------------------
void testApp::onPersonUpdated( Augmenta::EventArgs & augmentaEvent ){
    
    ofLog(OF_LOG_NOTICE, "Person updated!");
    
    // You can access the person like this :
    // augmentaEvent.person
}

//--------------------------------------------------------------
void testApp::onPersonWillLeave( Augmenta::EventArgs & augmentaEvent ){
    
    ofLog(OF_LOG_NOTICE, "Person left!");

    // You can access the person like this :
    // augmentaEvent.person
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key == 'd' || key == 'D'){
        
        m_bDebug = !m_bDebug;
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
void testApp::mousePressed(int x, int y, int button){

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
