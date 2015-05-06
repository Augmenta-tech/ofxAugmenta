/***************************************************************************
 *
 *  Augmenta::Person.h
 *
 ***************************************************************************/

#ifndef OFX_PERSON_OBJECT
#define OFX_PERSON_OBJECT

#include "ofMain.h"

namespace Augmenta {
    class Person {
	public: 
		/** Unique ID, different for each Person as long as Augmenta is running */
        int id;
		/** Ordered ID (not usually used), ID ranging from 0-Total Number of people */
        int oid;
        /** How long a person has been around (in seconds) */
		int age;
        /** Normalized (0.0-1.0) distance from camera. For Kinect camera, highest value (1) is approx. 10 meters*/
        float depth;
		/** Center of mass of person */
		ofPoint centroid;
        /** Speed since last update */
        ofPoint velocity;
        /** Closest point to the camera (with Kinect). If using non-depth camera, represents brightest point on person. */
        ofPoint highest;
        ofPoint lowest;
        /** Average motion within a Person's area */
        ofPoint opticalFlow;
        /** Bounding rectangle that surrounds Person's shape*/
		ofRectangle boundingRect;
        /** Rectangle representing a detected HAAR feature (if there is one) */
		ofRectangle haarRect;
        /** Defines the rough outline of a Person*/
		ofPolyline contour;
        /** Defines the simplified outline of a Person*/
		ofPolyline simpleContour;
        /** Time remaining relative to receiver's timeout*/
        int timeRemaining; 
        
		float area; //area as a scalar size
        
        Person();
        Person(int _id, int _oid, int _age, float _depth, ofPoint _centroid, ofPoint _velocity, ofRectangle _boundingRect, float _highestX, float _highestY, float _highestZ);
        Person( int pid, int oid );
		~Person();
		
        virtual void update();
        virtual void updateBoundingRect( ofRectangle _rect );
        virtual void updateCentroid( ofPoint _centroid, bool dampen );
        virtual void updateContour( ofPolyline _contour );
        
        virtual void draw( int cameraWidth, int cameraHeight, bool bSendContours=false);
        
		//this can be a pointer to whatever you want to store in this person
		void* customAttributes;
        
		
    };
}

#endif