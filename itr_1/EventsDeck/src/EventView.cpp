#include "EventView.h"

namespace next {
    EventView::EventView(Event* data) :
        AbstractAnimView(){
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
        Vec3f pos = mPositionState();

        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        mSpeakerStackView->draw();
        glPopMatrix();
    }
    
    void EventView::update(){
        mSpeakerStackView->update();
    }
    
    void EventView::stackSpeaker(const AnimCallback &callback){
        mSpeakerStackView->next(callback);
    }
    
    void EventView::focusTop(){
        mSpeakerStackView->focus();
    }
    
    void EventView::unfocus(){
        mSpeakerStackView->unfocus();
    }

}
