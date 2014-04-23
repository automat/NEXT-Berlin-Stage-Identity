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
#include "PingPongEventMetaSubLabel.h"
#include <string>

namespace next {
    using namespace std;
    class EventMetaLabel : public AbstractLabel {
        static Font sFont;

        bool mActive;
        
        Anim<float> mAlphaState;
        float mTextBoxFrontWidth;
        Vec2f mVertexTrapezoidIndex[4];

        PingPongEventMetaSubLabel* mSubLabel;
        
    public:
        EventMetaLabel();
        ~EventMetaLabel();
        
        void set(const string& type, const string& index);
        
        void draw();
        
        void on();
        void off();
    };
}

#endif
