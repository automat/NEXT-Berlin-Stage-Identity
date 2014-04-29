//
//  LineQuad.h
//  SessionView
//
//  Created by Henryk Wollik on 22/04/14.
//
//

#ifndef SessionView_LineQuad_h
#define SessionView_LineQuad_h

#include "cinder/Vector.h"
#include "cinder/Timeline.h"

namespace next {
    using namespace ci;
    struct LineQuad{
        Vec2f         posTarget;
        
        Anim<Vec2f>   posState;
        Anim<float>   alphaState;
        
        vector<Vec2f> vertices;
        vector<Vec2f> texcoords;
        
        LineQuad() : posState(Vec2f()), alphaState(0.0f){}
        
    };
}


#endif
