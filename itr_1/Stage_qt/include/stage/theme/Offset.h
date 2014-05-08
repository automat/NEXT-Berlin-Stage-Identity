//
//  Offset.h
//  Stage
//
//  Created by Henryk Wollik on 26/04/14.
//
//

#ifndef Stage_Offset_h
#define Stage_Offset_h

namespace next {
    class Offset{
        float mValue;
        float mOrigin;
        float mTarget;
        
        float mDist;
        
        float mTime;
        float mDuration;
        float mDelay;
        
        bool  mFinished;
        
        std::function<void()> mCallback;
        
    public:
        Offset();
        Offset(float origin, float target, float duration, float delay);
        
        void loop(bool loop = true);
        void reset(float origin,float target, float duration, float delay);
        void setCallback(const std::function<void()>& func);
        
        void update();
        float getValue();
        
        void pause();
        void resume();
        
        bool isFinished();
    };
}


#endif
