//
//  EventView.h
//  EventsDeck
//
//  Created by Henryk Wollik on 15/04/14.
//
//

#ifndef EventsDeck_EventView_h
#define EventsDeck_EventView_h

#include "data/session/Event.h"
#include "stage/session/AbstractAnimBase.h"
#include "stage/session/SpeakerStackView.h"
#include "cinder/Timeline.h"
#include "cinder/Vector.h"
#include "cinder/gl/Material.h"

namespace next {
    using namespace ci;
    
    class EventView : public AbstractAnimBase{
        friend class SessionView;

        Event* mData;
        SpeakerStackView* mSpeakerStackView;
        
        void reset(Event* data);
        void focusTop();
        void unfocus();

        Anim<Vec3f> mPositionState;

    public:
        EventView(Event* data);
        ~EventView();
        
        void draw();
        void update();

        void stackSpeaker(const std::function<void(int)>& callbackUpdate,const AnimCallback &callbackFinish);
        void resetStack();
        
        void focusIn();
        void unfocusOut();
        
        void loadMaterialProperties();
    };
}


#endif
