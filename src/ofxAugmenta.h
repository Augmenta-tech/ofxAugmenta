/***************************************************************************
 *
 *  Augmenta::AugmentaReceiver.h
 *
 ***************************************************************************/

#ifndef AUGMENTA_RECEIVER_H
#define AUGMENTA_RECEIVER_H

#include "ofxOsc.h"

#include "AugmentaEvents.h"
#include "AugmentaPerson.h"
#include "AugmentaScene.h"
#include "AugmentaInteractiveArea.h"

namespace Augmenta {
    
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
        void setTimeOut(int t);
        
        vector<Person*> & getPeople();
        inline Scene* getScene() {return &scene;}
        inline InteractiveArea* getInteractiveArea() {return &interactiveArea;}
        Person* getOldestPerson();
        Person* getNewestPerson();
        
    private:

        void updatePersonFromOSC( Person * p, ofxOscMessage & m );
        
        Person* getPersonAtIndex(int i);
        Person* getPersonWithPID(int pid);
        int getIndexOfPersonWithPID(int pid);
        ofPoint getSceneSize();

        InteractiveArea interactiveArea;
        ConnectionStatus status;
        Scene scene;
        
        vector<Person*> trackedPeople;         /** All people tracked in the interactive area */
        
        int personTimeout;
    };
}

#endif //AUGMENTA_RECEIVER_H
