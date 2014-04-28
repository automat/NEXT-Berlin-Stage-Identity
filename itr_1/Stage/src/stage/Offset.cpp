#include "stage/theme/Offset.h"
#include "Config.h"

namespace next {
    Offset::Offset() :
    mValue(0),
    mOrigin(0),
    mTarget(0),
    mDuration(1),
    mTime(0),
    mDelay(0),
    mCallback(NULL){}
    
    Offset::Offset(float origin, float target, float duration, float delay) :  mCallback(NULL){
        reset(origin, target, duration, delay);
    }
    
    void Offset::reset(float origin, float target, float duration, float delay){
        mFinished = false;
        mOrigin   = mValue = origin;
        mTarget   = target;
        mDist     = mTarget - mOrigin;
        
        mTime     = 0;
        mDuration = duration * APP_FPS;
        mDelay    = delay    * APP_FPS;

    }
   
    
    void Offset::update(){
        if(mFinished){
            return;
        } else {
            if( (mTime - mDelay) > mDuration ){
                mFinished = true;
                if (mCallback) {
                    mCallback();
                }
                return;
            }
        }
        
        mTime++;
        if(mTime < mDelay){
            return;
        }
        
         // step inv cubed
         float step = 1.0f - (mTime - mDelay) / mDuration;
               step = 1.0f - step * step * step * step;
        
        mValue = mDist * step + mOrigin;
    }
    
    float Offset::getValue(){
        return mValue;
    }
    
    bool Offset::isFinished(){
        return mFinished;
    }
    
    void Offset::setCallback(const std::function<void()>& func){
        mCallback = func;
    }
    
}