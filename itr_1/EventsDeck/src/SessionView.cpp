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
    static const float sTimeAnimateOut(1.0f);
    static const float sTimeAnimateStart(1.0f);
    static const float sTimeAnimateEnd(1.0f);
    
    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    SessionView::SessionView(Session* data){
        float slideLength   = 6;
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
        while (!mViews.empty()) delete mViews.back(), mViews.pop_back();
    }
    
    void SessionView::reset(Session* data){
        deleteEventViews();
        mViewIndexPrev = mViewIndexCurr = mViewIndexNext = -1;
        mNumViews  = -1;
        mAnimating = false;
        
        vector<Event>* events = data->getEvents();

        if(events->empty()){
            return;
        }
        
        size_t len = min(3,static_cast<int>(events->size()));
        mViewIndexPrev = -1;    //  unused
        
        //  Init views and reset to position of 'next' view
        
        mViewIndexCurr = 0 % len;
            mViews += new EventView(&(*events)[mViewIndexCurr]);
            mViews.back()->mPosState = mNextEventPos;
        
        mViewIndexNext = 1 % len == 0 ? -1 : 1;
        if(mViewIndexNext != -1){
            mViews += new EventView(&(*events)[mViewIndexNext]);
            mViews.back()->mPosState = mNextEventPos;
        }
        
        
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Animation
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::animateOut(EventView *view){
        
    }
    
    void SessionView::animateIn(EventView* view){
        mAnimating = true;
        Timeline& _timeline = timeline();
        _timeline.apply(&view->mPosState, mCurrEventPos, sTimeAnimateIn, EaseOutCubic())
                 .finishFn(std::bind(&SessionView::animateFinish,this));
        view->focusTop();
        
        int viewIndexPrev = mViewIndexCurr - 1;
        
        if(viewIndexPrev > -1){
            
        }
        
        
    }
    
    void SessionView::stackSpeakers(EventView* next){
        
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::draw(){
        for(vector<EventView*>::const_iterator itr = mViews.begin(); itr != mViews.end(); ++itr){
            (*itr)->draw();
        }
    }
    
    void SessionView::update(){
        for(vector<EventView*>::const_iterator itr = mViews.begin(); itr != mViews.end(); ++itr){
            (*itr)->update();
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
        
        
        if(mViewIndexCurr == 0){
            animateIn(mViews[mViewIndexCurr]);
        }
        
        mViewIndexCurr++;
        
        
    }
}