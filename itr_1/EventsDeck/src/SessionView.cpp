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
    
    //  to be replaced with config
    static const float sTimeAnimateIn(2.25f);
    static const float sTimeAnimateOutIn(2.25f);
    static const float sTimeAnimateOut(2.25f);
    static const float sTimeAnimateOutOut(2.25f);
    
    static const float sTimeAnimateInit(1.0f);
    
    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    SessionView::SessionView(Session* data) :
        AbstractAnimView(),
        mNumViews(0),
        mNumData(0),
        mDataIndex(-1),
        mViewIndexPrev(-1),
        mViewIndexCurr(-1),
        mViewIndexNext(-1){
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
        while (++i < mNumViews) {
            if(mViews[i] != NULL){
                delete mViews[i];
            }
        }
    }
    
    void SessionView::reset(Session* data){
        deleteEventViews();
        
        mAnimating = false;
        mNumData   = -1;
        mDataIndex = -1;
        
        mViewIndexPrev = mViewIndexCurr = mViewIndexNext = -1;
        mNumViews  = -1;
        
        mData = data;
        
        if(data->getEvents()->empty()){
            return;
        }
        
        vector<Event>* events = mData->getEvents();
        mNumData = events->size();
        mNumViews = min(4,mNumData);
        
        int i = -1;
        while (++i < mNumViews) {
            mViews[i] = new EventView(&(*events)[i]);
            mViews[i]->mPosState = mNextEventPosOut;
        }
        
        init();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Animation
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::animateInit(){
        mAnimating = true;
        Timeline& _timeline = timeline();
        
        _timeline.apply(&mViews[0]->mPosState, mNextEventPos, sTimeAnimateInit, EaseOutCubic())
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
                int prevIndex = (mViewIndexCurr - 1) % mNumViews;
                if(prevIndex > -1){
                    animatePrevOutOut(mViews[prevIndex]);
                }
            }
        } else {
            _timeline.apply(&view->mPosState, mPrevEventPosOut, sTimeAnimateOutOut, EaseOutCubic())
                     .finishFn(std::bind(&SessionView::resetEventView,this,view));
        }
        
    }
    
    void SessionView::animatePrevOutOut(EventView* view){
        mAnimating = true;
        Timeline& _timeline = timeline();
        _timeline.apply(&view->mPosState, mPrevEventPosOut, sTimeAnimateOutOut, EaseOutCubic())
                 .finishFn(std::bind(&SessionView::resetEventView,this,view));
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
    
    
    
    void SessionView::stackSpeakers(EventView* view){
        
    }
    
    void SessionView::resetEventView(EventView* view){
        view->mPosState = mNextEventPosOut;
        
        if(mDataIndex >= mNumData){
            return;
        }
        view->reset(&(*mData->getEvents())[mDataIndex]);
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::draw(){
        int  i = -1;
        while (++i < mNumViews) {
            mViews[i]->draw();
        }
    }
    
    void SessionView::update(){
        int  i = -1;
        while (++i < mNumViews) {
            mViews[i]->update();
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
    
    void SessionView::next(){
        if(mAnimating){
            return;
        }
       
        if(mDataIndex < mNumData + 2){
            if(mViewIndexCurr > -1){
                animatePrevOut(mViews[mViewIndexCurr]);
            }
        }
        
        
        mDataIndex++;
        
        if(mDataIndex >= mNumData){
            return;
        }
        mViewIndexCurr = (mViewIndexCurr + 1) % mNumViews;
        animateNextIn(mViews[mViewIndexCurr]);
       
        if(mDataIndex < mNumData -1){
            animateNextOutIn(mViews[(mViewIndexCurr + 1) % mNumViews]);
        }
        
        cout << mDataIndex << " / " << mNumData << endl;
        
    }
    
    void SessionView::init(){
        animateInit();
    }
}