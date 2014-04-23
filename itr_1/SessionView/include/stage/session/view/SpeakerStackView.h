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
#include "stage/session/view/AbstractAnimView.h"
#include "stage/session/view/SpeakerView.h"

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
        

        void deleteViews();

        void triggerNext(const AnimCallback_Int_1& callbackUpdate,
                         const AnimCallback& callbackFinish);

        //! proceed stack
        void next(const AnimCallback_Int_1& callbackUpdate,
                  const AnimCallback& callbackFinish ,
                  int index = 0);

        //! restack to bottom
        void restack(SpeakerView *view,
                const AnimCallback_Int_1 &callbackUpdate,
                const AnimCallback &callbackFinish);


    public:
        SpeakerStackView() : AbstractAnimView() {};
        SpeakerStackView(const vector<Speaker*>& data);
        ~SpeakerStackView();
       
        void reset(const vector<Speaker*>& data);
        void stack(const AnimCallback_Int_1& callbackUpdate,
                   const AnimCallback& callbackFinish);
        
        void resetStack();
        
        void draw();
        
        void unfocus();
 
        
        //! focus top image
        void focus();
        //! focus top image, originating from outside the scree
        void focusIn();
        //! unfocus
        void unfocusOut();
        
        //! clearStates images to initial state
        void clearStates();
    };
}

#endif
