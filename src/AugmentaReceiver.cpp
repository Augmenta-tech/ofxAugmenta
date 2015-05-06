
/***************************************************************************
 *
 *  Receiver.cpp
 *	Receiver
 *
 ***************************************************************************/

#include "AugmentaReceiver.h"

using namespace Augmenta;
    
//--------------------------------------------------------------
Receiver::Receiver(){
    
    // Init
    personTimeout = 120; // frame
    status = UNKNOWN;
}
    
//--------------------------------------------------------------
void Receiver::connect(int port){
    
    // Events
    if (status == UNKNOWN){
        ofAddListener(ofEvents().update, this, &Receiver::update);
    }
    
    // Osc connection
    setup(port);
    status = CONNECTED; // TODO : Better ?
}

//--------------------------------------------------------------
void Receiver::draw(int width, int height){
    
    // Draw every persons
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
ofPoint Receiver::getSceneSize(){
    return ofPoint(scene.width, scene.height);
}

//--------------------------------------------------------------
vector<Person*> & Receiver::getPeople(){

    // We have a map as an internal representation for the person vector (people) in the interactive area
    // So we transform it here into a vector because it is what is asked :)
    currentPeopleArray.clear();
    for( map<int, Person*>::const_iterator it = currentPeople.begin(); it != currentPeople.end(); ++it ) {
        currentPeopleArray.push_back( it->second );
    }
    return currentPeopleArray;
}

//--------------------------------------------------------------
void Receiver::update(ofEventArgs &e){
    
    // Check if there are persons to kill (timeout)
    std::vector<int> toDelete;
    
    for(int i=0 ; i<trackedPeople.size(); ++i) {

        if(trackedPeople[i]!=NULL){
            trackedPeople[i]->updateRemainingTime();
            if(trackedPeople[i]->getRemainingtime() <= 0){
                EventArgs args;
                args.person = trackedPeople[i];
                args.scene = &scene;
                ofNotifyEvent(Events().personWillLeave, args, this);
                toDelete.push_back(i);
            }
        }
    }
    
    // Delete the persons which are marked dead
    for(int j=0; j<toDelete.size(); ++j){
        currentPeople.erase(trackedPeople[toDelete[j]]->pid);
        delete trackedPeople[toDelete[j]];
        trackedPeople.erase(trackedPeople.begin() + toDelete[j]);
    }
    
    // Get OSC data
    if(hasWaitingMessages()){

        // Check for waiting messages
        while( hasWaitingMessages() ){
            // Get the next message
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
            
            // Custom event for dev
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
                
                int pid = m.getArgAsInt32(0);
                bool personIsNew = false;
                
                // Checking if the person already exist
                Person* person = getPersonWithPID(pid);
                
                // Person does not already exist, we create it
                if(person == NULL){
                    person = new Person(pid, trackedPeople.size());
                    trackedPeople.push_back(person);
                    personIsNew = true;
                }
                
                updatePersonFromOSC(person, m);
                person->setRemainingtime(personTimeout); // Timer init

                // Send Event if need to be
                EventArgs args;
                args.person = person;
                args.scene = &scene;
                
                // PERSON_ENTERED
                if (m.getAddress() == PERSON_ENTERED || personIsNew){
                    
                    // We send the event only if person is in the interactive area
                    if(interactiveArea.contains(person->centroid)){
                        ofNotifyEvent(Events().personEntered, args, this);
                        currentPeople.insert(std::pair<int, Person*>(person->pid, person));
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
                            currentPeople.insert(std::pair<int, Person*>(person->pid, person));
                        }
                        else{
                            ofNotifyEvent(Events().personUpdated, args, this);
                        }
                    } else{
                        // Else we have to act like that the person left
                        if(personExists){
                            ofNotifyEvent(Events().personWillLeave, args, this);
                            currentPeople.erase(mapIterator->second->pid);
                        } // If the person does not exist in the scene no need to do this again
                    }
                    
                } // PERSON_LEAVING
                
                // TODO  : TOKILL ?
                
                /*else if (m.getAddress() == PERSON_LEAVING){
                    
                    // We send the event only if person is in the interactive area
                    //ofNotifyEvent(Events().personWillLeave, args, this);
                }*/
                
                if(m.getAddress() == PERSON_LEAVING){
                    for (int i = trackedPeople.size() - 1; i >= 0; i--){
                        if (trackedPeople[i]->pid == person->pid){
                            trackedPeople.erase(trackedPeople.begin() + i);
                            currentPeople.erase(person->pid);
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
}

//--------------------------------------------------------------
//callbacks to get people
Person* Receiver::getPersonAtIndex(int i)
{
    return trackedPeople[i];
}

//--------------------------------------------------------------
Person* Receiver::getPersonWithPID(int pid)
{
    for (int i = 0; i < trackedPeople.size(); i++){
        if (trackedPeople[i]->pid == pid){
            return trackedPeople[i];
        }
    }
    return NULL;
}

//--------------------------------------------------------------
Person* Receiver::getOldestPerson(){
    int bestAge = 0;
    int bestPerson = -1;
    
    // Get people in the interactive area
    vector<Person*> currentPeople = getPeople();
    
    // For each person...
    for (int i = 0; i < currentPeople.size(); i++) {
        if(currentPeople[i]->age > bestAge){
            bestAge = currentPeople[i]->age;
            bestPerson = i;
        }
    }
    Person* p = NULL;
    if(bestPerson != -1){
        p = currentPeople[bestPerson];
    }
    return p;
}

//--------------------------------------------------------------
Person* Receiver::getNewestPerson(){
    int bestAge = 2147483647; // max int
    int bestPerson = -1;
    
    // Get people in the interactive area
    vector<Person*> currentPeople = getPeople();
    
    // For each person...
    for (int i = 0; i < currentPeople.size(); i++) {
        if(currentPeople[i]->age < bestAge){
            bestAge = currentPeople[i]->age;
            bestPerson = i;
        }
    }
    Person* p = NULL;
    if(bestPerson != -1){
        p = currentPeople[bestPerson];
    }
    return p;
}

//--------------------------------------------------------------
void Receiver::setTimeOut(int t)
{
    personTimeout = t;
}

