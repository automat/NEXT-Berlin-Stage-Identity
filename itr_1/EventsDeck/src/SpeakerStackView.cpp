#include "SpeakerStackView.h"
#include <boost/assign/std/vector.hpp>
#include <vector>

namespace next {
    using namespace std;
    using namespace boost::assign;
    
    const float SpeakerStackView::sStackPadding(0.125f);
    
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

        mNumViews = data->size();
        mViewTop  = 0;
        float top = static_cast<float>(mNumViews) * sStackPadding;
        
        vector<Speaker>& speakers = *data;
        
        for(vector<Speaker>::iterator itr = speakers.begin(); itr != speakers.end(); ++itr){
            mViews += new SpeakerView(&(*itr));
            mViews.back()->setPosition(Vec3f(0,top - static_cast<float>(mViews.size()) * sStackPadding,0));
        }
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
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
}