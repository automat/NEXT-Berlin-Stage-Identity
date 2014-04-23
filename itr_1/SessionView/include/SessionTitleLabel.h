//
//  SessionTitleLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_SessionTitleLabel_h
#define SessionView_SessionTitleLabel_h

#include "AbstractLabel.h"
#include "LineQuad.h"

namespace next {
    class SessionTitleLabel : public AbstractLabel {
        vector<LineQuad> mLineQuads;
        void genQuads();
        
    public:
        SessionTitleLabel();
        
        void set(const string& title);
        void draw();
        
        void on();
        void off();
    };
}

#endif
