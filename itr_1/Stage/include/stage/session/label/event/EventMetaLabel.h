//
//  EventMetaLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_EventMetaLabel_h
#define SessionView_EventMetaLabel_h

#include <map>
#include <vector>

#include "stage/session/label/AbstractLabel.h"
#include "stage/session/label/event/PingPongEventMetaTypeLabel.h"


namespace next {
    using namespace std;
    class EventMetaLabel : public AbstractLabel {
        static map<string,   TextBoxTexture> sMapTypeTexture;
        static map<uint32_t, TextBoxTexture> sMapIndexTexture;
        
        int  mKeyIndex;
        bool mActive;
        
        Anim<float> mAlphaState;
        float       mTextBoxFrontWidth;
        Vec2f       mVertexTrapezoidIndex[4];

        PingPongEventMetaTypeLabel* mSubLabel;
        
    public:
        static void Map(map<uint32_t,Event>* events);
        
        EventMetaLabel();
        ~EventMetaLabel();
        
        void set(const string& type, uint32_t index);
        
        void draw();
        
        void on();
        void off();
    };
}

#endif
