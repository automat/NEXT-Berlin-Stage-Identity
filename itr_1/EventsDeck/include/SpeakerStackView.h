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
        
        //void animateIn();                        // anim view reset bottom
        //void animateOut(SpeakerView* view);      // anim view top disappearing
        void animateIn(SpeakerView* view, const AnimCallback& callback);
        void animateOut(SpeakerView* view, const AnimCallback& callback);
        void animateMove(SpeakerView* view);     // anim view moving to new pos on stack
        void animateMoveTop(SpeakerView* view);  // anim view (new top) moving to new pos on stack
        void animateFinish();                    // anim on finish

        void deleteViews();

        void triggerNext(const AnimCallback& callback);
        
    public:
        SpeakerStackView() : AbstractAnimView() {};
        SpeakerStackView(const vector<Speaker*>& data);
        ~SpeakerStackView();
        
        void next(const AnimCallback& callback = NULL , int index = 0); // for debug
        void reset(const vector<Speaker*>& data);
        
        void draw();
        void update();
        
        void focus();
        void unfocus();
        
        //! stack the speakers!
        void stack(const AnimCallback &callback = NULL);
        
    };
}

#endif
