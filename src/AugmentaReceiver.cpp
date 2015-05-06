
/***************************************************************************
 *
 *  Receiver.cpp
 *	Receiver
 *
 ***************************************************************************/

#include "AugmentaReceiver.h"

namespace Augmenta {
    
    //--------------------------------------------------------------
    Receiver::Receiver(){
        personTimeout = 120;
        
        // set connection status
        status = UNKNOWN;
    }
        
    //--------------------------------------------------------------
    void Receiver::connect( int port ){
        if ( status == UNKNOWN ){
            ofAddListener(ofEvents().update, this, &Receiver::update);
        }
        setup(port);
        status = CONNECTED;
    }
    
    //--------------------------------------------------------------
    void Receiver::draw(int width, int height){
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(.5);
        for(int i = 0; i < totalPeople(); i++){
            Person* p = personAtIndex(i);
            p->draw(width, height);
        }
        ofPopStyle();
    }
    
    //--------------------------------------------------------------
    Scene* Receiver::getScene()
    {
        return &scene;
    }
    
    
    //--------------------------------------------------------------
    vector<Person*> & Receiver::getPeople(){
        return trackedPeople;
    }
    
    //--------------------------------------------------------------
    ofPoint Receiver::getSceneSize(){
        return ofPoint(scene.width, scene.height);
    }
    
    //--------------------------------------------------------------
    void Receiver::update( ofEventArgs &e ){
        // store people who hasn't been updated since a given timeout, and who will be deleted
        std::vector<std::vector<Person*>::iterator> peopleToRemove; // people are stored in temp. vector to prevent bad access exception
        for(std::vector<Person*>::iterator person = trackedPeople.begin(); person != trackedPeople.end(); ++person) {
            if((*person)!=NULL){
                (*person)->timeRemaining--;
                if((*person)->timeRemaining <= 0){
                    EventArgs args;
                    args.person = *person;
                    args.scene = &scene;
                    ofNotifyEvent(Events().personWillLeave, args, this);
                    peopleToRemove.push_back(person);
                }
            }
        }
        // delete people
        for(int personToRemove = 0; personToRemove<peopleToRemove.size(); personToRemove++){
            trackedPeople.erase(peopleToRemove[personToRemove]);
            delete *(peopleToRemove[personToRemove]);
        }
        peopleToRemove.clear();
        
        // update loop
        if(hasWaitingMessages()){
            // check for waiting messages
            while( hasWaitingMessages() ){
                // get the next message
                ofxOscMessage m;
                getNextMessage( &m );
                
                if(m.getAddress() == SCENE_UPDATED ) {
                    scene.age               = m.getArgAsInt32(0);
                    scene.percentCovered    = m.getArgAsFloat(1);
                    scene.numPeople         = m.getArgAsInt32(2);
                    scene.averageMotion.x   = m.getArgAsFloat(3);
                    scene.averageMotion.y   = m.getArgAsFloat(4);
                    scene.width             = m.getArgAsInt32(5);
                    scene.height            = m.getArgAsInt32(6);
                    
                    int gridX = m.getArgAsInt32(5);
                    int gridY = m.getArgAsInt32(6);
                    
                    if ( gridX != scene.getGridX() || gridY != scene.getGridY() ){
                        scene.buildGrid( gridX, gridY);
                    }
                    vector<bool> & grid = scene.getGridValues();
                    
                    // double check
                    if ( grid.size() == m.getNumArgs() - 7 ){
                        for (int i=0; i<grid.size(); i++){
                            grid[i] = m.getArgAsInt32(i + 7);
                        }
                    }
                    
                } else if ( m.getAddress() == CUSTOM_EVENT ){
                    
                    CustomEventArgs evtArgs;
                    evtArgs.name = m.getArgAsString(0);
                   
                    for (int i=1; i< m.getNumArgs(); i++){
                        evtArgs.args.push_back( m.getArgAsString(i) );
                    }
                    
                    ofNotifyEvent(Events().customEvent, evtArgs, this);
                    
                } else if (m.getAddress() == PERSON_ENTERED ||
                    m.getAddress() == PERSON_UPDATED ||
                    m.getAddress() == PERSON_LEAVING ){
                    
                    int id = m.getArgAsInt32(0);
                    bool personIsNew = false;
                    Person* person = personWithID( id );
                    if(person == NULL){
                        person = new Person(id, trackedPeople.size());
                        trackedPeople.push_back(person);
                        personIsNew = true;
                    }
                    updatePersonFromOSC(person, m);
                    person->timeRemaining = personTimeout;
                    
                    EventArgs args;
                    args.person = person;
                    args.scene = &scene;
                    
                    if (m.getAddress() == PERSON_ENTERED || personIsNew){
                        ofNotifyEvent(Events().personEntered, args, this);
                    } else if (m.getAddress() == PERSON_UPDATED){
                        ofNotifyEvent(Events().personUpdated, args, this);
                        
                    } else if (m.getAddress() == PERSON_LEAVING){
                        ofNotifyEvent(Events().personWillLeave, args, this);
                    }
                    
                    if(m.getAddress() == PERSON_LEAVING){
                        for (int i = trackedPeople.size() - 1; i >= 0; i--){
                            if (trackedPeople[i]->id == person->id){
                                trackedPeople.erase(trackedPeople.begin() + i);
                                break;
                            }
                        }
                        delete person;
                    }
                }
            }
        }
    }
    
    //--------------------------------------------------------------
    void Receiver::updatePersonFromOSC( Person * p, ofxOscMessage & m ){
        p->oid = m.getArgAsInt32(1);
        p->age = m.getArgAsInt32(2);
        p->centroid.set( m.getArgAsFloat(3), m.getArgAsFloat(4));
        p->velocity.set( m.getArgAsFloat(5), m.getArgAsFloat(6));
        p->depth = m.getArgAsFloat(7);
        p->boundingRect.set(m.getArgAsFloat(8),m.getArgAsFloat(9),m.getArgAsFloat(10),m.getArgAsFloat(11));
        p->highest.set(m.getArgAsFloat(12), m.getArgAsFloat(13), m.getArgAsFloat(14));
        
        ofPolyline line;
        for (int i=20; i<m.getNumArgs(); i+=2){
            line.addVertex( ofPoint(m.getArgAsFloat(i),m.getArgAsFloat(i+1)) );
        }
        if ( line.size() > 0 ){
            p->updateContour( line );
        }
    }
    
    //--------------------------------------------------------------
    //callbacks to get people
    Person* Receiver::personAtIndex(int i)
    {
        return trackedPeople[i];
    }
    
    //--------------------------------------------------------------
    Person* Receiver::personWithID(int pid)
    {
        for (int i = 0; i < trackedPeople.size(); i++){
            if (trackedPeople[i]->id == pid){
                return trackedPeople[i];
            }
        }
        return NULL;
    }
    
    //--------------------------------------------------------------
    Person* Receiver::getOldestPerson(){
        int bestAge = 0;
		int bestPerson = -1;
		// For each person...
		for (int i = 0; i < trackedPeople.size(); i++) {
            if(trackedPeople[i]->age > bestAge){
                bestAge = trackedPeople[i]->age;
                bestPerson = i;
            }
        }
        Person* p = NULL;
        if(bestPerson != -1){
            p = trackedPeople[bestPerson];
        }
		return p;
    }
    
    //--------------------------------------------------------------
    int Receiver::totalPeople()
    {
        return trackedPeople.size();
    }
}

