//
//  Controller.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Controller_h
#define grid_00_Controller_h

#include "cinder/app/App.h"
#include "world/World.h"

using namespace ci;
using namespace ci::app;

class Controller {
    WorldRef mWorld;
    
public:
    Controller(WorldRef world) :
        mWorld(world){
            

            
    }
    
    inline void keyDown(KeyEvent event){
        switch (event.getCode()) {
            case KeyEvent::KEY_UP:
                mWorld->zoomModelIn();
                break;
            case KeyEvent::KEY_DOWN:
                mWorld->zoomModelOut();
                break;
            case KeyEvent::KEY_LEFT:
                mWorld->viewOrtho();
                break;
            case KeyEvent::KEY_RIGHT:
                mWorld->viewTop();
                break;
                
            default:
                break;
        }
    }
};


#endif
