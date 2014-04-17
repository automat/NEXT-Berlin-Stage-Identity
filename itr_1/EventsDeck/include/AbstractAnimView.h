//
//  AnimView.h
//  EventsDeck
//
//  Created by Henryk Wollik on 17/04/14.
//
//

#ifndef EventsDeck_AnimView_h
#define EventsDeck_AnimView_h



namespace next {
    class AbstractAnimView {
    protected:
        bool mAnimating;    // currently animating ?
        
        inline void animateFinish(){
            mAnimating = false;
        }
        
    public:
        virtual void draw() = 0;
        virtual void update() = 0;
        
        inline bool isAnimating(){
            return mAnimating;
        }
    };
}

#endif
