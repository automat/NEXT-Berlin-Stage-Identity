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
        int      mNumData;
        int      mDataIndex;
        Session* mData;
        
        size_t             mNumViews;
        EventView*         mViews[4]; // prev, current, next , buffer
        
        int mViewIndexPrev;    //  index previous view
        int mViewIndexCurr;    //  index current view
        int mViewIndexNext;    //  index next view
        
        
        Vec3f mCurrEventPos;
        Vec3f mPrevEventPos;
        Vec3f mNextEventPos;
        Vec3f mPrevEventPosOut;
        Vec3f mNextEventPosOut;
        
        
        
        void stackSpeakers(EventView* view);
        
        void animateInit();
        void animateNextIn(EventView* view);        //  animate next view in
        void animateNextOutIn(EventView* view);     //  animate next next view in
        
        void animatePrevOut(EventView* view);       //  animate prev view out
        void animatePrevOutOut(EventView* view);    //  animate prev prev view out
        void resetEventView(EventView* view);       //  reset view with next data
        
        
        void deleteEventViews();
    public:
        SessionView(Session* data);
        ~SessionView();
        
        void reset(Session* data);
        
        void draw();
        void update();
        
    
        
        void debugDraw();
        
        void next(); // for debug
        void init();
    };
}

#endif
