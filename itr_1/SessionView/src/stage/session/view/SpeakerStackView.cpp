#include "stage/session/view/SpeakerStackView.h"

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

    // to be replaced with config
    static const float sTimeAnimateOut(0.35f);
    static const float sTimeAnimateOutAlpha(0.35f);
    static const float sTimeAnimateMove(1.5f);
    static const float sTimeAnimateInScale(0.75f);
    static const float sTimeAnimateInTranslate(1.5f);
    static const float sTimeAnimateFocus(1.0f);
    static const float sTimeAnimateUnfocus(1.0f);
    
    const Vec3f SpeakerStackView::sStackStep(0,0.05f,0);
    
    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    SpeakerStackView::SpeakerStackView(const vector<Speaker*>& data) :
        AbstractAnimView(){
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
        mStackTop    = sStackStep * static_cast<float>(mNumViews);
        mStackTopOut = mStackTop + Vec3f(0,0.5f,0);
        
        mTimeAnimDelaySpeaker = MAX(SESSION_EVENT_TIME_SPEAKER_MIN,SESSION_EVENT_TIME_MAX / static_cast<float>(mNumViews));
        
        for(vector<Speaker*>::const_iterator itr = data.begin(); itr != data.end(); ++itr){
            mViews   += new SpeakerView(*itr);
            mViews.back()->mPositionState = mStackTop - sStackStep * static_cast<float>(mViews.size());
        }
    }
    
    void SpeakerStackView::resetStack(){
        int i = 0;
        while (i < mNumViews) {
            SpeakerView* view = mViews[i];
            view->mPositionState = mStackTop - sStackStep * static_cast<float>(i + 1);
            view->unfocusImage();
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

    /*--------------------------------------------------------------------------------------------*/
    //  Focus / Unfocus stack
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerStackView::focus(){
        SpeakerView* first = mViews.front();
        tween(&first->mIntrplState, 0.0f, 1.0f, sTimeAnimateFocus, EaseNone(),
              std::bind(&SpeakerView::updateFocusState,first));
    }
    
    void SpeakerStackView::unfocus(){
        for(vector<SpeakerView*>::iterator itr = mViews.begin(); itr != mViews.end(); ++itr){
            tween(&(*itr)->mIntrplState, 1.0f, 0.0f, sTimeAnimateUnfocus, EaseNone(),
                  std::bind(&SpeakerView::updateFocusState, *itr));
        }
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Trigger
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
        int nextFocusedViewIndex = mViewIndex + 1;
        
        animateOut(mViews[mViewIndex],callbackUpdate, callbackFinish);
        animateMoveTop(mViews[(nextFocusedViewIndex)%mNumViews]);
        
        int i = 1;
        while(++i < mNumViews){
            animateMove(mViews[(mViewIndex+i)%mNumViews]);
        }
        
        callbackUpdate(nextFocusedViewIndex);
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
    //  Animation
    /*--------------------------------------------------------------------------------------------*/

    void SpeakerStackView::animateOut(SpeakerView *view,
                                      const AnimCallback_Int_1& callbackUpdate,
                                      const AnimCallback& callbackFinish){
        
        tween(&view->mPositionState, mStackTopOut, sTimeAnimateOut, EaseOutQuad(),
              NULL, std::bind(&SpeakerStackView::animateIn,
                              this,
                              view,
                              callbackUpdate,
                              callbackFinish));
        
        tween(&view->mIntrplState, 1.0f, 0.0f, sTimeAnimateOut, EaseOutQuad(),
              std::bind(&SpeakerView::updateFocusImage, view));
        
        tween(&view->mColorState, 0.0f, sTimeAnimateOutAlpha, EaseOutInSine(),
              std::bind(&SpeakerView::updateColorState, view));
    }
    
    void SpeakerStackView::animateIn(SpeakerView *view,
                                     const AnimCallback_Int_1& callbackUpdate,
                                     const AnimCallback& callbackFinish){
        const static Vec3f zero;
        
        view->mPositionState = Vec3f(0,-0.25f,0);
        view->mScaleState    = 0.35f;
        view->mColorState    = 1.0f;
        view->mIntrplState   = 0.0f;
        
        view->updateColorState();
        view->updateFocusImage();
        
        tween(&view->mScaleState, 1.0f, sTimeAnimateInScale, EaseOutCirc());
        tween(&view->mPositionState, zero, sTimeAnimateInTranslate, EaseOutCirc(),
              NULL, std::bind(&SpeakerStackView::triggerNext,
                              this,
                              callbackUpdate,
                              callbackFinish));
    }
    
    void SpeakerStackView::animateMove(SpeakerView* view){
        Vec3f target = view->mPositionState() + sStackStep;
        tween(&view->mPositionState, target, sTimeAnimateMove, EaseOutCirc());
    }
    
    void SpeakerStackView::animateMoveTop(SpeakerView* view){
        animateMove(view);
        
        tween(&view->mIntrplState, 1.0f, sTimeAnimateMove, EaseOutCirc(),
              std::bind(&SpeakerView::updateFocusImage,view));
    }

}