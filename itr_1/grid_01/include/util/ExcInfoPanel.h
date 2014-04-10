//
//  JsonExcInfoPanel.h
//  grid_01
//
//  Created by Henryk Wollik on 08/04/14.
//
//

#ifndef grid_01_ExcInfoPanel_h
#define grid_01_ExcInfoPanel_h

#include <string>
#include <iostream>

#include "Resources.h"
#include "cinder/Matrix33.h"
#include "cinder/Vector.h"
#include "cinder/Text.h"
#include "cinder/Font.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rect.h"
#include "cinder/app/App.h"


namespace utils {
    using namespace std;
    using namespace ci;

    class ExcInfoPanel{
        Vec2i       mPos;
        Vec2i       mSize;
        string      mString;

        TextBox     mTextBox;
        gl::Texture mTexture;

    public:
        ExcInfoPanel() :
            mSize(400,200){
                mTextBox = TextBox().alignment(TextBox::CENTER).font(Font("Arial Bold",20)).size(Vec2i(mSize.x - 40,TextBox::GROW));
        }

        inline void setString(const string& string){
            if(string == mString){
                return;
            }
            mTextBox.text(string);
            mTexture = gl::Texture(mTextBox.render());
            mSize.y  = mTexture.getHeight();

            Vec2i windowSize = app::getWindowSize();
            mPos.x = windowSize.x / 2 - mSize.x / 2;
            mPos.y = windowSize.y / 2 - mSize.y / 2;

            mString = string;
        }

        inline void clear(){
            if(mString.empty()){
                return;
            }
            mString.clear();
        }

        inline void draw(){
            if(mString.empty()){
                return;
            }

            Vec2i windowSize = app::getWindowSize();
            static float padding = 20;;

            gl::enableAlphaTest();
            gl::enableAdditiveBlending();
            gl::disableDepthRead();
            gl::pushMatrices();
            gl::setMatricesWindow(windowSize, true);

            glPushMatrix();
            glColor3f(0,0,0);
            glTranslatef(mPos.x, mPos.y, 0);
            gl::drawSolidRect(Rectf(-padding,-padding,-padding + mTextBox.measure().x,30));
            glColor3f(1,1,1);
            gl::draw(mTexture);

            glPopMatrix();

            gl::popMatrices();
            gl::enableDepthRead();
            gl::disableAlphaBlending();
            gl::disableAlphaTest();
        }
    };
}


#endif
