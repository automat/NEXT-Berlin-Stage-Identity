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
#include "AbstractAnimView.h"
#include "SpeakerStackView.h"
#include "cinder/Timeline.h"
#include "cinder/Vector.h"

namespace next {
    using namespace ci;
    
    class EventView : public AbstractAnimView{
        friend class SessionView;
        
        Anim<Vec3f> mPosState;
        
        Event* mData;
        SpeakerStackView* mSpeakerStackView;
        
        void reset(Event* data);
        void focusTop();
        void unfocus();
        
    public:
        EventView(Event* data);
        ~EventView();
        
        void draw();
        void update();
        
        void nextSpeaker(const AnimCallback& callback); // for debug
        
    };
}


#endif
