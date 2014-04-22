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
#include <vector>

namespace next {
    using namespace std;
    class EventTitleLabel : public AbstractLabel {
        struct LineQuad{
            Vec2f         posTarget;
            
            Anim<Vec2f>   posState;
            Anim<float>   alphaState;

            vector<Vec2f> vertices;
            vector<Vec2f> texcoords;
            
            LineQuad() : posState(Vec2f()), alphaState(0.0f){}
            
        };
        
        string           mString;
        vector<LineQuad> mLineQuads;
       
        void genQuads();
        
    public:
        EventTitleLabel();
        
        void draw();
        void setString(const string& str);
        
        void on();
        void off();
        
        void show();
        void hide();
    };
}


#endif
