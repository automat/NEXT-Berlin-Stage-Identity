//
//  WindowParseError.h
//  NextQuoteProcessor
//
//  Created by Henryk Wollik on 27/03/14.
//
//

#ifndef NextQuoteProcessor_WindowParseError_h
#define NextQuoteProcessor_WindowParseError_h

#include <string>
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include <OpenGL/OpenGL.h>

#include "CinderGwen.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/Label.h"
#include "Gwen/Controls/TextBox.h"


using namespace ci;

class WindowParseError : public Gwen::Controls::WindowControl {
    TextBox     mTextbox;
    gl::Texture mTexture;
public:
    WindowParseError(Gwen::Controls::Base* base) :
    Gwen::Controls::WindowControl(base){
        SetTitle("Parse Error");
        SetClosable(false);
        SetTabable(false);
        SetSize(400, 200);
        
        mTextbox = TextBox().alignment(TextBox::CENTER).font(Font("Arial Bold",11)).size(Vec2i(GetSize().x - 40,TextBox::GROW));
        mTextbox.setColor( Color( 1,1,1 ) );
    }
    
    inline void Render( Gwen::Skin::Base* skin){
        Gwen::Controls::WindowControl::Render(skin);
        
        float windowWidth  = app::getWindowWidth();
        float windowHeight = app::getWindowHeight();
        int   width  = GetSize().x;
        int   height = GetSize().y;
        
        SetPos(windowWidth * 0.5f - width * 0.5f, windowHeight * 0.5f - height * 0.5f);
        
        float posX = GetPos().x;
        float posY = GetPos().y;
        
        const static Font font("Arial",20);
        //const static ci::gl::TextureFontRef font = ci::gl::TextureFont::create(ci::Font("Arial",20));
        //TextBox tbox = TextBox().alignment( TextBox::CENTER ).font( font ).size( Vec2i( width, TextBox::GROW) ).text( mString );
        
        
        glColor3f(1,1,1);
        glPushMatrix();
        glTranslatef(posX + 20, posY + 25.0f + 20, 0);
        gl::draw(mTexture);
        glPopMatrix();
        
        
    }
    
    inline void setMessage(const std::string& str){
        mTextbox.text(str);
        mTexture = gl::Texture(mTextbox.render());
        
        SetSize(GetSize().x, 25.0f + mTexture.getHeight() + 40);
        
    }
};


#endif
