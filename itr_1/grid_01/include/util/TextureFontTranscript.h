//
//  FontTranscript.h
//  grid_01
//
//  Created by Henryk Wollik on 11/04/14.
//
//

#ifndef grid_01_FontTranscript_h
#define grid_01_FontTranscript_h

#include "cinder/app/App.h"
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Vector.h"
#include "Resources.h"

using namespace  ci;

class TextureFontTranscript {
public:
    enum FontSize{
        Size_96 = 0,
        Size_48,
        Size_24,
        Size_12
    };
    
    inline static const gl::TextureFontRef& GetSize96(){
        const static gl::TextureFontRef fontRef = gl::TextureFont::create(ci::Font(app::loadResource(RES_FONT_TRANSCRIPT),192));
        return fontRef;
    }
    
    inline static const gl::TextureFontRef& GetSize48(){
        const static gl::TextureFontRef fontRef = gl::TextureFont::create(ci::Font(app::loadResource(RES_FONT_TRANSCRIPT),96));
        return fontRef;
    }
    
    inline static const gl::TextureFontRef& GetSize24(){
        const static gl::TextureFontRef fontRef = gl::TextureFont::create(ci::Font(app::loadResource(RES_FONT_TRANSCRIPT),24));
        return fontRef;
    }
    
    inline static const gl::TextureFontRef& GetSize12(){
        const static gl::TextureFontRef fontRef = gl::TextureFont::create(ci::Font(app::loadResource(RES_FONT_TRANSCRIPT),12));
        return fontRef;
    }
    
    inline static void drawString(const string& str, Vec2f pos, const FontSize& size = FontSize::Size_96){
        gl::TextureFontRef fontRef;
        switch (size) {
            case Size_96:
                fontRef = GetSize96();
                break;
            case Size_48:
                fontRef = GetSize48();
                break;
            case Size_24:
                fontRef = GetSize24();
                break;
            case Size_12:
                fontRef = GetSize12();
                break;
            default:
                return;
                break;
        }
        pos.y += fontRef->getAscent() - fontRef->getDescent();
        glPushMatrix();
        glScalef(0.5f, 0.5f, 0.5f);
        fontRef->drawString(str, pos);
        glPopMatrix();
    }
    
    
};

#endif
