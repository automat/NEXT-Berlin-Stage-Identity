//
//  EventMetaLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_EventMetaLabel_h
#define SessionView_EventMetaLabel_h

#include "stage/session/label/AbstractLabel.h"
#include "stage/session/label/event/PingPongEventMetaTypeLabel.h"
#include "data/session/Event.h"
#include "util/text/TextBoxTexture.h"

#include <map>
#include <vector>

namespace next {
    using namespace std;
    class EventMetaLabel : public AbstractLabel {
        static map<string,   TextBoxTexture> sMapTypeTexture;
        static map<uint32_t, TextBoxTexture> sMapIndexTexture;
        
    public:
         static void Map(map<uint32_t,Event>* events);
    private:
        
        int  mIndex;
        bool mActive;
        
        Anim<float> mAlphaState;
        float       mTextBoxFrontWidth;
        Vec2f       mVertexTrapezoidIndex[4];

        PingPongEventMetaTypeLabel* mSubLabel;
        
    public:
        EventMetaLabel();
        ~EventMetaLabel();
        
        void set(const string& type, uint32_t index);
        
        void draw();
        
        void on();
        void off();
    };
}

#endif
