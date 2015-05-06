/*
 *  Augmenta::Scene.h
 *
 */

#pragma once

#include "ofMain.h"

#ifdef AUGMENTA_USE_OSC
class ofxOscMessage;
#endif

namespace Augmenta {
    class Person;
    class Scene {
    public:
        
        Scene();
        
        int     age;
        float   percentCovered;
        int     numPeople;
        ofPoint averageMotion;
        int     width;
        int     height;
        
        
#ifdef AUGMENTA_USE_OSC
        ofxOscMessage   getOscMessage( string address );
#endif
        string          getJSONMessge();
        string          getTCPMessage();
        
        void reset();
        void update( vector<Person*> * people, int cameraWidth, int cameraHeight );
        void draw( int width, int height );
        
        void buildGrid( int w, int h );
        vector<ofRectangle> & getGrid();
        vector<bool> & getGridValues();
        
        int getGridX();
        int getGridY();
        
    private:
        
        void processPerson( Person * p, int cameraWidth, int cameraHeight );
        vector<bool>        activeGrid;
        vector<ofRectangle> grid;
        
        int gridX, gridY;
    };
}
