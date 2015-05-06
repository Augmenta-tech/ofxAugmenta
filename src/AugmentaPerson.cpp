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
void Person::draw(int _width, int _height){
    
    ofSetColor(ofColor::yellow);
    
    // Draw person
    ofRect(boundingRect.x* _width, boundingRect.y* _height, boundingRect.width* _width, boundingRect.height* _height);
    
    // Draw highest point
    ofSetColor(ofColor::purple);
    ofCircle(highest.x* _width, highest.y* _height, 4);
    
    // Draw centroid
    ofSetColor(ofColor::red);
    ofCircle(centroid.x* _width, centroid.y* _height, 3);
    
    // Draw pid oid age
    ofSetColor(ofColor::white);
    char idstr[1024];
    sprintf(idstr, "pid: %d\noid: %d\nage: %d", pid, oid, age );
    ofDrawBitmapString(idstr, centroid.x* _width+8, centroid.y* _height);
}
