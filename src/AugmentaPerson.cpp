/*
 *  Person.cpp
 *  openTSPS
 *
 */

#include "AugmentaPerson.h"
#define MAX_HAAR_GHOSTFRAMES 15 //how long before we say it's really gone

namespace Augmenta {
    //--------------------------------------------------------------
    Person::Person(int pid, int oid )
    : pid(pid),
    oid(oid),
    age(0),
    hasHaar(false),
    haarRect(ofRectangle(0,0,0,0)),
    opticalFlowVectorAccumulation(ofPoint(0,0)),
    centroid(0,0),
    framesWithoutHaar(0),
    customAttributes(NULL),
    depth(0),
    highest(-1,1),
    lowest(-1,-1)
    {
        update();
    }
    
    //--------------------------------------------------------------
    Person::~Person(){
        if(customAttributes != NULL){
            free(customAttributes);
        }
    }
    
    //--------------------------------------------------------------
    void Person::update(){
        age++;
    }
    
    //--------------------------------------------------------------
    void Person::updateBoundingRect(ofRectangle rect ){
        boundingRect.set(rect);
        area = boundingRect.width * boundingRect.height;
    }
    
    //--------------------------------------------------------------
    void Person::updateCentroid( ofPoint _centroid, bool dampen ){
        if(dampen){
            centroid = (centroid * .7) + (_centroid * .3);
        }
        else{
            centroid = _centroid;
        }
        
        velocity	 = _centroid - centroid;
    }
    
    //--------------------------------------------------------------
    void Person::draw( int cameraWidth, int cameraHeight, bool bSendContours, bool bSendHaar, float haarPadding ){
        //draw contours 
        ofPushStyle();
        ofNoFill();
        if (bSendContours){
            ofSetHexColor(0x3abb93);
        } else {
            ofSetHexColor(0xc4b68e);
        }
        ofBeginShape();
        for( int j=0; j<contour.size(); j++ ) {
            ofVertex( contour[j].x * cameraWidth, contour[j].y * cameraHeight );
        }
        ofEndShape();
        ofPopStyle();
        
        // draw haar    
        ofSetHexColor(0xffffff);							
        if(bSendHaar){
            ofSetHexColor(0xee3523);
            //draw haar search area expanded 
            //limit to within data box so it's not confusing				
            ofRectangle haarRect = ofRectangle(boundingRect.x - haarPadding, 
                                               boundingRect.y - haarPadding, 
                                               boundingRect.width  + haarPadding*2, 
                                               boundingRect.height + haarPadding*2);
            if (haarRect.x < 0){
                haarRect.width += haarRect.x;
                haarRect.x = 0;					
            }
            if (haarRect.y < 0){
                haarRect.height += haarRect.y;	
                haarRect.y = 0;
            }
            if (haarRect.x + haarRect.width > cameraWidth) haarRect.width = cameraWidth-haarRect.x;
            if (haarRect.y + haarRect.height > cameraHeight) haarRect.height = cameraHeight-haarRect.y;
            ofRect(haarRect.x* cameraWidth, haarRect.y* cameraHeight, haarRect.width* cameraWidth, haarRect.height* cameraHeight);
            
            if(hasHaarRect()){
                //draw the haar rect
                ofSetHexColor(0xee3523);
                ofRect(getHaarRect().x* cameraWidth, getHaarRect().y* cameraHeight, getHaarRect().width* cameraWidth, getHaarRect().height* cameraHeight);
                //haar-detected people get a red square
                ofSetHexColor(0xfd5f4f);
            } else {
                //no haar gets a yellow square
                ofSetHexColor(0xeeda00);
            }
        } else {
            //no haar gets a yellow square
            ofSetHexColor(0xeeda00);
        }
        
        //draw person
        ofRect(boundingRect.x* cameraWidth, boundingRect.y* cameraHeight, boundingRect.width* cameraWidth, boundingRect.height* cameraHeight);
        
        // draw highest point
        ofSetHexColor(0xff00ff);	
        ofCircle(highest.x* cameraWidth, highest.y* cameraHeight, 4);
        
        //draw centroid
        ofSetHexColor(0xff0000);
        ofCircle(centroid.x* cameraWidth, centroid.y* cameraHeight, 3);
        
        //draw id
        ofSetHexColor(0xffffff);
        char idstr[1024];
        sprintf(idstr, "pid: %d\noid: %d\nage: %d", pid, oid, age );
        ofDrawBitmapString(idstr, centroid.x* cameraWidth+8, centroid.y* cameraHeight);
    }
    
    //--------------------------------------------------------------
    void Person::updateContour(ofPolyline _contour){
        contour		 = _contour; 
        
        simpleContour = contour;    
        simpleContour.simplify(2.0f);
        float simplifyAmount = 2.5f;
        while (simpleContour.size() > 100){
            simpleContour.simplify(simplifyAmount);
            simplifyAmount += .5f;
        }
    }
    
    //--------------------------------------------------------------
    void Person::setHaarRect(ofRectangle _haarRect){
        haarRect = _haarRect;
        if ( haarRect.isEmpty() ){
            noHaarThisFrame();
        } else {
            hasHaar = true;
            framesWithoutHaar = 0;
        }
    }
    
    //--------------------------------------------------------------
    ofRectangle Person::getHaarRect(){
        if(!hasHaar){
            printf("Person: accessing Haar rect without Haar!\n");
            return ofRectangle(0,0,0,0);
        }
        return haarRect;
    }
    
    //--------------------------------------------------------------
    bool Person::hasHaarRect(){
        return hasHaar;
    }
    
    //--------------------------------------------------------------
    void Person::noHaarThisFrame(){
        //temp remove keep haar
        hasHaar = false;
        haarRect.set(0,0,0,0);
        return;
        
        //if we had a haar but it vanished move it by the movement of the blob
        if(framesWithoutHaar++ > MAX_HAAR_GHOSTFRAMES){
            hasHaar = false;
        }
        if(hasHaar){
            haarRect.x += velocity.x;
            haarRect.y += velocity.y;
        }
    }
}
