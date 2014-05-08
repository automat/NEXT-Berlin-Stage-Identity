//
//  SessionMetaLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_SubLabel_h
#define SessionView_SubLabel_h

#include <string>
#include <map>
#include <vector>

#include "stage/session/label/AbstractLabel.h"
#include "data/session/Event.h"

namespace next {
    using namespace std;
    class EventTypeLabel : public AbstractLabel {
        static map<string, TextBoxTexture> sMapTypeTexture;
        
        string      mKeyType;
        
        float       mTextBoxWidth;
        Vec2f       mVertexTrapezoid[4];
        Anim<float> mAlphaState;
        Anim<Vec2f> mPositionState;
    public:
        EventTypeLabel();
        
        void set(const string& str);
        
        void draw();
        
        void show();
        void hide();
        void on();
        void off();
        
        float getWidth();
   
        static void Map(map<uint32_t,Event>* events);
    };
}


#endif
