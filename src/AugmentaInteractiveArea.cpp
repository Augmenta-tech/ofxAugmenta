//
//  AugmentaInteractiveArea.cpp
//  Augmenta_2D
//
//  Created by Theoriz-MacMini on 5/5/15.
//
//

#include "AugmentaInteractiveArea.h"

namespace Augmenta {
    
    InteractiveArea::InteractiveArea(){
        area = ofRectangle(0.0f, 0.0f, 1.0f, 1.0f);
    }
    
    void InteractiveArea::set(float x, float y, float width, float height){
        area = ofRectangle(x, y, width, height);
    }
    
    bool InteractiveArea::contains(ofPoint p){
        return (p.x > area.x && p.y > area.y && p.x < area.x+area.width && p.y < area.y+area.height);
    }
    
    /** Draw a debug view */
    void InteractiveArea::draw(){
        //draw contours
        ofPushStyle();
        ofNoFill();
        ofSetColor(255, 0, 0, 100);
        ofSetLineWidth(2);
        ofRect(area.x*ofGetWidth(), area.y*ofGetHeight(), area.width*ofGetWidth(), area.height*ofGetHeight());
        ofPopStyle();
    }
    
}