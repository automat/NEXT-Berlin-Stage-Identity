//
//  EventView.h
//  EventsDeck
//
//  Created by Henryk Wollik on 15/04/14.
//
//

#ifndef EventsDeck_EventView_h
#define EventsDeck_EventView_h

#include "Event.h"
#include "SpeakerStackView.h"

namespace next {
    class EventView {
        Event* mData;
        SpeakerStackView* mSpeakerStackView;
    
        void reset(Event* data);
        void deleteSpeakerStackView();
       
        
        
    public:
        EventView(Event* data);
        ~EventView();
        
        void drawAlphaBlended();
        void draw();
        void update();
        
        
        void nextSpeaker(); // for debug
        
    };
}


#endif
