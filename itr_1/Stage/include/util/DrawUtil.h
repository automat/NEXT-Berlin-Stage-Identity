//
//  DrawUtil.h
//  grid_00
//
//  Created by Henryk Wollik on 30/03/14.
//
//

#ifndef grid_00_DrawUtil_h
#define grid_00_DrawUtil_h

#include <string>
#include <OpenGL/Opengl.h>
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"

namespace utils {
    using namespace std;
    
    
    inline void drawUnitQuad(){
        static float vertices[] = {0,0,1,0,1,1,0,1};
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &vertices[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    using namespace ci;
    inline void drawTexture(const gl::Texture& texture){
        const Vec2f windowSize(app::getWindowSize());
        
        gl::disableDepthRead();
        gl::pushMatrices();
        gl::setMatricesWindow(windowSize.x, windowSize.y, true);
        gl::draw(texture, app::getWindowBounds());
        gl::popMatrices();
        gl::enableDepthRead();
    
    }
    
    inline void drawClearedScreenRect(const Vec2i& size, bool originUpperLeft = true){
        gl::pushMatrices();
        glClearColor( 1, 1 ,1, 1 );
        glClearDepth(1.0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        gl::setMatricesWindow(size,originUpperLeft);
        gl::drawSolidRect( Rectf( 0, 0, size.x, size.y) );
        gl::popMatrices();
    }
    
    //! draws string top aligned
    inline void drawString(const gl::TextureFontRef& font, const string& str, Vec2f pos = Vec2f(), float scale = 1){
        pos.y += font->getAscent() - font->getDescent();
        glPushMatrix();
        glScalef(scale, scale, scale);
        font->drawString(str, pos);
        glPopMatrix();
    }
   
}



#endif
