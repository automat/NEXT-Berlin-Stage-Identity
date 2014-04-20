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
        TextBox* mTextBoxSub;
        float    mTextBoxWidth;
    public:
        SessionMetaLabel();
        ~SessionMetaLabel();
        
        void setTime(const string& timeStart, const string& endTime, time_t timestamp);
        
        void draw();
        void update();
    };
}


#endif
