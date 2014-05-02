//
//  TwitterCTA.h
//  Stage
//
//  Created by Henryk Wollik on 02/05/14.
//
//

#ifndef Stage_TwitterCTA_h
#define Stage_TwitterCTA_h

#include "Resources.h"
#include "cinder/gl/Texture.h"
#include "cinder/Matrix44.h"
#include "util/text/TextBox.h"
#include "util/text/TextBoxTexture.h"
#include "stage/AbstractAnimBase.h"

namespace next {
    using namespace ci;
    
    
    
    class TwitterCTA : public AbstractAnimBase {
        
        typedef EaseInOutCubic AnimEaseInOut;
        
        TextBoxTexture mText0;
        TextBoxTexture mText1;
        TextBoxTexture mText2;
        gl::Texture    mImage;
        Matrix44f      mTransform;
        
        Anim<float>    mAlphaState;
        
        
        
    public:
        TwitterCTA() :
            AbstractAnimBase(),
            mAlphaState(0.0f){
            mImage = gl::Texture(loadImage(app::loadResource(RES_IMAGE_TWITTER)));
            mTransform.setToIdentity();
            mTransform.translate(Vec3f(TWITTER_LOGO_MARGIN_LEFT, STAGE_HEIGHT - TWITTER_LOGO_MARGIN_BOTTOM, 0));
            
            TextBox textbox;
            textbox.setFont(Font(app::loadResource(RES_AKKURAT_BOLD),38 * 8));
            textbox.setFontSize(38);
            textbox.setColorFont(Color::white());
            textbox.setWidth(800);
            
            textbox.setString("What is your New Normal?");
            mText0.topleft = textbox.getTopLeft();
            mText0.calculatedSize = textbox.getCalculatedSize();
            mText0.texture = gl::Texture(Surface(textbox.getTexture()));
            
            textbox.setString("Selected tweets from");
            mText1.topleft = textbox.getTopLeft();
            mText1.calculatedSize = textbox.getCalculatedSize();
            mText1.texture = gl::Texture(Surface(textbox.getTexture()));
            
            textbox.setColorFont(COLOR_NEXT_BLUE);
            textbox.setString("#newnormal #next14");
            mText2.topleft = textbox.getTopLeft();
            mText2.calculatedSize = textbox.getCalculatedSize();
            mText2.texture = gl::Texture(Surface(textbox.getTexture()));
            
            
        }
        
        inline void draw(){
            float alpha = mAlphaState();
            if(alpha == 0.0f){
                return;
            }
            
            glPushMatrix();
            glMultMatrixf(&mTransform[0]);
            glColor4f(1, 1, 1, alpha);
            gl::draw(mImage);
            glTranslatef(TWITTER_LOGO_SIZE + 20, 20, 0);
            gl::draw(mText0.texture);
            glTranslatef(0, 38 + 8, 0);
            gl::draw(mText1.texture);
            glTranslatef(mText1.calculatedSize.x + 5, 0, 0);
            gl::draw(mText2.texture);
            glPopMatrix();
        }
        
        inline void on(){
            tween(&mAlphaState, mAlphaState(), 1.0f, 1.0f, AnimEaseInOut());
            
        }
        
        inline void off(){
            tween(&mAlphaState, mAlphaState(), 0.0f, 1.0f, AnimEaseInOut());
        }
        
    };
}




#endif
