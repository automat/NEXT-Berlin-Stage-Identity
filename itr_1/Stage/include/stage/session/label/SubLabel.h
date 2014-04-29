//
//  SessionMetaLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_SubLabel_h
#define SessionView_SubLabel_h

#include "stage/session/label/AbstractLabel.h"
#include <string>

namespace next {
    using namespace std;
    class SubLabel : public AbstractLabel {
    private:
        float       mTextBoxWidth;
        Vec2f       mVertexTrapezoid[4];
        Anim<float> mAlphaState;
        Anim<Vec2f> mPositionState;
    public:
        SubLabel();
        
        void set(const string& str);
        
        void draw();
        
        void show();
        void hide();
        void on();
        void off();
        
        float getWidth();
    };
}


#endif
