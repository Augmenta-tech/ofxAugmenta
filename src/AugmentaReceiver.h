//
//  AugmentaReceiver.h
//  
//


#pragma once

#include "ofxOsc.h"
#include "AugmentaEvents.h"
#include "AugmentaPerson.h"
#include "AugmentaScene.h"
#include "AugmentaInteractiveArea.h"

namespace Augmenta {
    
    // update these if Augmenta core changes!
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
        
        InteractiveArea interactiveArea;
        
        void connect( int port );
        void update( ofEventArgs &e );
        void draw( int width, int height );
        
        Person* personAtIndex(int i);
        Person* personWithID(int pid);
        Person* getOldestPerson();
        
        int totalPeople();
        Scene* getScene();
        vector<Person*> & getPeople();
        
        ofPoint getSceneSize();
        
        void setTimeOut(int t);
        
    protected:
        ConnectionStatus status;
        
        /** All people tracked in the whole scene */
        vector<Person*> trackedPeople;
        /** Map of people in the interactive area, accessible by unique id */
        map<int, Person*> currentPeople;
        /** Array of people in the interactive area */
        vector<Person*> currentPeopleArray;
        
        Scene scene;
        int personTimeout;
        
        void updatePersonFromOSC( Person * p, ofxOscMessage & m );
    };
}
