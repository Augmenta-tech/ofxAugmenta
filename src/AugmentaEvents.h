/***************************************************************************
 *
 *  Augmenta::AugmentaEvents.h
 *
 ***************************************************************************/

#ifndef AUGMENTA_EVENTS_H
#define AUGMENTA_EVENTS_H

#include "ofMain.h"

namespace Augmenta {
    
    class Person;
    class Scene;
    
    class EventArgs {
    public:
        
		EventArgs(){};

        Person* person;
        Scene* scene;
    };
    
    class CustomEventArgs {
    public:
        
		CustomEventArgs(){};
        
        string              name;
        vector<string>      args;
    };
    
    class CoreEvents {
    public:
        
		CoreEvents(){};

        //called when the person enters the system
        ofEvent<EventArgs> personEntered;
        
        //called one frame before the person is removed from the list to let you clean up
        ofEvent<EventArgs> personWillLeave;
        
        //called every frame no matter what.
        ofEvent<EventArgs> personUpdated;
        
        //called when a custom event occurs.
        ofEvent<CustomEventArgs> customEvent;
    };
    
    CoreEvents & Events();
}

template<class ListenerClass>
void ofxAddAugmentaListeners( ListenerClass * listener ){
    ofAddListener(Augmenta::Events().personEntered, listener, &ListenerClass::onPersonEntered);
    ofAddListener(Augmenta::Events().personUpdated, listener, &ListenerClass::onPersonUpdated);
    ofAddListener(Augmenta::Events().personWillLeave, listener, &ListenerClass::onPersonWillLeave);
}

template<class ListenerClass>
void ofxAddAugmentaCustomEventListener( ListenerClass * listener ){
    ofAddListener(Augmenta::Events().customEvent, listener, &ListenerClass::onCustomEvent);
}

template<class ListenerClass>
void ofxRemoveAugmentaListeners( ListenerClass * listener ){
    ofRemoveListener(Augmenta::Events().personEntered, listener, &ListenerClass::onPersonEntered);
    ofRemoveListener(Augmenta::Events().personUpdated, listener, &ListenerClass::onPersonUpdated);
    ofRemoveListener(Augmenta::Events().personWillLeave, listener, &ListenerClass::onPersonWillLeave);
}

template<class ListenerClass>
void ofxRemoveAugmentaCustomEventListener( ListenerClass * listener ){
    ofRemoveListener(Augmenta::Events().customEvent, listener, &ListenerClass::onCustomEvent);
}

#endif // AUGMENTA_EVENTS_H
