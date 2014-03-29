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
    Controller(WorldRef world);
    void keyDown(KeyEvent event);
};


#endif
