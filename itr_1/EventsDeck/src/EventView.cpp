#include "EventView.h"
#include <OpenGL/OpenGL.h>

namespace next {
    EventView::EventView(Event* data){
        mSpeakerStackView = new SpeakerStackView();
        reset(data);
    }
    
    void EventView::reset(Event* data) {
        mData = data;
        mSpeakerStackView->reset(mData->getSpeakers());
    }
    
    EventView::~EventView(){
        delete mSpeakerStackView;
    }

    void EventView::draw(){
        Vec3f pos = mPosState();

        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        mSpeakerStackView->draw();
        glPopMatrix();
    }
    
    void EventView::update(){
        mSpeakerStackView->update();
    }
    
    void EventView::nextSpeaker(){
        mSpeakerStackView->next();
    }
    
    void EventView::focusTop(){
        mSpeakerStackView->focus();
    }

}
