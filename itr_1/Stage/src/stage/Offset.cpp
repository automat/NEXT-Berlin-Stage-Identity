#include "stage/theme/Offset.h"
#include "util/MathUtil.h"
#include "Config.h"

namespace next {
    Offset::Offset() :
    mOrigin(0),
    mTarget(0),
    mDuration(1),
    mTime(0),
    mDelay(0),
    mLoop(false),
    mCallback(NULL){}
    
    Offset::Offset(float origin, float target, float duration, float delay, bool loop) : mTime(0), mCallback(NULL){
        reset(origin, target, duration, delay, loop);
    }
    
    void Offset::reset(float origin, float target, float duration, float delay, bool loop){
        mOrigin   = mValue = origin;
        mTarget   = target;
        mDist     = mTarget - mOrigin;
        
        mTime     = 0;
        mDuration = duration * APP_FPS;
        mDelay    = delay    * APP_FPS;
        mLoop     = loop;
    }
    
    void Offset::update(){
        if(mLoop && mFinished){
            mFinished = false;
            mTime     = 0;
        }
        if ((mTime - mDelay) > mDuration){
            mFinished = true;
            if(mCallback){
                mCallback();
            }
            return;
        }
        mTime++;
        if(mTime < mDelay){
            return;
        }
        
        mValue = mDist * util::stepInvCubedf((mTime - mDelay) / mDuration) + mOrigin;
    }
    
    float Offset::getValue(){
        return mValue;
    }
    
    bool Offset::isFinished(){
        return mFinished;
    }
    
}