//
//  TextBoxTexture.h
//  Stage
//
//  Created by Henryk Wollik on 29/04/14.
//
//

#ifndef Stage_TextBoxTexture_h
#define Stage_TextBoxTexture_h

#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include <vector>

namespace next {
    using namespace ci;
    using namespace std;
    
    struct TextBoxTexture {
        Vec2f                 calculatedSize;
        vector<vector<Vec2f>> texcoords;
        gl::Texture           texture;
        Vec2f                 topleft;
    };
    
}

#endif
