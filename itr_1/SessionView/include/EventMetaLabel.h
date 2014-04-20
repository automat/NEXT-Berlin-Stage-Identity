//
//  EventMetaLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_EventMetaLabel_h
#define SessionView_EventMetaLabel_h

#include "AbstractLabel.h"
#include <string>

namespace next {
    using namespace std;
    class EventMetaLabel : public AbstractLabel {
        TextBox* mTextBoxSub;
        float    mTextBoxWidth;
    public:
        EventMetaLabel();
        ~EventMetaLabel();
        
        void setType(const string& str);
        void setIndex(const string& str);
        
        void draw();
        void update();
    };
}

#endif
