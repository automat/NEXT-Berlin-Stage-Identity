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

namespace next {
    class SessionTitleLabel : public AbstractLabel {
    public:
        SessionTitleLabel();
        
        void setString(const string& str);
        void draw();
    };
}

#endif
