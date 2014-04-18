//
//  AnimView.h
//  EventsDeck
//
//  Created by Henryk Wollik on 17/04/14.
//
//

#ifndef EventsDeck_AnimView_h
#define EventsDeck_AnimView_h

#include "cinder/Timeline.h"

namespace next {
    using namespace ci;
    using namespace ci::app;
    
    class AbstractAnimView {
    public :
        typedef std::function<void ()> AnimCallback;
        
    protected:
        template<typename T>
        typename Tween<T>::Options tween(Anim<T> *target, T endValue, float duration, EaseFn easeFunction = EaseNone(), const AnimCallback& updateCallback = NULL, const AnimCallback& finishCallback = NULL){
            return timeline().apply(target, endValue, duration, easeFunction).updateFn(updateCallback).finishFn(finishCallback);
        }
        
        template<typename T>
        typename Tween<T>::Options tween(Anim<T> *target, T startValue, T endValue, float duration, EaseFn easeFunction = EaseNone(), const AnimCallback& updateCallback = NULL, const AnimCallback& finishCallback = NULL){
            return timeline().apply(target, endValue, duration, easeFunction).updateFn(updateCallback).finishFn(finishCallback);
        }
        
    public:
        
        virtual void draw() = 0;
        virtual void update() = 0;
        
    };
}

#endif
