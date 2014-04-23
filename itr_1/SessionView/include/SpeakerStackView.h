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
        
        float                mTimeAnimDelaySpeaker;
        
        void animateIn(SpeakerView* view,
                       const AnimCallback_Int_1& callbackUpdate,
                       const AnimCallback& callbackFinish);
        
        void animateOut(SpeakerView* view,
                        const AnimCallback_Int_1& callbackUpdate,
                        const AnimCallback& callbackFinish);
        
        void animateMove(SpeakerView* view);     // anim view moving to new pos on stack
        void animateMoveTop(SpeakerView* view);  // anim view (new top) moving to new pos on stack

        void deleteViews();

        void triggerNext(const AnimCallback_Int_1& callbackUpdate,
                         const AnimCallback& callbackFinish);
        
        void next(const AnimCallback_Int_1& callbackUpdate,
                  const AnimCallback& callbackFinish ,
                  int index = 0); // for debug
        
    public:
        SpeakerStackView() : AbstractAnimView() {};
        SpeakerStackView(const vector<Speaker*>& data);
        ~SpeakerStackView();
       
        void reset(const vector<Speaker*>& data);
        void stack(const AnimCallback_Int_1& callbackUpdate,
                   const AnimCallback& callbackFinish);
        
        void resetStack();
        
        void draw();
        void update();
        
        void focus();
        void unfocus();
    };
}

#endif
