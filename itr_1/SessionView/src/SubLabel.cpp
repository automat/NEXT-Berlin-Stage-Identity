#include "SubLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


namespace next {

    using namespace boost;
    typedef EaseInOutQuad AnimEaseInOut;

    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/

    SubLabel::SubLabel() :
        AbstractLabel(),
        mTextBoxWidth(0),
        mAlphaState(0.0f),
        mPositionState(Vec2f()){
            mTextBox->setFont(      Font(app::loadResource(RES_AKKURAT_BOLD), SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR));
            mTextBox->setWidth(     SESSION_LABEL_SESSION_META_BOX_WIDTH);
            mTextBox->setFontSize(  SESSION_LABEL_META_FONT_SIZE);
            mTextBox->setColorFont( SESSION_LABEL_SESSION_META_FONT_COLOR);
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Draw
    /*--------------------------------------------------------------------------------------------*/

    void SubLabel::draw(){
        if(mTextBox->getString().empty()){
            return;
        }
        Vec2f offset  = mPositionState();
        Vec2f pos     = mPos + offset;
        Vec2f topLeft = mTextBox->getTopLeft();
        float alpha   = mAlphaState();
        
        glPushMatrix();
            glTranslatef(pos.x, pos.y, 0);
        
            glPushMatrix();
                glTranslatef(SESSION_LABEL_META_OFFSET_X, SESSION_LABEL_META_OFFSET_Y, 0);
                glColor4f(0,0,0,alpha);
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(2, GL_FLOAT, 0, &mVertexTrapezoid[0]);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();
        
            glColor4f(1, 1, 1, alpha);
            glTranslatef(topLeft.x, topLeft.y, 0);
            gl::draw(mTextBox->getTexture());
#ifdef SESSION_VIEW_LABEL_SESSION_META_DEBUG_DRAW
            mTextBox->debugDraw();
#endif
        glColor4f(1, 1, 1, 1);
        glPopMatrix();
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Set
    /*--------------------------------------------------------------------------------------------*/

    void SubLabel::set(const string& str){
        mTextBox->setString(str);
        mTextBoxWidth = mTextBox->getCalculatedSize().x + SESSION_LABEL_EVENT_META_TYPE_INDEX_SPACING;

        float trapezoidWidth = mTextBoxWidth + SESSION_LABEL_META_OFFSET_X * -2;
        static const float slope = 14.5f;
        
        mVertexTrapezoid[0] = Vec2f(slope,0);
        mVertexTrapezoid[1] = Vec2f(trapezoidWidth + slope,0);
        mVertexTrapezoid[2] = Vec2f(0,SESSION_LABEL_META_BOX_HEIGHT);
        mVertexTrapezoid[3] = Vec2f(trapezoidWidth, SESSION_LABEL_META_BOX_HEIGHT);
    }
    
    float SubLabel::getWidth(){
        return mTextBoxWidth;
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Tigger state
    /*--------------------------------------------------------------------------------------------*/

    void SubLabel::show(){
        const static Vec2f offset(SESSION_LABEL_EVENT_META_ANIM_OFFSET_IN,0);
        const static Vec2f zero;
        
        tween(&mPositionState, offset, zero,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_OFFSET_SHOW,
              AnimEaseInOut());
        
        tween(&mAlphaState, 0.0f, 1.0f,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_ALPHA_SHOW,
              AnimEaseInOut());
    }
    
    void SubLabel::hide(){
        tween(&mAlphaState, 0.0f,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIN_TIME_ALPHA_HIDE,
              AnimEaseInOut());
    }
    
    void SubLabel::on(){
        tween(&mAlphaState, 1.0f,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_ALPHA_ON,
              AnimEaseInOut());
     }
    
    void SubLabel::off(){
        tween(&mAlphaState, 0.0f,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_ALPHA_OFF,
              AnimEaseInOut());
    }
}