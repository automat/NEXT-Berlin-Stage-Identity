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
#include "AbstractAnimView.h"
#include "SpeakerView.h"

namespace next {
    using namespace std;

    class SpeakerStackView : public AbstractAnimView {
        static const Vec3f sStackStep;
        
        vector<SpeakerView*> mViews;
        size_t               mNumViews;
        int                  mViewIndex;    //  current front
        Vec3f                mStackTop;     //  pos of front
        Vec3f                mStackTopOut;  //  target pos out
        
        bool mActive;       // is current event ?
        
        void animateIn();                        // anim view reset bottom
        void animateOut(SpeakerView* view);      // anim view top disappearing
        void animateMove(SpeakerView* view);     // anim view moving to new pos on stack
        void animateMoveTop(SpeakerView* view);  // anim view (new top) moving to new pos on stack
        void animateFinish();                    // anim on finish
        
        void animateFocus();
        void animateUnfocus();
        
        void deleteViews();
        
        void showNext(int index);
        
    public:
        SpeakerStackView() : AbstractAnimView() {};
        SpeakerStackView(const vector<Speaker*>& data);
        ~SpeakerStackView();
        
        void next(const AnimCallback& callback = std::bind(&AbstractAnimView::AnimCallbackNull)); // for debug
        void reset(const vector<Speaker*>& data);
        
        void draw();
        void update();
        
        void focus();
        void unfocus();
        
        //! stack the speakers!
        void show(const AnimCallback& callback = std::bind(&AbstractAnimView::AnimCallbackNull));
        
    };
}

#endif
