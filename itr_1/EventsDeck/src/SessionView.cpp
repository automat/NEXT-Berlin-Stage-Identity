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
    static const float sTimeAnimateInOut(2.0f);
    
    static const float sTimeAnimateStartEnd(1.0f);

    typedef EaseOutCubic ViewStartEndEasing;
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
        moveViews();
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
        moveViews();
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
            tween(&front->mPositionState, mEventViewSlots[slot], sTimeAnimateInOut, ViewInOutEasing());
            
            int j= mEventViewFront;
            while (++j < mNumEventViews) {
                slot = CLAMP(mEventViewsOffset[j] + mEventViewStep, 0, mEventViewSlotEnd);
       
                if(direction > 0){
                    if(slot == mEventViewSlotEnd){
                        mEventViewFront++;
                    }
                } else {
                    if(mEventViewFront > 0){
                        mEventViewFront--;
                    }
                }
                
                view = mEventViews[j];
                setViewState(view, slot, direction);
                tween(&view->mPositionState, view->mPositionState(), mEventViewSlots[slot], sTimeAnimateInOut, ViewInOutEasing());
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
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::draw(){
        for (vector<EventView*>::const_iterator itr = mEventViews.begin(); itr != mEventViews.end(); itr++) {
            (*itr)->draw();
        }
    }
    
    void SessionView::update(){
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