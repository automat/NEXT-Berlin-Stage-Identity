//
//  EventTitleLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_EventTitleLabel_h
#define SessionView_EventTitleLabel_h

#include "AbstractLabel.h"

namespace next {
    class EventTitleLabel : public AbstractLabel {
    public:
        EventTitleLabel();
        
        void draw();
        void update();
    };
}


#endif
