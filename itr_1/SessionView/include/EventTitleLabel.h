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
            Anim<float>   scaleState;
            Anim<float>   alphaState;
            
            vector<Vec2f> vertices;
            vector<Vec2f> texcoords;
            
            LineQuad() : scaleState(1.0f), alphaState(1.0f){}
            
        };
        
        string        mString;
        vector<LineQuad> mLineQuads;
        
        void genQuads();
        
    public:
        EventTitleLabel();
        
        void draw();
        void update();
        void setString(const string& str);
    };
}


#endif
