#include "SessionView.h"
#include "Event.h"

#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include <math.h>

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#define CLAMP(a,b,c)({\
    MIN(MAX(b,a),c); \
})

namespace next {
    using namespace boost::assign;
    using namespace ci;
    using namespace ci::app;
    
    static const float sSlideLength(4);
    
    //  to be replaced with config
    static const float sTimeAnimateIn(2.25f);
    static const float sTimeAnimateOutIn(2.25f);
    static const float sTimeAnimateOut(2.25f);
    static const float sTimeAnimateOutOut(2.25f);
    
    static const float sTimeAnimateStart(1.0f);
    static const float sTimeAnimateEnd(1.0f);

    typedef EaseOutCubic ViewInOutEasing;
    
    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    SessionView::SessionView(Session* data) :
        AbstractAnimView(),
        mValid(false),
        mNumData(0),
        mEventViewStep(0),
        mEventViewFront(0),
        mEventViewSlotBegin(0),
        mEventViewSlotFocus(2),
        mEventViewSlotUnfocusNext(1),
        mEventViewSlotUnfocusPrev(3),

        mEventViewSlotEnd(4){
            float slideLength_2 = sSlideLength * 0.5f;


            mEventViewSlots[0] = Vec3f(0,0,-sSlideLength );
            mEventViewSlots[1] = Vec3f(0,0,-slideLength_2);
            mEventViewSlots[3] = Vec3f(0,0, slideLength_2);
            mEventViewSlots[4] = Vec3f(0,0, sSlideLength );
            
        
            reset(data);
    }
    
