/***************************************************************************
 *
 *  Augmenta::AugmentaInteractiveArea.h
 *
 ***************************************************************************/

#ifndef AUGMENTA_INTERACTIVE_AREA_H
#define AUGMENTA_INTERACTIVE_AREA_H

#include "ofMain.h"

namespace Augmenta {
    
    class InteractiveArea {
        
    public:
        
        InteractiveArea();
        
        void draw();
        
        void set(float x, float y, float width, float height);
        bool contains(ofPoint p);

    private:
        
        ofRectangle area;
    };
}

#endif // AUGMENTA_INTERACTIVE_AREA_H