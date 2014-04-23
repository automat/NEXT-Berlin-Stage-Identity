#include "EventView.h"

namespace next {
    EventView::EventView(Event* data) :
        AbstractAnimView(){
        mSpeakerStackView = new SpeakerStackView();
        reset(data);
    }
    
    void EventView::reset(Event* data) {
        mData = data;
        mSpeakerStackView->reset(mData->speakers);
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

    void EventView::stackSpeaker(const std::function<void(int)>& callbackUpdate,
                                 const AnimCallback &callbackFinish){
        mSpeakerStackView->stack(callbackUpdate, callbackFinish);
    }
    
    void EventView::focusTop(){
        mSpeakerStackView->focus();
    }
    
    void EventView::unfocus(){
        mSpeakerStackView->unfocus();
    }
    
    void EventView::resetStack(){
        mSpeakerStackView->resetStack();
    }
}
