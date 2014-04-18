#include "SessionView.h"
#include "Event.h"

#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

namespace next {
    using namespace boost::assign;
    using namespace ci;
    using namespace ci::app;
    
    const int SessionView::sMinCyclicBufferLen(5);
    
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
        mBufferViewLen(0),
        mBufferViewIndex(-1),
        mBufferViewValid(false),
        mNumData(0),
        mDataIndex(-1),
        mBufferViewStep(-1){
            float slideLength_2 = sSlideLength * 0.5f;
        
            mPrevEventPos    = Vec3f(0,0, slideLength_2);
            mNextEventPos    = Vec3f(0,0,-slideLength_2);
            mPrevEventPosOut = mPrevEventPos * 2;
            mNextEventPosOut = mNextEventPos * 2;
            
            mEventPositions[0] = Vec3f(0,0,-sSlideLength );
            mEventPositions[1] = Vec3f(0,0,-slideLength_2);
            mEventPositions[3] = Vec3f(0,0, slideLength_2);
            mEventPositions[4] = Vec3f(0,0, sSlideLength );
            
        
            reset(data);
    }
    
    SessionView::~SessionView(){
        deleteEventViews();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Handle Data
    /*--------------------------------------------------------------------------------------------*/
 
    void SessionView::deleteEventViews(){
        int i = -1;
        while (++i < mBufferViewLen) {
            if(mBufferView[i] != NULL){
                delete mBufferView[i];
            }
        }
    }
    
    void SessionView::reset(Session* data){
        deleteEventViews();
        
        mData            = data;
        mNumData         = -1;
        mDataIndex       = -1;
        mBufferViewIndex = -1;
        mBufferViewLen   = -1;
        mBufferViewValid = !data->getEvents()->empty();
        mBufferViewStep  = -1;
        
        if(!mBufferViewValid){
            return;
        }
        
        vector<Event>* events = mData->getEvents();
        mNumData         = events->size();
        mBufferViewLen   = min(sMinCyclicBufferLen,mNumData);
        
        int i = -1;
        while (++i < mBufferViewLen) {
            mBufferView[i] = new EventView(&(*events)[i]);
            mBufferView[i]->mPositionState = mNextEventPosOut;
        }
        
        //start();
        next();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Animation Trigger
    /*--------------------------------------------------------------------------------------------*/
    
    /*
    void SessionView::next(){
        if(!mBufferViewValid){
            return;
        }
        
        if(mDataIndex < mNumData + 2){
            if(mBufferViewIndex > -1){
                animatePrevOut(mBufferView[mBufferViewIndex]);
            }
        }
        
        cout << mDataIndex << endl;
        
        mDataIndex++;
        if(mDataIndex >= mNumData){
            return;
        }
        
        mBufferViewIndex = (mBufferViewIndex + 1) % mBufferViewLen;
        animateNextIn(mBufferView[mBufferViewIndex]);
        
        if(mDataIndex < mNumData - 1){
            animateNextOutIn(mBufferView[(mBufferViewIndex + 1) % mBufferViewLen]);
        }
    }*/
    
    void SessionView::next(){
        if(!mBufferViewValid){
            return;
        }
        
        cout << mBufferViewStep << " / " << mNumData << endl;
        
        if(mBufferViewStep >= mNumData){
            return;
        }
        
        mBufferViewStep++;
        
        
        
        int bufferStepRel;
        int i = -1;
        while(++i < mBufferViewLen){
            bufferStepRel = MAX(0, (mBufferViewStep - i) % 5);
            if(bufferStepRel == 0){ // if end, hard reset to front
                mBufferView[i]->mPositionState = mEventPositions[0];
            } else { // tween to new pos
                tween(&mBufferView[i]->mPositionState, mEventPositions[bufferStepRel], 1.0f);
            }
        }
        
    }
    
    void SessionView::start(){
        animateStart();
        
    }
    
    void SessionView::triggerNext(){
        mBufferView[mBufferViewIndex]->stackSpeaker(std::bind(&SessionView::next, this));
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Animation
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::animateStart(){
        tween(&mBufferView[0]->mPositionState, mNextEventPos, sTimeAnimateStart, ViewInOutEasing(),
              NULL, std::bind(&SessionView::next, this));
    }
    
    void SessionView::animatePrevOut(EventView *view){
        tween(&view->mPositionState, mPrevEventPos, sTimeAnimateOut, ViewInOutEasing());
        
        view->unfocus();
        
        if (mDataIndex < mNumData) {
            if(mDataIndex != 0){
                int prevBufferIndex = (mBufferViewIndex - 1) % mBufferViewLen;
                if(prevBufferIndex > -1){
                    animatePrevOutOut(mBufferView[prevBufferIndex]);
                }
            }
        } else {
            tween(&view->mPositionState, mPrevEventPosOut, sTimeAnimateOutOut, ViewInOutEasing(),
                  NULL, std::bind(&SessionView::resetBufferView,this,view));
        }
    }
    
    void SessionView::animatePrevOutOut(EventView* view){
        tween(&view->mPositionState, mPrevEventPosOut, sTimeAnimateOutOut, ViewInOutEasing(),
              NULL, std::bind(&SessionView::resetBufferView,this,view));
    }
    
    void SessionView::animateNextIn(EventView* view){
        tween(&view->mPositionState, mCurrEventPos, sTimeAnimateIn, ViewInOutEasing());
        view->focusTop();
    }
    
    void SessionView::animateNextOutIn(EventView* view){
        tween(&view->mPositionState, mNextEventPos, sTimeAnimateOutIn, ViewInOutEasing());
    }
    
    void SessionView::resetBufferView(EventView* view){
        view->mPositionState = mNextEventPosOut;
        int nextDataIndex = mDataIndex + 2; //  next after next
        if(nextDataIndex >= mNumData){
            return;
        }
        view->reset(&(*mData->getEvents())[nextDataIndex]);
        triggerNext();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::draw(){
        if(!mBufferViewValid){
            return;
        }
        
        int  i = -1;
        while (++i < mBufferViewLen) {
            mBufferView[i]->draw();
        }
    }
    
    void SessionView::update(){
        if(!mBufferViewValid){
            return;
        }
        
        int  i = -1;
        while (++i < mBufferViewLen) {
            mBufferView[i]->update();
        }
    }
    
    void SessionView::debugDraw(){
        glColor3f(1,0,0);
        gl::drawLine(mPrevEventPosOut, mPrevEventPos);
        gl::drawLine(mPrevEventPos, mCurrEventPos);
        gl::drawSphere(mPrevEventPosOut, 0.025f);
        gl::drawSphere(mPrevEventPos, 0.025f);
        glColor3f(1,0,1);
        gl::drawLine(mNextEventPos, mCurrEventPos);
        gl::drawLine(mNextEventPosOut, mNextEventPos);
        gl::drawSphere(mNextEventPos, 0.025f);
        gl::drawSphere(mNextEventPosOut, 0.025f);
        
    }
}