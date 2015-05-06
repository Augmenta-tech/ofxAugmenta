/*
 *  Events.cpp
 *  
 */

#include "AugmentaEvents.h"

namespace Augmenta {
    CoreEvents & Events(){
        static CoreEvents * events = new CoreEvents;
        return *events;
    }
}