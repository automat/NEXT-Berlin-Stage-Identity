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
        Session* mData;
        
        size_t             mNumViews;
        vector<EventView*> mViews;
        
        int mViewIndexPrev;    //  index previous view
        int mViewIndexCurr;    //  index current view
        int mViewIndexNext;    //  index next view
        
        
        Vec3f mCurrEventPos;
        Vec3f mPrevEventPos;
        Vec3f mNextEventPos;
        Vec3f mPrevEventPosOut;
        Vec3f mNextEventPosOut;
        
        
        void resetEventView();
        
        void stackSpeakers(EventView* view);
        
        void animateIn(EventView* view);
        void animateOut(EventView* view);
        
        void deleteEventViews();
    public:
        SessionView(Session* data);
        ~SessionView();
        
        void reset(Session* data);
        
        void draw();
        void update();
        
        void debugDraw();
        
        void next(); // for debug
    };
}

#endif
