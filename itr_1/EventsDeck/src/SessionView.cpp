#include "SessionView.h"
#include "Event.h"

#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/Utilities.h"
#include <math.h>

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "Config.h"

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
        
        vector<Event>* events = data->getEvents();
        
        mNumEventViews = events->size();
        mEventViewStep = mEventViewFront = mEventViewBack = 0;
        mValid         = mNumEventViews != 0;

        mAnimating = false;

        if(!mValid){
            return;
        }

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
        if((mNumEventViews < 2) ||
           (mEventViewFront >= mNumEventViews - 1)){
            return;
        }
        moveViews();
    }

    void SessionView::prev(){
        if((mNumEventViews < 2) ||
           (mEventViewStep <= 0)){
            return;
        }
        moveViews(1,-1);
    }

    void SessionView::start(){
#ifdef SESSION_VIEW_DEBUG_STATE
        cout << "### Session View Start ###" << endl;
#endif
        
        if(mNumEventViews < 2){
            moveViews(2, 1);
            return;
        }
        moveViews();
    }
    
    void SessionView::onFinish(){
#ifdef SESSION_VIEW_DEBUG_STATE
        cout << "### Session View End ###" << endl;
#endif
    }
 

    void SessionView::moveViews(int count, int direction) {
        if(!mValid ||
            mEventViewFront >= mNumEventViews ||
            mEventViewFront <= -1){
            return;
        }

        int slot;
        EventView* front;

        int i = -1;
        while(++i < count){
            mEventViewStep = mEventViewStep + direction;
            
            if(direction < 0 && mEventViewFront > 0){
                mEventViewFront--;
            }
            
            slot = CLAMP(mEventViewsOffset[mEventViewFront] + mEventViewStep, 0, mEventViewSlotEnd);
            front = mEventViews[mEventViewFront];
            setViewState(front, slot, direction);
    
            int j= mEventViewFront;
            while (++j < mNumEventViews) {
                slot = CLAMP(mEventViewsOffset[j] + mEventViewStep, 0, mEventViewSlotEnd);
       
                if(direction > 0 && slot == mEventViewSlotEnd){
                    mEventViewFront++;
                }
                setViewState(mEventViews[j], slot, direction);
            }
        }
        
#ifdef SESSION_VIEW_DEBUG_STATE
        cout << "Front: " << mEventViewFront << endl;
#endif
    }
    
    void SessionView::focusView(next::EventView *view){
        view->stackSpeaker(std::bind(&SessionView::next, this));
    }
    
    
    void SessionView::setViewState(EventView *view, int slot, int direction) {
        if(direction > 0){  //  forward
            if(slot == mEventViewSlotFocus){ //  current view is focused
                view->focusTop();
                tween(&view->mPositionState, mEventViewSlots[slot], sTimeAnimateInOut, ViewInOutEasing(),
                      NULL, std::bind(&SessionView::focusView,this,view));
            } else { // current view isnt focused
                if(slot == mEventViewSlotUnfocusPrev){
                    view->unfocus();
                    if(view == mEventViews.back()){ //  last view on out
                        tween(&view->mPositionState, mEventViewSlots[slot], sTimeAnimateInOut, ViewInOutEasing(),
                              NULL, std::bind(&SessionView::next, this));
                    } else {    // some other view
                        tween(&view->mPositionState, mEventViewSlots[slot], sTimeAnimateInOut, ViewInOutEasing());
                    }
                } else {
                    
                    tween(&view->mPositionState, mEventViewSlots[slot], sTimeAnimateInOut, ViewInOutEasing());
                }
            }
        } else {    //  backward
            if(slot == mEventViewSlotUnfocusNext){
                view->unfocus();
            }
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
        const static float fontScale = 0.0125f;
        Vec3f pos;
        glColor3f(1, 1, 1);
        
        gl::enableAlphaTest();
        gl::enableAlphaBlending();
        int i = -1;
        while (++i< mNumEventViews) {
            pos = mEventViews[i]->mPositionState();
            glPushMatrix();
            glTranslatef(pos.x - 1, pos.y, pos.z);
            glRotatef(90, 0, 0, 0);
            glScalef(fontScale, fontScale, fontScale);
            gl::drawString(toString(i),Vec2f::zero());
            glPopMatrix();
            
        }
        gl::disableAlphaBlending();
        gl::disableAlphaTest();
        
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