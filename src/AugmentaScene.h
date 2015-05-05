/*
 *  ofxTSPS::Scene.h
 *  TSPSPeopleVision
 *
 *  Created by Jim on 12/10/09.
 *  Copyright 2009 University of Washington. All rights reserved.
 *
 Portions of this software are distributed under the GPL and the MIT license.
 
 The MIT License
 
 Copyright (c) 2010 James George
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
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
        
        ofPoint averageMotion;
        float   percentCovered;
        int     numPeople;
        
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
