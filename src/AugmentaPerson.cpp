/*
 *  AugmentaPerson.cpp
 */

#include "AugmentaPerson.h"

using namespace Augmenta;

//--------------------------------------------------------------
Person::Person()
:
pid(-1),
oid(-1),
age(0),
centroid(0,0),
velocity(0,0),
depth(0),
highest(-1,1),
timeRemaining(0)
{
    update();
}

//--------------------------------------------------------------
Person::Person(int _pid, int _oid )
:
pid(_pid),
oid(_oid),
age(0),
centroid(0,0),
velocity(0,0),
depth(0),
highest(-1,1),
timeRemaining(0)
{
    update();
}

//--------------------------------------------------------------
Person::Person(int _pid, int _oid, int _age, float _depth, ofPoint _centroid, ofPoint _velocity, ofRectangle _boundingRect, float _highestX, float _highestY, float _highestZ)
:
pid(_pid),
oid(_oid),
age(_age),
depth(_depth),
centroid(_centroid),
velocity(_velocity),
boundingRect(_boundingRect),
highest(ofPoint(_highestX, _highestY)),
timeRemaining(0)
{
    update();
}

//--------------------------------------------------------------
Person::~Person(){
}

//--------------------------------------------------------------
void Person::update(){
    age++;
}

//--------------------------------------------------------------
void Person::setBoundingRect(ofRectangle _rect){
    boundingRect.set(_rect);
}

//--------------------------------------------------------------
void Person::setCentroid( ofPoint _centroid, bool _dampen=false ){

    if(_dampen){
        centroid = (centroid * .7) + (_centroid * .3);
    }
    else{
        centroid = _centroid;
    }
}

//--------------------------------------------------------------
void Person::draw(){
    // dimensions from Augmenta are 0-1, so we multiply by current renderer width and height
    draw(ofGetCurrentRenderer()->getViewportWidth(), ofGetCurrentRenderer()->getViewportHeight());
}

//--------------------------------------------------------------
void Person::draw(int _width, int _height){
    
    ofSetColor(ofColor::yellow);
    
    // Draw highest point
    ofSetColor(ofColor::purple);
    ofDrawCircle(highest.x* _width, highest.y* _height, 4);
    
    // Draw centroid
    ofFill();
    ofSetColor(ofColor::white);
    ofDrawCircle(centroid.x* _width, centroid.y* _height, 8);
    ofSetColor(genColorFromPID());
    ofDrawCircle(centroid.x* _width, centroid.y* _height, 5);
   
    // Draw person
    ofNoFill();
    ofDrawRectangle(boundingRect.x* _width, boundingRect.y* _height, boundingRect.width* _width, boundingRect.height* _height);
    
    // Draw pid oid age
    ofSetColor(ofColor::white);
    char idstr[1024];
    sprintf(idstr, "pid: %d\noid: %d\nage: %d", pid, oid, age );
    ofDrawBitmapString(idstr, centroid.x* _width+8, centroid.y* _height);
}

//--------------------------------------------------------------
ofColor Person::genColorFromPID()
{
    // TODO : Use monokaï colors
    
    // Compute a color for the points
    ofColor color;
    
    if (pid%5 == 0){
        color.r = 255;
        color.g = 255;
        color.b = 0;
    } else if (pid%5 == 1){
        color.r = 255;
        color.g = 0;
        color.b = 255;
    } else if (pid%5 == 2){
        color.r = 0;
        color.g = 255;
        color.b = 255;
    } else if (pid%5 == 3){
        color.r = 0;
        color.g = 255;
        color.b = 50;
    } else {
        color.r = 50;
        color.g = 255;
        color.b = 0;
    }
    
    return color;
}
