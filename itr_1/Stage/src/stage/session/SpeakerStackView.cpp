#include "stage/session/SpeakerStackView.h"

#include <boost/assign/std/vector.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include "cinder/Timeline.h"
#include "cinder/Tween.h"

#include "Config.h"

namespace next {
    
    
    using namespace std;
    using namespace ci;
    using namespace ci::app;
    using namespace boost::assign;

    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    SpeakerStackView::SpeakerStackView(const vector<Speaker*>& data) :
        AbstractAnimBase(){
        reset(data);
    }
    
    SpeakerStackView::~SpeakerStackView(){
        deleteViews();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Handle Data
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerStackView::deleteViews(){
        while (!mViews.empty()) delete mViews.back(), mViews.pop_back();
    }
    
    void SpeakerStackView::reset(const vector<Speaker*>& data){
        deleteViews();
        
        mNumViews    = data.size();
        mViewIndex   = -1;
        mStackTop    = SESSION_SPEAKER_STACK_STEP * static_cast<float>(mNumViews);
        mStackTopOut = mStackTop + Vec3f(0,0.5f,0);
        
        mTimeAnimDelaySpeaker = MAX(SESSION_EVENT_TIME_SPEAKER_MIN,SESSION_EVENT_TIME_MAX / static_cast<float>(mNumViews));
        
        for(vector<Speaker*>::const_iterator itr = data.begin(); itr != data.end(); ++itr){
            mViews   += new SpeakerView(*itr);
            mViews.back()->mPositionState = mStackTop - SESSION_SPEAKER_STACK_STEP * static_cast<float>(mViews.size());
        }
    }
    
    void SpeakerStackView::resetStack(){
        int i = 0;
        while (i < mNumViews) {
            SpeakerView* view = mViews[i];
            view->mPositionState = mStackTop - SESSION_SPEAKER_STACK_STEP * static_cast<float>(i + 1);
            view->clearStates();
            i++;
        }
        mViewIndex = -1;
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Draw
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerStackView::draw(){
        mViews[(mViewIndex + 1)%mNumViews]->draw();
        int i = 0;
        while(++i < mNumViews){
            mViews[(mViewIndex+i+1)%mNumViews]->draw();
        }
    }
    
    void SpeakerStackView::update(){
        for (vector<SpeakerView*>::iterator itr = mViews.begin(); itr != mViews.end(); ++itr) {
            (*itr)->update();
        }
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Focus / Unfocus stack
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerStackView::focus(){
        SpeakerView* first = mViews.front();
        first->focus();
    }
    
    void SpeakerStackView::focusIn(){
        mViews.front()->focusIn();
    }
    
    void SpeakerStackView::unfocus(){
        mViews[mNumViews-1]->unfocusImage();
        for(vector<SpeakerView*>::iterator itr = mViews.begin(); itr != mViews.end(); ++itr){
            (*itr)->unfocus();
        }
    }
    
    void SpeakerStackView::unfocusOut(){
        for(vector<SpeakerView*>::iterator itr = mViews.begin(); itr != mViews.end(); ++itr){
            (*itr)->unfocusOut();
        }
    }

    /*--------------------------------------------------------------------------------------------*/
    //  trigger stacking
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerStackView::stack(const AnimCallback_Int_1& callbackUpdate, const AnimCallback& callbackFinish){
        if(mViews.size() == 1){
            delayCallback(mTimeAnimDelaySpeaker, callbackFinish);
            return;
        }
        delayCallback(mTimeAnimDelaySpeaker,
                      std::bind(&SpeakerStackView::next,
                                this,
                                callbackUpdate,
                                callbackFinish,
                                0));
    }
    
    void SpeakerStackView::next(const AnimCallback_Int_1& callbackUpdate, const AnimCallback& callbackFinish, int index){
        mViewIndex = index;
        int indexNext = (mViewIndex + 1) % mNumViews; // just in cease there will be multiple stacking cycles

        //
        //  curr : move out & blur & hide
        //
        SpeakerView* curr = mViews[mViewIndex];
        tween(&curr->mPositionState, mStackTopOut, SESSION_SPEAKER_STACK_ANIM_OUT, EaseOutQuad(),
              NULL, std::bind(&SpeakerStackView::restack, this, curr, callbackUpdate, callbackFinish));

        curr->unfocusImage();
        curr->hide();

        //
        //  next : move up & focus
        //
        SpeakerView* next = mViews[indexNext];
        tween(&next->mPositionState, next->mPositionState() + SESSION_SPEAKER_STACK_STEP, SESSION_SPEAKER_STACK_ANIM_MOVE, EaseOutCirc());
        next->focus();

        //
        //  rest : move up
        //
        int i = 1;
        while(++i < mNumViews){
            next = mViews[(mViewIndex+i)%mNumViews];
            tween(&next->mPositionState, next->mPositionState() + SESSION_SPEAKER_STACK_STEP, SESSION_SPEAKER_STACK_ANIM_MOVE, EaseOutCirc());
        }
        
        callbackUpdate(indexNext);
    }
    
    void SpeakerStackView::triggerNext(const AnimCallback_Int_1& callbackUpdate, const AnimCallback& callbackFinish){
        mViewIndex = (mViewIndex + 1) % mNumViews;

        if (mViewIndex == mNumViews - 1) {
            delayCallback(mTimeAnimDelaySpeaker, callbackFinish);
        } else {
            delayCallback(mTimeAnimDelaySpeaker,
                          std::bind(&SpeakerStackView::next,
                                    this,
                                    callbackUpdate,
                                    callbackFinish,
                                    mViewIndex));
        }
    }

    /*--------------------------------------------------------------------------------------------*/
    //  restack
    /*--------------------------------------------------------------------------------------------*/

    void SpeakerStackView::restack(SpeakerView *view,
            const AnimCallback_Int_1 &callbackUpdate,
            const AnimCallback &callbackFinish){
        const static Vec3f zero;
        
        view->mPositionState = Vec3f(0,-0.25f,0);
        view->mScaleState    = 0.35f;

        //view->updateAlpha();
        //view->updateFocusImage();

        view->show();

        tween(&view->mScaleState, 1.0f, SESSION_SPEAKER_STACK_ANIM_IN_SCALE, EaseOutCirc());
        tween(&view->mPositionState, zero, SESSION_SPEAKER_STACK_ANIM_IN_TRANSLATION, EaseOutCirc(),
              NULL, std::bind(&SpeakerStackView::triggerNext,
                              this,
                              callbackUpdate,
                              callbackFinish));
    }
}