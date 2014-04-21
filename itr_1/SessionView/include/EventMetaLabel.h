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
#include "SubLabel.h"
#include <string>

namespace next {
    using namespace std;
    class EventMetaLabel : public AbstractLabel {
        
        float mTextBoxFrontWidth;
        Vec2f mVertexTrapezoidIndex[4];
        Vec2f mVertexTrapezoidType[4];
        
        next::SubLabel* mSubLabel;
        
    public:
        EventMetaLabel();
        ~EventMetaLabel();
        
        void set(const string& type, const string& index);
        
        void draw();
        void update();
    };
}

#endif
