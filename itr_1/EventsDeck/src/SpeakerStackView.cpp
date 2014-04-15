#include "SpeakerStackView.h"
#include <boost/assign/std/vector.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include "cinder/Timeline.h"



namespace next {
    using namespace std;
    using namespace ci;
    using namespace ci::app;
    using namespace boost::assign;

    // to be replaced with config
    static const float sTimeAnimateOut(0.75f);
    static const float sTimeAnimateOutAlpha(0.35f);
    static const float sTimeAnimateMove(1.5f);
    static const float sTimeAnimateInScale(0.75f);
    static const float sTimeAnimateInTranslate(1.5f);
    
    const Vec3f SpeakerStackView::sStackStep(0,0.125f,0);
    
    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    SpeakerStackView::SpeakerStackView(vector<Speaker>* data){
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
    
    void SpeakerStackView::reset(vector<Speaker>* data){
        deleteViews();

        mAnimating   = false;
        mActive      = true;
        
        mNumViews    = data->size();
        mViewIndex   = -1;
        mStackTop    = sStackStep * static_cast<float>(mNumViews);
        mStackTopOut = mStackTop + Vec3f(0,1,0);
        
        
        
        vector<Speaker>& speakers = *data;
        for(vector<Speaker>::iterator itr = speakers.begin(); itr != speakers.end(); ++itr){
            mViews   += new SpeakerView(&(*itr));
            
            mViews.back()->setPosition(mStackTop - sStackStep * static_cast<float>(mViews.size()));
        }
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
    //  Animation
    /*--------------------------------------------------------------------------------------------*/
    
    //
    //  switch to next speaker
    //
    void SpeakerStackView::next(){
        if(mAnimating){
            return;
        }
        mAnimating = true;
        mViewIndex = (mViewIndex+1) % (mNumViews);
        
        // animate out front
        animateOut(mViews[mViewIndex]);
        
        // animate move new front
        animateMoveTop(mViews[(mViewIndex+1)%mNumViews]);
        
        // animate move rest
        int i = 1;
        while(++i < mNumViews){
            animateMove(mViews[(mViewIndex+i)%mNumViews]);
        }
    }
    
    //
    //  fade current speaker out
    //
    void SpeakerStackView::animateOut(SpeakerView* view){
        //  move view out
        timeline().apply(&view->mTranslation, mStackTopOut, sTimeAnimateOut, EaseOutQuad())
                  .finishFn(std::bind(&SpeakerStackView::animateIn, this));
        
        timeline().apply(&view->mScale,       1.3f, sTimeAnimateOut,     EaseOutCirc());
        
        
        //  unfocus view image
        view->mIntrplState = 1.0f;
        timeline().apply(&view->mIntrplState, 0.0f, sTimeAnimateOut, EaseOutQuad())
                  .updateFn(std::bind(&SpeakerView::updateFocusImage,view));
        
        //  blend view alpha to 0
        timeline().apply(&view->mColorState, 0.0f, sTimeAnimateOutAlpha, EaseOutInSine())
                  .updateFn(std::bind(&SpeakerView::updateColorState, view));
    }
    
    void SpeakerStackView::animateMove(SpeakerView* view){
        //  move view stack step up
        Vec3f target = view->mTranslation() + sStackStep;
        timeline().apply(&view->mTranslation, target, sTimeAnimateMove, EaseOutCirc());
    }
    
    //
    //  fade next speaker in
    //
    void SpeakerStackView::animateMoveTop(SpeakerView* view){
        //  move view stack step up
        animateMove(view);
        
        //  focus view image
        timeline().apply(&view->mIntrplState, 1.0f, sTimeAnimateMove, EaseOutCirc())
                  .updateFn(std::bind(&SpeakerView::updateFocusImage,view));
    }
    
    //
    //  append prev speaker to bottom
    //
    void SpeakerStackView::animateIn(){
        const static Vec3f zero;
        
        SpeakerView* view  = mViews[mViewIndex];
        view->mTranslation = Vec3f(0,-0.25f,0);
        view->mScale       = 0.35f;
        view->mColorState  = 1.0f;
        
        view->updateColorState();
        
        timeline().apply(&view->mScale,       1.0f, sTimeAnimateInScale,     EaseOutCirc());
        timeline().apply(&view->mTranslation, zero, sTimeAnimateInTranslate, EaseOutCirc())
                  .finishFn(std::bind(&SpeakerStackView::animateFinish, this));
    }
    
    void SpeakerStackView::animateFinish(){
        mAnimating = false;
    }
    
    void SpeakerStackView::animateFocusImage(SpeakerView* view){
        
    }
    
    void SpeakerStackView::animateFocusState(SpeakerView* view){
        
    }
    
    
}