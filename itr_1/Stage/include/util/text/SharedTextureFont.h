//
//  SharedTextureFont.h
//  grid_00
//
//  Created by Henryk Wollik on 02/04/14.
//
//

#ifndef grid_00_SharedTextureFont_h
#define grid_00_SharedTextureFont_h

#include "cinder/app/App.h"
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"
#include "Resources.h"

namespace next{
    using namespace  ci;

    class SharedTextureFont {
    public:
        inline static const gl::TextureFontRef& Get(){
            const static gl::TextureFontRef fontRef = gl::TextureFont::create(ci::Font(app::loadResource(RES_FONT_APERCU_MONO),18));
            return fontRef;
        }
    };
}




#endif
