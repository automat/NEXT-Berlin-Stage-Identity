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
    static const float sTimeAnimateOut(0.35f);
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

        mNumViews    = data->size();
        mViewIndex   = -1;
        mAnimating   = false;
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
    
    void SpeakerStackView::drawSelected(){
        mViews[(mViewIndex + 1)%mNumViews]->draw();
    }
    
    void SpeakerStackView::drawUnselected(){
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
    
    
    void SpeakerStackView::next(){
        if(mAnimating){
            return;
        }
        mAnimating = true;
        
        // animate out front
        mViewIndex = (mViewIndex+1) % (mNumViews);
        animateOut(mViews[mViewIndex]);
        
        // animate move rest
        int i = 0;
        while(++i < mNumViews){
            animateMove(mViews[(mViewIndex+i)%mNumViews]);
        }
        
        
    }
    
    void SpeakerStackView::animateOut(SpeakerView* view){
        timeline().apply(&view->mTranslation, mStackTopOut, sTimeAnimateOut, EaseOutQuad())
                  .finishFn(std::bind(&SpeakerStackView::animateIn, this));
    }
    
    void SpeakerStackView::animateMove(SpeakerView* view){
        Vec3f target = view->mTranslation() + sStackStep;
        timeline().apply(&view->mTranslation, target, sTimeAnimateMove, EaseOutCirc());
    }
    
    void SpeakerStackView::animateIn(){
        SpeakerView* view  = mViews[mViewIndex];
        
        const static Vec3f zero;
        view->mTranslation = Vec3f(0,-0.25f,0);
        view->mScale       = 0.35f;
        
        timeline().apply(&view->mScale,       1.0f, sTimeAnimateInScale,     EaseOutCirc());
        timeline().apply(&view->mTranslation, zero, sTimeAnimateInTranslate, EaseOutCirc())
                  .finishFn(std::bind(&SpeakerStackView::animateFinish, this));
    }
    
    void SpeakerStackView::animateFinish(){
        mAnimating = false;
    }
    
    
}