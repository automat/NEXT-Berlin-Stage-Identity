#include "SpeakerStackView.h"
#include <boost/assign/std/vector.hpp>


using namespace boost::assign;

const float SpeakerStackView::sStackPadding(0.125f);


SpeakerStackView::SpeakerStackView(const vector<Speaker*>& data){
    reset(data);
    
}

SpeakerStackView::~SpeakerStackView(){
    deleteViews();
}


void SpeakerStackView::deleteViews(){
    while (!mViews.empty()) delete mViews.back(), mViews.pop_back();
}

void SpeakerStackView::reset(const vector<Speaker *> &data){
    deleteViews();
    
    mNumViews = data.size();
    mViewTop  = 0;
    float top = static_cast<float>(mNumViews) * sStackPadding;
    
    for(vector<Speaker*>::const_iterator itr = data.begin(); itr != data.end(); ++itr){
        mViews += new SpeakerView(*itr);
        mViews.back()->setPosition(Vec3f(0,top - static_cast<float>(mViews.size()) * sStackPadding,0));
    }
}


void SpeakerStackView::draw(){
    for(vector<SpeakerView*>::const_iterator itr = mViews.begin(); itr != mViews.end(); ++itr){
        (*itr)->draw();
    }
}

void SpeakerStackView::update(){
    for(vector<SpeakerView*>::const_iterator itr = mViews.begin(); itr != mViews.end(); ++itr){
        (*itr)->update();
    }
    
}