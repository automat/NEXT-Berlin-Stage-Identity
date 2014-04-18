//
//  SessionView.h
//  EventsDeck
//
//  Created by Henryk Wollik on 16/04/14.
//
//

#ifndef EventsDeck_SessionView_h
#define EventsDeck_SessionView_h

#include "AbstractAnimView.h"
#include "Session.h"
#include "EventView.h"

#include "cinder/Vector.h"


namespace next {
    using namespace ci;
    
    class SessionView : public AbstractAnimView{
        bool     mValid;
        int      mNumData;      //  length of session data / num events
        Session* mData;         //  data ref

        int                mNumEventViews;
        vector<EventView*> mEventViews;
        vector<int>        mEventViewsOffset;
        int                mEventViewStep;
        int                mEventViewFront;
        int                mEventViewBack;


        Vec3f       mEventViewSlots[5];
        const int   mEventViewSlotBegin;     // begin
        const int   mEventViewSlotEnd;       // end
        const int   mEventViewSlotFocus;
        const int   mEventViewSlotUnfocusPrev;
        const int   mEventViewSlotUnfocusNext;


        void callbackTest();
        
        void animateStart();


        void animateNextIn(EventView* view);        //  animate next view in
        void animateNextOutIn(EventView* view);     //  animate next next view in
        void animatePrevOut(EventView* view);       //  animate prev view out
        void animatePrevOutOut(EventView* view);    //  animate prev prev view out
        
        void resetBufferView(EventView* view);       //  reset view with next data
        void deleteEventViews();
        
        void triggerNext();

        void moveViews(int count = 1, int direction = 1);
        void setViewState(EventView* view, int slot, int direction = 1);

    public:
        SessionView(Session* data);
        ~SessionView();
        
        void reset(Session* data);  //  reset session with new data
        
        void draw();
        void update();
        
        void debugDraw();
        
        void next(); // for debug
        void prev();
        void start();   //  start animation
    };
}

#endif
