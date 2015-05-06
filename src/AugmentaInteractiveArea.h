//
//  AugmentaInteractiveArea.h
//  Augmenta_2D
//
//  Created by Theoriz-MacMini on 5/5/15.
//
//

#ifndef __Augmenta_2D__AugmentaInteractiveArea__
#define __Augmenta_2D__AugmentaInteractiveArea__

#include <iostream>
#include "ofMain.h"

#endif /* defined(__Augmenta_2D__AugmentaInteractiveArea__) */

namespace Augmenta {
    
    class InteractiveArea {
    public:
        ofRectangle area;
        
        InteractiveArea();
        void set(float x, float y, float width, float height);
        bool contains(ofPoint p);
        
        /** Draw a debug view */
        void draw();
    };
}
