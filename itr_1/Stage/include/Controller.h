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
#include "stage/Stage.h"

using namespace ci;
using namespace ci::app;

class Controller {
    next::StageRef mWorld;
    
public:
    Controller(next::StageRef world);
    void keyDown(KeyEvent event);
};


#endif
