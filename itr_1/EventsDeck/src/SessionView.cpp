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
    
    const int SessionView::sMinCyclicBufferLen(4);
    
    static const float sSlideLength(3);
    
    //  to be replaced with config
    static const float sTimeAnimateIn(2.25f);
    static const float sTimeAnimateOutIn(2.25f);
    static const float sTimeAnimateOut(2.25f);
    static const float sTimeAnimateOutOut(2.25f);
    
    static const float sTimeAnimateStart(1.0f);
    static const float sTimeAnimateEnd(1.0f);
    
    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    SessionView::SessionView(Session* data) :
        AbstractAnimView(),
        mBufferViewLen(0),
        mBufferViewIndex(-1),
        mBufferViewValid(false),
        mNumData(0),
        mDataIndex(-1){
            float slideLength   = 3;
            float slideLength_2 = slideLength * 0.5f;
        
            mPrevEventPos    = Vec3f(0,0, slideLength_2);
            mNextEventPos    = Vec3f(0,0,-slideLength_2);
            mPrevEventPosOut = mPrevEventPos * 2;
            mNextEventPosOut = mNextEventPos * 2;
        
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
        
        mAnimating = false;
        
        mData            = data;
        mNumData         = -1;
        mDataIndex       = -1;
        mBufferViewIndex = -1;
        mBufferViewLen   = -1;
        mBufferViewValid = !data->getEvents()->empty();
        
        if(!mBufferViewValid){
            return;
        }
        
        vector<Event>* events = mData->getEvents();
        mNumData         = events->size();
        mBufferViewLen   = min(sMinCyclicBufferLen,mNumData);
        
        int i = -1;
        while (++i < mBufferViewLen) {
            mBufferView[i] = new EventView(&(*events)[i]);
            mBufferView[i]->mPosState = mNextEventPosOut;
        }
        
        start();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Animation Trigger
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::next(){
        if(mAnimating || !mBufferViewValid){
            return;
        }
        
        if(mDataIndex < mNumData + 2){
            if(mBufferViewIndex > -1){
                animatePrevOut(mBufferView[mBufferViewIndex]);
            }
        }
        
        mDataIndex++;
        if(mDataIndex >= mNumData){
            return;
        }
        
        mBufferViewIndex = (mBufferViewIndex + 1) % mBufferViewLen;
        animateNextIn(mBufferView[mBufferViewIndex]);
        
        if(mDataIndex < mNumData -1){
            animateNextOutIn(mBufferView[(mBufferViewIndex + 1) % mBufferViewLen]);
        }
    }
    
    void SessionView::start(){
        animateStart();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Animation
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::animateStart(){
        mAnimating = true;
        Timeline& _timeline = timeline();
        
        _timeline.apply(&mBufferView[0]->mPosState, mNextEventPos, sTimeAnimateStart, EaseOutCubic())
                 .finishFn(std::bind(&SessionView::animateFinish, this));
    }
    
    void SessionView::animatePrevOut(EventView *view){
        mAnimating = true;
        Timeline& _timeline = timeline();
        _timeline.apply(&view->mPosState, mPrevEventPos, sTimeAnimateOut, EaseOutCubic())
                 .finishFn(std::bind(&SessionView::animateFinish,this));
        view->unfocus();
        
        if (mDataIndex < mNumData) {
            if(mDataIndex != 0){
                int prevBufferIndex = (mBufferViewIndex - 1) % mBufferViewLen;
                if(prevBufferIndex > -1){
                    animatePrevOutOut(mBufferView[prevBufferIndex]);
                }
            }
        } else {
            _timeline.apply(&view->mPosState, mPrevEventPosOut, sTimeAnimateOutOut, EaseOutCubic())
                     .finishFn(std::bind(&SessionView::resetBufferView,this,view));
        }
        
    }
    
    void SessionView::animatePrevOutOut(EventView* view){
        mAnimating = true;
        Timeline& _timeline = timeline();
        _timeline.apply(&view->mPosState, mPrevEventPosOut, sTimeAnimateOutOut, EaseOutCubic())
                 .finishFn(std::bind(&SessionView::resetBufferView,this,view));
    }
    
    void SessionView::animateNextIn(EventView* view){
        mAnimating = true;
        Timeline& _timeline = timeline();
        _timeline.apply(&view->mPosState, mCurrEventPos, sTimeAnimateIn, EaseOutCubic())
                 .finishFn(std::bind(&SessionView::animateFinish,this));
        view->focusTop();
    }
    
    void SessionView::animateNextOutIn(EventView* view){
        mAnimating = true;
        Timeline& _timeline = timeline();
        _timeline.apply(&view->mPosState, mNextEventPos, sTimeAnimateOutIn, EaseOutCubic())
                 .finishFn(std::bind(&SessionView::animateFinish,this));
    }
    
    void SessionView::resetBufferView(EventView* view){
        view->mPosState = mNextEventPosOut;
        int nextDataIndex = mDataIndex + 2; //  next after next
        if(nextDataIndex >= mNumData){
            return;
        }
        view->reset(&(*mData->getEvents())[nextDataIndex]);
    }
    
    void SessionView::showSpeakers(EventView* view){
        
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