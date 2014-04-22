//
//  SessionMetaLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_SessionMetaLabel_h
#define SessionView_SessionMetaLabel_h

#include "AbstractLabel.h"
#include <string>

namespace next {
    using namespace std;
    class SessionMetaLabel : public AbstractLabel {
    private:
        Anim<float> mAlphaState;
        float mTextBoxFrontWidth;
        next::TextBox* mTextBoxSub;
        Vec2f mVertexTrapezoid[4];
        
        
    public:
        SessionMetaLabel();
        ~SessionMetaLabel();
        
        void set(const string& timeStart, const string& endTime, time_t timestamp);
        
        void draw();
        
        void on();
        void off();
    };
}


#endif
