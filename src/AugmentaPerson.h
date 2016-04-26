/***************************************************************************
 *
 *  Augmenta::AugmentaPerson.h
 *
 ***************************************************************************/

#ifndef AUGMENTA_PERSON_H
#define AUGMENTA_PERSON_H

#include "ofMain.h"

/*
 
 Get latest protocol up-to-date here : https://github.com/Theoriz/Augmenta/wiki
 
 Copied here (for reference only) :
 
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 
     * Augmenta OSC Protocol :
     
     /au/personWillLeave args0 arg1 ... argn
     /au/personUpdated   args0 arg1 ... argn
     /au/personEntered   args0 arg1 ... argn
     
     where args are :
     
     
     0: pid (int)                        // Personal ID ex : 42th person to enter stage has pid=42
     1: oid (int)                        // Ordered ID ex : if 3 person on stage, 43th person has oid=2
     2: age (int)                        // Time on stage (in frame number)
     3: centroid.x (float)               // Position projected to the ground
     4: centroid.y (float)
     5: velocity.x (float)               // Speed and direction vector
     6: velocity.y (float)
     7: depth (float)                    // Distance to sensor (in m)
     8: boundingRect.x (float)           // Bounding box on the ground
     9: boundingRect.y (float)
     10: boundingRect.width (float)
     11: boundingRect.height (float)
     12: highest.x (float)               // Highest point placement
     13: highest.y (float)
     14: highest.z (float)               // Height
     
     /au/scene   args0 arg1 ... argn
     
     0: currentTime (int)                // Time (in frame number)
     1: percentCovered (float)           // Percent covered
     2: numPeople (int)                  // Number of person
     3: averageMotion.x (float)          // Average motion
     4: averageMotion.y (float)
     5: scene.width (int)                // Scene size
     6: scene.height (int)
 
////////////////////////////////////////////////////////////////////////////////////////////////////////*/

namespace Augmenta {
    
    class Person {
        
	public:
        
        // Methods
        Person();
        Person(int _pid, int _oid, int _age, float _depth, ofPoint _centroid, ofPoint _velocity, ofRectangle _boundingRect, float _highestX, float _highestY, float _highestZ);
        Person( int pid, int oid );
		~Person();
        
        virtual void update();
        virtual void draw();
        virtual void draw(int _width, int _height);
        
        // Getter
        inline int getRemainingtime() {return timeRemaining;}
        inline void updateRemainingTime() {timeRemaining--;}
        
        // Setter
        inline void setRemainingtime(int _timeRemaining) {timeRemaining = _timeRemaining;}
        virtual void setBoundingRect( ofRectangle _rect );
        virtual void setCentroid( ofPoint _centroid, bool dampen );
        
        // Smooth values
        void smooth(float amount);
        
        // Members
        int pid;
        int oid;
		int age;
		ofPoint centroid;
        ofPoint velocity;
        float depth; // Not implemented yet
		ofRectangle boundingRect;
        ofPoint highest;
        
        // Members stored from last frame
        ofPoint lastCentroid;
        float lastDepth;
        ofRectangle lastBoundingRect;
        ofPoint lastHighest;

    private:
        
        ofColor genColorFromPID();
        
        int timeRemaining;
    };
}

#endif // AUGMENTA_PERSON_H
