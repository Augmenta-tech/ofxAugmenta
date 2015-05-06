
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
        vector<Person*> people = getPeople();
        for(int i=0; i<people.size(); ++i) {
            people[i]->draw(width, height);
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
        //return trackedPeople;
        currentPeopleArray.clear();
        for( map<int, Person*>::const_iterator it = currentPeople.begin(); it != currentPeople.end(); ++it ) {
            currentPeopleArray.push_back( it->second );
        }
        return currentPeopleArray;
    }
    
    //--------------------------------------------------------------
    ofPoint Receiver::getSceneSize(){
        return ofPoint(scene.width, scene.height);
    }
    
    //--------------------------------------------------------------
    void Receiver::update( ofEventArgs &e ){
        std::vector<int> toDelete;
        for(int i=0 ; i<trackedPeople.size(); ++i) {
            if(trackedPeople[i]!=NULL){
                trackedPeople[i]->timeRemaining--;
                if(trackedPeople[i]->timeRemaining <= 0){
                    EventArgs args;
                    args.person = trackedPeople[i];
                    args.scene = &scene;
                    ofNotifyEvent(Events().personWillLeave, args, this);
                    toDelete.push_back(i);
                }
            }
        }
        for(int j=0; j<toDelete.size(); ++j){
            currentPeople.erase(trackedPeople[toDelete[j]]->id);
            delete trackedPeople[toDelete[j]];
            trackedPeople.erase(trackedPeople.begin() + toDelete[j]);
        }
        
        
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
                    
                    // PERSON_ENTERED
                    if (m.getAddress() == PERSON_ENTERED || personIsNew){
                        if(interactiveArea.contains(person->centroid)){
                            ofNotifyEvent(Events().personEntered, args, this);
                            currentPeople.insert(std::pair<int, Person*>(person->id, person));
                        }
                    } // PERSON_UPDATED
                    else if (m.getAddress() == PERSON_UPDATED){
                        map<int, Person*>::iterator mapIterator;
                        mapIterator = currentPeople.find(m.getArgAsInt32(0));
                        // Check if the person exists in the scene
                        bool personExists = (mapIterator != currentPeople.end());
                        
                        if(interactiveArea.contains(person->centroid)){
                            if(!personExists){
                                ofNotifyEvent(Events().personEntered, args, this);
                                currentPeople.insert(std::pair<int, Person*>(person->id, person));
                            }
                            else{
                                ofNotifyEvent(Events().personUpdated, args, this);
                            }
                        } else{
                            // Else we have to act like that the person left
                            if(personExists){
                                ofNotifyEvent(Events().personWillLeave, args, this);
                                currentPeople.erase(mapIterator->second->id);
                            }// if the person does not exist in the scene no need to do this again
                        }
                        
                    } // PERSON_LEAVING
                    else if (m.getAddress() == PERSON_LEAVING){
                        //ofNotifyEvent(Events().personWillLeave, args, this);
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
    
    //--------------------------------------------------------------
    void Receiver::setTimeOut(int t)
    {
        personTimeout = t;
    }
}

