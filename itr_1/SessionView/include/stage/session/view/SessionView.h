//
//  SessionView.h
//  EventsDeck
//
//  Created by Henryk Wollik on 16/04/14.
//
//

#ifndef EventsDeck_SessionView_h
#define EventsDeck_SessionView_h

#include "stage/session/view/AbstractAnimView.h"
#include "stage/session/model/Session.h"
#include "stage/session/view/EventView.h"

#include "stage/session/view/label/session/SessionMetaLabel.h"
#include "stage/session/view/label/session/SessionTitleLabel.h"
#include "stage/session/view/label/event/EventMetaLabel.h"
#include "stage/session/view/label/event/PingPongEventTitleLabel.h"
#include "stage/session/view/label/speaker/PingPongSpeakerLabel.h"

#include "cinder/Vector.h"

namespace next {
    using namespace ci;
    
    class SessionView : public AbstractAnimView{
        Session* mData;
        
        bool mValid;    //  is valid, data > 0
        bool mAnimating;
        
        int                mNumEventViews;
        int                mIndexEventViews;
        vector<EventView*> mEventViews;
        vector<int>        mEventViewsOffset;
        int                mEventViewStep;
        int                mEventViewFront;
        int                mEventViewBack;

        Vec3f     mEventViewSlots[5];
        const int mEventViewSlotBegin;
        const int mEventViewSlotEnd;
        const int mEventViewSlotFocus;
        const int mEventViewSlotOut;
        const int mEventViewSlotIn;
        
        Vec3f mSpeakerLabelPos;

        //! clear all views
        void deleteEventViews();
        //! move all views in steps and direction
        void moveViews(int count = 1);
        //!
        void stepForward_1();
        void stepForward_2();

        //! update view state
        void setViewState(EventView* view, int slot);
        //! focus view
        void focusView(EventView* view);
        //! on all events shown
        void onFinish();
        //! on begin
        void onStart();
        
        void finish();
        
        void resetEventViews();
        
        void turnOnSessionLabels();
        void turnOffSessionLabels();
        
        SessionTitleLabel*         mLabelTitle;
        SessionMetaLabel*          mLabelMeta;
        PingPongEventTitleLabel*   mPingPongLabelEventTitle;
        EventMetaLabel*            mLabelEventMeta;
        PingPongSpeakerLabel*      mPingPongLabelSpeaker;
        
        void updateSpeakerLabel(int index = 0);
        
        
    public:
        SessionView(Session* data);
        ~SessionView();
        
        void reset(Session* data);  //  reset session with new data
        void start();               //  start animation


        void draw();
        void drawLabels();
        
        void debugDraw();
        void update();

    };
}

#endif
