//
//  SessionTitleLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_SessionTitleLabel_h
#define SessionView_SessionTitleLabel_h

#include "stage/session/label/AbstractLabel.h"
#include "stage/session/label/LineQuad.h"

namespace next {
    class SessionTitleLabel : public AbstractLabel {
        TextBoxTexture   mTitle;
        vector<LineQuad> mLineQuads;
        
    public:
        SessionTitleLabel();
        
        void set(const string& title);
        void draw();
        
        void on();
        void off();
    };
}

#endif
