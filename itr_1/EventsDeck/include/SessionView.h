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
        bool     mValid;    //  is valid, data > 0

        bool mAnimating;
        
        int                mNumEventViews;
        vector<EventView*> mEventViews;
        vector<int>        mEventViewsOffset;
        int                mEventViewStep;
        int                mEventViewFront;
        int                mEventViewBack;


        Vec3f       mEventViewSlots[5];
        const int   mEventViewSlotBegin;
        const int   mEventViewSlotEnd;
        const int   mEventViewSlotFocus;
        const int   mEventViewSlotUnfocusPrev;
        const int   mEventViewSlotUnfocusNext;


        void callbackTest();

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
      
        
    public:
        SessionView(Session* data);
        ~SessionView();
        
        void reset(Session* data);  //  reset session with new data
        
        void draw();
        void update();
        
        void debugDraw();

        void start();   //  start animation
    };
}

#endif
