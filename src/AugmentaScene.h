/***************************************************************************
 *
 *  Augmenta::AugmentaScene.h
 *
 ***************************************************************************/

#ifndef AUGMENTA_SCENE_H
#define AUGMENTA_SCENE_H

#include "ofMain.h"

namespace Augmenta {

    class Scene {
    
    public:
        
        Scene();
        
        int     age;
        float   percentCovered;
        int     numPeople;
        ofPoint averageMotion;
        int     width;
        int     height;
        int     depth;
        
    };
}

#endif // AUGMENTA_SCENE_H
