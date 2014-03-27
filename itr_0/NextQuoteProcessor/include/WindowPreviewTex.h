//
//  WindowPreviewTex.h
//  NextQuoteProcessor
//
//  Created by Henryk Wollik on 25/03/14.
//
//

#ifndef NextQuoteProcessor_WindowPreviewTex_h
#define NextQuoteProcessor_WindowPreviewTex_h

#include "CinderGwen.h"
#include "Gwen/Controls/WindowControl.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "Cell.h"
#include "Quote.h"
#include "QuoteTypesetter.h"
#include "Grid.h"

using namespace ci;

class WindowPreviewTex : public Gwen::Controls::WindowControl {
    QuoteRef mQuote;
    QuoteTypesetter* mTypesetter;
    bool mDrawQuote;
    
public:
    WindowPreviewTex(Gwen::Controls::Base* base) : Gwen::Controls::WindowControl(base),
    mDrawQuote(false){
        SetTitle("Texture View");
        SetClosable(false);
        SetTabable(false);
        Dock(Gwen::Pos::Fill);
        
    }
    
    inline void connect(QuoteTypesetter* typesetter){
        mTypesetter = typesetter;
    }
    
    inline void drawQuote(bool b){
        mDrawQuote = b;
    }
    
    inline void updateLayout(){
        mQuote = mTypesetter->getQuote();
    }
    
    inline void Render( Gwen::Skin::Base* skin){
        //Gwen::Controls::WindowControl::Render(skin);

        static const float paddingT = 0.0f;
        
        float windowHeight   = app::getWindowHeight();
        float viewportWidth  = this->GetSize().x;
        float viewportHeight = this->GetSize().y - paddingT;
        float globalPosX     = LocalPosToCanvas().x;
        float globalPosY     = LocalPosToCanvas().y + paddingT;
        
        Area viewport(Rectf(globalPosX,                windowHeight - globalPosY,
                            globalPosX + viewportWidth,windowHeight - viewportHeight - globalPosY));

        glPushAttrib(GL_VIEWPORT_BIT);
        gl::setViewport(viewport);
        gl::setMatricesWindow(viewportWidth, viewportHeight, true);
        
        glColor3f(0,0,0);
        gl::drawSolidRect(Rectf(0,0,viewportWidth,viewportHeight));
        gl::setMatricesWindow(viewport.getWidth(),viewport.getHeight());
        
        if(mQuote == nullptr){
            gl::setMatricesWindow(app::getWindowSize());
            glPopAttrib();
            return;
        }
        
        float unit = MIN(viewportWidth,viewportHeight);
        
        static const float padding = 25.0f;
        Rectf rect  = Rectf(0,0,unit - padding * 2, unit - padding * 2);
        Vec2f trans = Vec2f(viewportWidth  * 0.5f - rect.getWidth() * 0.5f,
                            viewportHeight * 0.5  - rect.getHeight() * 0.5f);
        
        glPushMatrix();
        glTranslatef(trans.x,trans.y,0);

        glColor3f(0.5f, 0, 0.125f);
        gl::drawStrokedRect(rect);
        
        if(mDrawQuote){
            glEnable(GL_TEXTURE_2D);
            glColor3f(1, 1, 1);
            gl::draw(mQuote->getTexture(),rect);
            glDisable(GL_TEXTURE_2D);
        }
        
        glPopMatrix();
        gl::setMatricesWindow(app::getWindowSize());
        glPopAttrib();
    }
};



#endif
