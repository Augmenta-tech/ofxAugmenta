
/***************************************************************************
 *
 *  ofxAugmenta.cpp
 *
 ***************************************************************************/

#include "ofxAugmenta.h"

using namespace Augmenta;
    
//--------------------------------------------------------------
Receiver::Receiver(){
    
    // Init
    personTimeout = 120; // frame
    maxNumPeople = INT32_MAX;
    status = UNKNOWN;
}
    
//--------------------------------------------------------------
void Receiver::connect(int port){
    
    // Events
    if (status == UNKNOWN){
        ofAddListener(ofEvents().update, this, &Receiver::update);
    }
    
    // Osc connection
    
    try{
        setup(port);
    } catch (std::exception&e){
        ofLogWarning("setupOSC") << "Error : Couldn't bind to port "<< port << " ("<< ofToString(e.what()) << ")";
        status = DISCONNECTED;
        return;
    }
    
    status = CONNECTED; 
}

//--------------------------------------------------------------
bool Receiver::isConnected(){
    
    switch(status){
        case UNKNOWN:
        case CONNECTING:
        case DISCONNECTED:
            return false;
            break;
        
        case CONNECTED:
            return true;
            break;
            
        default:
            break;
        
    }
    
}

//--------------------------------------------------------------
void Receiver::draw(int width, int height){
    
    // Draw every persons
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(.5);
    vector<Person*> people = getPeople();
    for(int i=0; i<people.size(); ++i) {
        if(people[i] != NULL)
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

    return trackedPeople;
}

//--------------------------------------------------------------
void Receiver::update(ofEventArgs &e){
    
    // Check if there are persons to kill (timeout)
    for(int i=0 ; i<trackedPeople.size(); ++i) {

        if(trackedPeople[i]!=NULL){
            trackedPeople[i]->updateRemainingTime();
            // if person excedeed timeout
            if(trackedPeople[i]->getRemainingtime() <= 0){
                // update event
                EventArgs args;
                args.person = trackedPeople[i];
                args.scene = &scene;
                ofNotifyEvent(Events().personWillLeave, args, this);
                
                // delete person
                Person* personToDelete = trackedPeople[i];
                trackedPeople.erase(trackedPeople.begin() + i);
                delete personToDelete;
                
                // update index for the loop
                i--;    // to don't skip the next person, which actually took the place of the person we just deleted
            }
        }
    }
    
    // Get OSC data
    if(hasWaitingMessages()){

        // Check for waiting messages
        while( hasWaitingMessages() ){
            // Get the next message
            ofxOscMessage m;
            getNextMessage(m);
            
            // SCENE EVENTS
            // ------------
            if(m.getAddress() == SCENE_UPDATED || m.getAddress() == SCENE_UPDATED+"/" ) {
                scene.age               = m.getArgAsInt32(0);
                scene.percentCovered    = m.getArgAsFloat(1);
                scene.numPeople         = m.getArgAsInt32(2);
                scene.averageMotion.x   = m.getArgAsFloat(3);
                scene.averageMotion.y   = m.getArgAsFloat(4);
                scene.width             = m.getArgAsInt32(5);
                scene.height            = m.getArgAsInt32(6);
            
            // CUSTOM EVENTS
            // -------------
            } else if ( m.getAddress() == CUSTOM_EVENT || m.getAddress() == CUSTOM_EVENT+"/" ){
                
                CustomEventArgs evtArgs;
                evtArgs.name = m.getArgAsString(0);
               
                for (int i=1; i< m.getNumArgs(); i++){
                    evtArgs.args.push_back( m.getArgAsString(i) );
                }
                
                ofNotifyEvent(Events().customEvent, evtArgs, this);
            
            // PERSON EVENTS
            // -------------
            } else if (m.getAddress() == PERSON_ENTERED || m.getAddress() == PERSON_ENTERED+"/" ||
                m.getAddress() == PERSON_UPDATED || m.getAddress() == PERSON_UPDATED+"/" ||
                m.getAddress() == PERSON_LEAVING || m.getAddress() == PERSON_LEAVING+"/" ){
                
                int pid = m.getArgAsInt32(0);
                ofPoint centroid = ofPoint(m.getArgAsFloat(3), m.getArgAsFloat(4));
                bool personExists = true;
                
                // Checking if the person already exist
                Person* person = getPersonWithPID(pid);

                // Person does not already exist
                if(person == NULL){
                    personExists = false;
                }
                
                // Send Event if need to be
                EventArgs args;
                args.person = person;  // /!\ Take care to update this pointer in cases below if new person is created 
                args.scene = &scene;
                
                // PERSON_ENTERED
                if (m.getAddress() == PERSON_ENTERED || m.getAddress() == PERSON_ENTERED+"/" || !personExists){

                    // Create person if it is in interactive area and we did not reached limit 
                    if(interactiveArea.contains(centroid) && trackedPeople.size()<maxNumPeople){
                        person = new Person(pid, trackedPeople.size());
                        trackedPeople.push_back(person);
                        
                        updatePersonFromOSC(person, m);
                        person->setRemainingtime(personTimeout); // Timer init
                        
                        args.person = person;   // Update pointer to person we just created
                        ofNotifyEvent(Events().personEntered, args, this);
                    }

                }
                // PERSON_UPDATED
                else if (m.getAddress() == PERSON_UPDATED || m.getAddress() == PERSON_UPDATED+"/" ){
                    // Check if the person is in the interactive area
                    if(interactiveArea.contains(centroid)){
                        // If person does not already exist, we create it if we did not reached limit
                        if(!personExists && trackedPeople.size()<maxNumPeople){
                            person = new Person(pid, trackedPeople.size());
                            trackedPeople.push_back(person);
                            
                            updatePersonFromOSC(person, m);
                            person->setRemainingtime(personTimeout); // Timer init
                            
                            args.person = person;   // Update pointer to person we just created
                            ofNotifyEvent(Events().personEntered, args, this);
                        }
                        // Else, the person already exists, we just update it
                        else{
                            updatePersonFromOSC(person, m);
                            person->setRemainingtime(personTimeout); // Timer init

                            ofNotifyEvent(Events().personUpdated, args, this);
                        }
                    }
                    // Else we have to act like that the person left
                    else{
                        if(personExists){
                            ofNotifyEvent(Events().personWillLeave, args, this);
                            trackedPeople.erase(trackedPeople.begin() + getIndexOfPersonWithPID(pid));
                            delete person;
                        } // If the person does not exist in the scene no need to do this again
                    }
                    
                }
                // PERSON_LEAVING
                else if(m.getAddress() == PERSON_LEAVING || m.getAddress() == PERSON_LEAVING+"/" ){
                    // If the person leaving was in the interactive area
                    if(personExists){
                        ofNotifyEvent(Events().personWillLeave, args, this);
                        trackedPeople.erase(trackedPeople.begin() + getIndexOfPersonWithPID(pid));
                        delete person;
                    }
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
    
    if(smoothAmount != 0){
        p->smooth(smoothAmount);
    }
    
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
int Receiver::getIndexOfPersonWithPID(int pid)
{
    for (int i = 0; i < trackedPeople.size(); i++){
        if (trackedPeople[i]->pid == pid){
            return i;
        }
    }
    return -1;
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
        // If several persons have the same oldest age, take the one with smallest pid
        else if(currentPeople[i]->age == bestAge){
            if(currentPeople[i]->pid < currentPeople[bestPerson]->pid){
                bestPerson = i;
            }
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
        // If several persons have the same newest age, take the one with greatest pid
        else if(currentPeople[i]->age == bestAge){
            if(currentPeople[i]->pid > currentPeople[bestPerson]->pid){
                bestPerson = i;
            }
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

//--------------------------------------------------------------
void Receiver::setMaxNumPeople(int m)
{
    maxNumPeople = m;
}

//--------------------------------------------------------------
void Receiver::setSmooth(float amount)
{
    // Check amount bounds
    if(amount < 0){
        smoothAmount = 0;
    } else if(amount > 0.99){
        smoothAmount = 0.99f;
    } else {
        smoothAmount = amount;
    }
}
