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
        int                  mViewIndex;
        Vec3f                mStackTop;
        Vec3f                mStackTopOut;
        
        bool mAnimating;
        void animateIn();
        void animateOut(SpeakerView* view);
        void animateMove(SpeakerView* view);
        void animateFinish();
        
        void deleteViews();
        
    public:
        SpeakerStackView(){};
        SpeakerStackView(vector<Speaker>* data);
        ~SpeakerStackView();
        
        void next();
        void reset(vector<Speaker>* data);
        
        void drawSelected();
        void drawUnselected();
        void update();
        
    };
}

#endif
