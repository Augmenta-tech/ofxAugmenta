//
//  ofxTSPSReceiver.h
//  TSPSReceiver
//
//  Created by Brett Renfer on 11/1/12.
//
//

#pragma once

#include "ofxOsc.h"
#include "AugmentaEvents.h"
#include "AugmentaPerson.h"
#include "AugmentaScene.h"

namespace Augmenta {
    
    // update these if TSPS core changes!
    // valid as of TSPS version 1.3.4
    static const string PERSON_ENTERED  = "/au/personEntered";
    static const string PERSON_UPDATED  = "/au/personUpdated";
    static const string PERSON_LEAVING  = "/au/personWillLeave";
    static const string SCENE_UPDATED   = "/au/scene";
    static const string CUSTOM_EVENT    = "/au/customEvent";
    
    enum ConnectionStatus
    {
        UNKNOWN,
        CONNECTING,
        CONNECTED,
        DISCONNECTED
    };
    
    class Receiver : public ofxOscReceiver
    {
    public:
        Receiver();
        
        void connect( int port );
        void update( ofEventArgs &e );
        void draw( int width, int height );
        
        Person* personAtIndex(int i);
        Person* personWithID(int pid);
        
        int totalPeople();
        Scene* getScene();
        vector<Person*> & getPeople();
        
    protected:
        ConnectionStatus status;
        
        vector<Person*> trackedPeople;
        Scene scene;
        float personTimeout;
        
        void updatePersonFromOSC( Person * p, ofxOscMessage & m );
    };
}
