//
//  SpeakerStackView.h
//  SpeakerImageFilter
//
//  Created by Henryk Wollik on 15/04/14.
//
//

#ifndef SpeakerImageFilter_SpeakerStackView_h
#define SpeakerImageFilter_SpeakerStackView_h

#include <vector>
#include "SpeakerView.h"

namespace next {
    using namespace std;

    class SpeakerStackView {
        static const Vec3f sStackStep;
        
        vector<SpeakerView*> mViews;
        size_t               mNumViews;
        int                  mViewIndex;    //  current front
        Vec3f                mStackTop;     //  pos of front
        Vec3f                mStackTopOut;  //  target pos out
        
        bool mAnimating;    // currently animating ?
        bool mActive;       // is current event ?
        
        void animateIn();                        // anim view reset bottom
        void animateOut(SpeakerView* view);      // anim view top disappearing
        void animateMove(SpeakerView* view);     // anim view moving to new pos on stack
        void animateMoveTop(SpeakerView* view);  // anim view (new top) moving to new pos on stack
        void animateFinish();                    // anim on finish
        
        void animateFocus();
        void animateUnfocus();
        
        void deleteViews();
        
    public:
        SpeakerStackView(){};
        SpeakerStackView(vector<Speaker>* data);
        ~SpeakerStackView();
        
        void next(); // for debug
        void reset(vector<Speaker>* data);
        
        void draw();
        void update();
        
        void focus();
        void unfocus();
        
    };
}

#endif
