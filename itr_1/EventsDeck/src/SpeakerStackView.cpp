#include "SpeakerStackView.h"
#include <boost/assign/std/vector.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include "cinder/Timeline.h"
#include "cinder/Tween.h"

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

        mAnimating   = false;
        mActive      = true;
        
        mNumViews    = data.size();
        mViewIndex   = -1;
        mStackTop    = sStackStep * static_cast<float>(mNumViews);
        mStackTopOut = mStackTop + Vec3f(0,0.5f,0);
        
        for(vector<Speaker*>::const_iterator itr = data.begin(); itr != data.end(); ++itr){
            mViews   += new SpeakerView(*itr);
            
            mViews.back()->setPosition(mStackTop - sStackStep * static_cast<float>(mViews.size()));
        }
        
        //focus();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerStackView::draw(){
        mViews[(mViewIndex + 1)%mNumViews]->draw();
        int i = 0;
        while(++i < mNumViews){
            mViews[(mViewIndex+i+1)%mNumViews]->draw();
        }
    }
    
    void SpeakerStackView::update(){
        for(vector<SpeakerView*>::const_iterator itr = mViews.begin(); itr != mViews.end(); ++itr){
            (*itr)->update();
        }
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Focus / Unfocus stack
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerStackView::focus(){
        if(mAnimating){
            return;
        }
        
        mAnimating = true;
        Timeline& _timeline = timeline();
        SpeakerView* first = mViews.front();
        _timeline.apply(&first->mIntrplState, 0.0f, 1.0f, 1.0f)
                 .updateFn(std::bind(&SpeakerView::updateFocusState,first))
                 .finishFn(std::bind(&SpeakerStackView::animateFinish,this));
    }
    
    void SpeakerStackView::unfocus(){
        mAnimating = true;
        Timeline& _timeline = timeline();

        
        //  TODO: Add animate finish counter here
        for(vector<SpeakerView*>::iterator itr = mViews.begin(); itr != mViews.end(); ++itr){
            _timeline.apply(&(*itr)->mIntrplState, 1.0f, 0.0f, 1.0f)
                     .updateFn(std::bind(&SpeakerView::updateFocusState,*itr))
                     .finishFn(std::bind(&SpeakerStackView::animateFinish,this));
        }
    }
    
    void SpeakerStackView::show(const AnimCallback& callback){
        
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Animation
    /*--------------------------------------------------------------------------------------------*/
    
    /*
    void SpeakerStackView::next(const AnimCallback& callback, int index){
        if(mAnimating){
            return;
        }
        mAnimating = true;
        mViewIndex = (mViewIndex+1) % (mNumViews);
        
        animateOut(mViews[mViewIndex]);
        animateMoveTop(mViews[(mViewIndex+1)%mNumViews]);
   
        int i = 1;
        while(++i < mNumViews){
            animateMove(mViews[(mViewIndex+i)%mNumViews]);
        }
    }*/
    
    void SpeakerStackView::next(const AnimCallback& callback, int index){
        if(mAnimating){
            return;
        }
        
        if(mViews.size() == 1){
            callback();
            return;
        }
        
        mAnimating = true;
        mViewIndex = index;
        
        animateOut(mViews[mViewIndex],callback);
        animateMoveTop(mViews[(mViewIndex+1)%mNumViews]);
        
        int i = 1;
        while(++i < mNumViews){
            animateMove(mViews[(mViewIndex+i)%mNumViews]);
        }
        
        //animates
    }
    
    void SpeakerStackView::triggerNext(const AnimCallback& callback){
        mAnimating = false;
        mViewIndex = (mViewIndex + 1) % mNumViews;
        cout << mViewIndex << endl;
        if (mViewIndex == mNumViews -1) {
            callback();
        } else {
            next(callback,mViewIndex);
        }
    }
    
    void SpeakerStackView::animateOut(SpeakerView *view, const AnimCallback &callback){
        Timeline& _timeline = timeline();
        
        //  move view out
        _timeline.apply(&view->mTranslation, mStackTopOut, sTimeAnimateOut, EaseOutQuad())
        .finishFn(std::bind(&SpeakerStackView::animateIn, this, view, callback));
        
        _timeline.apply(&view->mScale, 1.0f, sTimeAnimateOut, EaseOutCirc());
        
        //  unfocus view image
        view->mIntrplState = 1.0f;
        _timeline.apply(&view->mIntrplState, 0.0f, sTimeAnimateOut, EaseOutQuad())
        .updateFn(std::bind(&SpeakerView::updateFocusImage,view));
        
        //  blend view alpha to 0
        _timeline.apply(&view->mColorState, 0.0f, sTimeAnimateOutAlpha, EaseOutInSine())
        .updateFn(std::bind(&SpeakerView::updateColorState, view));

    }
    
    void SpeakerStackView::animateIn(SpeakerView *view, const AnimCallback &callback){
        Timeline& _timeline = timeline();
        const static Vec3f zero;
        
        view->mTranslation = Vec3f(0,-0.25f,0);
        view->mScale       = 0.35f;
        view->mColorState  = 1.0f;
        view->mIntrplState = 0.0f;
        
        view->updateColorState();
        view->updateFocusImage();
        
        _timeline.apply(&view->mScale,       1.0f, sTimeAnimateInScale,     EaseOutCirc());
        _timeline.apply(&view->mTranslation, zero, sTimeAnimateInTranslate, EaseOutCirc())
        .finishFn(std::bind(&SpeakerStackView::triggerNext, this, callback));
    }
    
    /*
    void SpeakerStackView::animateOut(SpeakerView* view){
        Timeline& _timeline = timeline();
        
        //  move view out
        _timeline.apply(&view->mTranslation, mStackTopOut, sTimeAnimateOut, EaseOutQuad())
                 .finishFn(std::bind(&SpeakerStackView::animateIn, this));
        
        _timeline.apply(&view->mScale, 1.0f, sTimeAnimateOut, EaseOutCirc());
        
        //  unfocus view image
        view->mIntrplState = 1.0f;
        _timeline.apply(&view->mIntrplState, 0.0f, sTimeAnimateOut, EaseOutQuad())
                 .updateFn(std::bind(&SpeakerView::updateFocusImage,view));
        
        //  blend view alpha to 0
        _timeline.apply(&view->mColorState, 0.0f, sTimeAnimateOutAlpha, EaseOutInSine())
                 .updateFn(std::bind(&SpeakerView::updateColorState, view));
    }*/
    
    void SpeakerStackView::animateMove(SpeakerView* view){
        Vec3f target = view->mTranslation() + sStackStep;
        Timeline& _timeline = timeline();
        _timeline.apply(&view->mTranslation, target, sTimeAnimateMove, EaseOutCirc());
    }
    
    void SpeakerStackView::animateMoveTop(SpeakerView* view){
        Timeline& _timeline = timeline();
        //  move view stack step up
        animateMove(view);
        //  focus view image
        _timeline.apply(&view->mIntrplState, 1.0f, sTimeAnimateMove, EaseOutCirc())
                  .updateFn(std::bind(&SpeakerView::updateFocusImage,view));
    }
    /*
    void SpeakerStackView::animateIn(){
        Timeline& _timeline = timeline();
        const static Vec3f zero;
        
        SpeakerView* view  = mViews[mViewIndex];
        view->mTranslation = Vec3f(0,-0.25f,0);
        view->mScale       = 0.35f;
        view->mColorState  = 1.0f;
        view->mIntrplState = 0.0f;
        
        view->updateColorState();
        view->updateFocusImage();
        
        _timeline.apply(&view->mScale,       1.0f, sTimeAnimateInScale,     EaseOutCirc());
        _timeline.apply(&view->mTranslation, zero, sTimeAnimateInTranslate, EaseOutCirc())
                 .finishFn(std::bind(&SpeakerStackView::animateFinish, this));
    }*/
    
    void SpeakerStackView::animateFinish(){
        mAnimating = false;
    }
}