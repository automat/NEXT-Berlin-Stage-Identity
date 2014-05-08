//
//  NextLogo.h
//  SessionView
//
//  Created by Henryk Wollik on 24/04/14.
//
//

#ifndef SessionView_NextLogo_h
#define SessionView_NextLogo_h

#include <OpenGL/OpenGL.h>

#include "Resources.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"
#include "cinder/Matrix44.h"

#include "Config.h"


namespace next {
    using namespace ci;
    class NEXTLogo{
        gl::Texture    mImage;
        Matrix44f      mTransform;
        TextBoxTexture mText;
        
    public:
        NEXTLogo(){
            mImage = gl::Texture(loadImage(app::loadResource(RES_NEXT_LOGO)));
            mTransform.setToIdentity();
            mTransform.translate(Vec3f(STAGE_WIDTH - LOGO_MARGIN_RIGHT, LOGO_MARGIN_TOP, 0));
            
            TextBox textbox;
        }
        
        inline void draw(){
            glPushMatrix();
                glMultMatrixf(&mTransform[0]);
                glColor4f(1, 1, 1, 1);
                gl::draw(mImage);
            glPopMatrix();
        }
    };
    
}


#endif