    SessionView::~SessionView(){
        deleteEventViews();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Handle Data
    /*--------------------------------------------------------------------------------------------*/
 
    void SessionView::deleteEventViews(){
        while(!mEventViews.empty()) delete mEventViews.back(), mEventViews.pop_back();
    }
    
    void SessionView::reset(Session* data){
        deleteEventViews();
        
        mData    = data;
        mNumData = mData->getEvents()->size();
        mValid   = mNumData != 0;
        mEventViewStep = mEventViewFront = mEventViewBack = 0;

        if(!mValid){
            return;
        }
        
        vector<Event>* events = mData->getEvents();
        mNumEventViews        = mNumData;

        int i = -1;
        while (++i < mNumEventViews) {
            mEventViews += new EventView(&(*events)[i]);
            mEventViews.back()->mPositionState = mEventViewSlots[0];
            mEventViewsOffset += -i;
        }

        start();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Animation Trigger
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::callbackTest(){
        cout << "fsdf" << endl;
        mEventViews[mEventViewFront]->stackSpeaker(std::bind(&SessionView::next,this));
    }
    
    
    void SessionView::next(){
        if(mNumEventViews < 2){
            return;
        }
        moveViews(1);
    }

    void SessionView::prev(){
        if(mNumEventViews < 2){
            return;
        }
        moveViews(1,-1);
    }

    void SessionView::start(){
        if(mNumEventViews < 2){
            moveViews(2, 1);
            return;
        }

        cout << "###" << endl;
        moveViews(1);

        cout << mEventViewFront << endl;


    }

    void SessionView::moveViews(int count, int direction) {
        if(!mValid ||
            mEventViewFront >= mNumEventViews ||
            mEventViewFront <= -1){
            return;
        }

        int slot;
        EventView* front;
        EventView* view;

        int i = -1;
        while(++i < count){
            mEventViewStep = mEventViewStep + direction;

            slot = CLAMP(mEventViewsOffset[mEventViewFront] + mEventViewStep, 0, mEventViewSlotEnd);

            front = mEventViews[mEventViewFront];
            setViewState(front, slot, direction);
            tween(&front->mPositionState, mEventViewSlots[slot], 2.0f, ViewInOutEasing());


            int j= mEventViewFront;
            while (++j < mNumEventViews) {
                slot = CLAMP(mEventViewsOffset[j] + mEventViewStep, 0, mEventViewSlotEnd);

                mEventViewFront += static_cast<int>(slot == mEventViewSlotEnd) * direction;

                view = mEventViews[j];
                setViewState(view, slot, direction);
                tween(&view->mPositionState, mEventViewSlots[slot], 2.0f, ViewInOutEasing());
            }
        }
    }


    void SessionView::setViewState(EventView *view, int slot, int direction) {
        if(slot == mEventViewSlotFocus){
            view->focusTop();
        } else if(direction == 1 && slot == mEventViewSlotUnfocusPrev){
            view->unfocus();
        } else if(direction == -1 && slot == mEventViewSlotUnfocusNext){
            view->unfocus();
        }
    }

    void SessionView::triggerNext(){
       // mEventViews[mBufferViewIndex]->stackSpeaker(std::bind(&SessionView::next, this));
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Animation
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::animateStart(){
        /*
        tween(&mEventViews[0]->mPositionState, mNextEventPos, sTimeAnimateStart, ViewInOutEasing(),
              NULL, std::bind(&SessionView::next, this));
              */
    }
    
    void SessionView::animatePrevOut(EventView *view){
        /*
        tween(&view->mPositionState, mPrevEventPos, sTimeAnimateOut, ViewInOutEasing());
        
        view->unfocus();
        
        if (mDataIndex < mNumData) {
            if(mDataIndex != 0){
                int prevBufferIndex = (mBufferViewIndex - 1) % mBufferViewLen;
                if(prevBufferIndex > -1){
                    animatePrevOutOut(mEventViews[prevBufferIndex]);
                }
            }
        } else {
            tween(&view->mPositionState, mPrevEventPosOut, sTimeAnimateOutOut, ViewInOutEasing(),
                  NULL, std::bind(&SessionView::resetBufferView,this,view));
        }
        */
    }
    
    void SessionView::animatePrevOutOut(EventView* view){
        /*
        tween(&view->mPositionState, mPrevEventPosOut, sTimeAnimateOutOut, ViewInOutEasing(),
              NULL, std::bind(&SessionView::resetBufferView,this,view));
              */
    }
    
    void SessionView::animateNextIn(EventView* view){
        /*
        tween(&view->mPositionState, mCurrEventPos, sTimeAnimateIn, ViewInOutEasing());
        view->focusTop();  */
    }
    
    void SessionView::animateNextOutIn(EventView* view){
        /*
        tween(&view->mPositionState, mNextEventPos, sTimeAnimateOutIn, ViewInOutEasing());
        */
    }
    
    void SessionView::resetBufferView(EventView* view){
        /*
        view->mPositionState = mNextEventPosOut;
        int nextDataIndex = mDataIndex + 2; //  next after next
        if(nextDataIndex >= mNumData){
            return;
        }
        view->reset(&(*mData->getEvents())[nextDataIndex]);
        triggerNext();
        */
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::draw(){
        if(!mValid){
            return;
        }
        
        for (vector<EventView*>::const_iterator itr = mEventViews.begin(); itr != mEventViews.end(); itr++) {
            (*itr)->draw();
        }
    }
    
    void SessionView::update(){
        if(!mValid){
            return;
        }
        
        for (vector<EventView*>::const_iterator itr = mEventViews.begin(); itr != mEventViews.end(); itr++) {
            (*itr)->update();
        }
    }
    
    void SessionView::debugDraw(){
        glColor3f(1,0,0);
        gl::drawLine(mEventViewSlots[0], mEventViewSlots[1]);
        gl::drawLine(mEventViewSlots[1], mEventViewSlots[2]);
        gl::drawSphere(mEventViewSlots[0], 0.025f);
        gl::drawSphere(mEventViewSlots[1], 0.025f);
        glColor3f(1,0,1);
        gl::drawLine(mEventViewSlots[2], mEventViewSlots[3]);
        gl::drawLine(mEventViewSlots[3], mEventViewSlots[4]);
        gl::drawSphere(mEventViewSlots[3], 0.025f);
        gl::drawSphere(mEventViewSlots[4], 0.025f);
        
    }
}