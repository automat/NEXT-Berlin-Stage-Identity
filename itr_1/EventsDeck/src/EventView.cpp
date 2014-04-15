#include "EventView.h"

namespace next {
    EventView::EventView(Event* data){
        mSpeakerStackView = new SpeakerStackView();
        reset(data);
    }
    
    void EventView::deleteSpeakerStackView(){
        delete mSpeakerStackView;
    }
    
    void EventView::reset(Event* data) {
        mData = data;
        mSpeakerStackView->reset(mData->getSpeakers());
    }
    
    EventView::~EventView(){
        delete mSpeakerStackView;
    }
    
    void EventView::draw(){
        mSpeakerStackView->draw();
    }
    
    void EventView::update(){
        mSpeakerStackView->update();
    }
    
    void EventView::nextSpeaker(){
        mSpeakerStackView->next();
    }

}
