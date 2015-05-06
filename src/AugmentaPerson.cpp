/*
 *  Person.cpp
 *  
 *
 */

#include "AugmentaPerson.h"
#define MAX_HAAR_GHOSTFRAMES 15 //how long before we say it's really gone

namespace Augmenta {
    //--------------------------------------------------------------
    Person::Person()
    : age(0),
    haarRect(ofRectangle(0,0,0,0)),
    opticalFlow(ofPoint(0,0)),
    centroid(0,0),
    customAttributes(NULL),
    depth(0),
    highest(-1,1),
    lowest(-1,-1),
    timeRemaining(0)
    {
        update();
    }
   
    //--------------------------------------------------------------
    Person::Person(int _id, int _oid, int _age, float _depth, ofPoint _centroid, ofPoint _velocity, ofRectangle _boundingRect, float _highestX, float _highestY, float _highestZ) : id(_id),
    oid(_oid),
    age(_age),
    depth(_depth),
    centroid(_centroid),
    velocity(_velocity),
    boundingRect(_boundingRect),
    highest(ofPoint(_highestX, _highestY)),
    
    //default :
    haarRect(ofRectangle(0,0,0,0)),
    opticalFlow(ofPoint(0,0)),
    customAttributes(NULL),
    lowest(-1,-1),
    timeRemaining(0)
    {
        update();
    }
    
    //--------------------------------------------------------------
    Person::Person(int pid, int oid )
    : id(pid),
    oid(oid),
    age(0),
    haarRect(ofRectangle(0,0,0,0)),
    opticalFlow(ofPoint(0,0)),
    centroid(0,0),
    customAttributes(NULL),
    depth(0),
    highest(-1,1),
    lowest(-1,-1),
    timeRemaining(0)
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
    void Person::draw( int cameraWidth, int cameraHeight, bool bSendContours){
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
        
        ofSetHexColor(0xeeda00);
        
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
        sprintf(idstr, "pid: %d\noid: %d\nage: %d", id, oid, age );
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
}
