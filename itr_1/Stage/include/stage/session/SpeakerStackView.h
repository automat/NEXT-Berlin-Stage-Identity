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
#include "stage/session/AbstractAnimBase.h"
#include "stage/session/SpeakerView.h"

namespace next {
    using namespace std;

    class SpeakerStackView : public AbstractAnimBase {
        
        vector<SpeakerView*> mViews;
        size_t               mNumViews;
        int                  mViewIndex;    //  current front
        Vec3f                mStackTop;     //  pos of front
        Vec3f                mStackTopOut;  //  target pos out
        
        float                mTimeAnimDelaySpeaker;


        //! cleanup
        void deleteViews();

        //! callback next
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
        SpeakerStackView() : AbstractAnimBase() {};
        SpeakerStackView(const vector<Speaker*>& data);
        ~SpeakerStackView();

        //! reset stack with new data
        void reset(const vector<Speaker*>& data);

        //! trigger stacking animation, callback on next item & stacking finished
        void stack(const AnimCallback_Int_1& callbackUpdate,
                   const AnimCallback& callbackFinish);

        //! focus top image
        void focus();
        //! focus top image, originating from outside the scree
        void focusIn();

        //! unfocus color and image, complete stack on out
        void unfocusOut();
        //! unfocus color and image, complete stack
        void unfocus();

        //! reset stack to its initial unfocused state
        void resetStack();
        //! draw
        void draw();
        
        void update();

    };
}

#endif
