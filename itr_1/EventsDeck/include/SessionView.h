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
        const static int sMinCyclicBufferLen;   //  minimum number of events to use cyclic repeating buffered event views
        
        int      mNumData;      //  length of session data / num events
        int      mDataIndex;    //  current focused event
        Session* mData;         //  data ref
        
        size_t      mBufferViewLen;   //  length of buffer
        EventView*  mBufferView[5];   //  prev, current, next , buffer
        int         mBufferViewIndex; //  index current buffer view
        bool        mBufferViewValid;
        int         mBufferViewStep;
        
        Vec3f mCurrEventPos;    //  ref position focused event
        Vec3f mPrevEventPos;    //  ref position previous event
        Vec3f mNextEventPos;    //  ref position next event
        Vec3f mPrevEventPosOut; //  ref position previous event outside visible area
        Vec3f mNextEventPosOut; //  ref position next event outside visible area
        
        Vec3f mEventPositions[5];
        
        
        void animateStart();
        void animateEnd();
        
        void animateNextIn(EventView* view);        //  animate next view in
        void animateNextOutIn(EventView* view);     //  animate next next view in
        void animatePrevOut(EventView* view);       //  animate prev view out
        void animatePrevOutOut(EventView* view);    //  animate prev prev view out
        
        void resetBufferView(EventView* view);       //  reset view with next data
        void deleteEventViews();
        
        void triggerStart();
        void triggerNext();
    
    public:
        SessionView(Session* data);
        ~SessionView();
        
        void reset(Session* data);  //  reset session with new data
        
        void draw();
        void update();
        
        void debugDraw();
        
        void next(); // for debug
        void start();   //  start animation
    };
}

#endif
