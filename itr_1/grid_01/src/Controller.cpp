#include "Controller.h"

Controller::Controller(WorldRef world) :
mWorld(world){}

void Controller::keyDown(KeyEvent event){
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