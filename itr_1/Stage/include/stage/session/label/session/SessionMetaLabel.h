//
//  SessionMetaLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_SessionMetaLabel_h
#define SessionView_SessionMetaLabel_h

#include "stage/session/label/AbstractLabel.h"
#include <string>


namespace next {
    using namespace std;
    class SessionMetaLabel : public AbstractLabel {
    private:
        Anim<float>    mAlphaState;
        next::TextBox* mTextBoxTimeRemaining;
        float          mTextBoxTimeWidth;
        
        bool           mReachedTargetTimestamp;
        time_t         mTargetTimestamp;

        Vec2f mVertexTrapezoid[4];
        
        gl::Texture    mClockImageRef;
        
        void updateTrapezoid();
        void updateTimeRemaining();
        
    public:
        SessionMetaLabel();
        ~SessionMetaLabel();
        
        void set(const string& timeStart, const string& endTime, time_t timestamp, const gl::Texture& clockImageRef);
        void update();
        
        void draw();
        
        void on();
        void off();
    };
}


#endif
