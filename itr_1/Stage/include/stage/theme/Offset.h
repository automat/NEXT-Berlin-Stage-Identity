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
        float mTickStart;
        float mTickEnd;
        float mOrigin;
        float mDist;
        float mTarget;
        float mValue;
        float mDuration;
        float mTime;
        float mDelay;
        bool  mLoop;
        bool  mFinished;
        std::function<void()> mCallback;
        
    public:
        Offset();
        Offset(float origin, float target, float duration, float delay, bool loop);
        
        void loop(bool loop = true);
        void reset(float origin,float target, float duration, float delay, bool loop);
        void setCallback(const std::function<void()>& func);
        
        void update();
        float getValue();
        
        bool isFinished();
    };
}


#endif
