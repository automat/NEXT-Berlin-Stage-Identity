#include "SubLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


namespace next {
    using namespace boost;
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
    
    void SubLabel::draw(){
        if(mTextBox->getString().empty()){
            return;
        }
        Vec2f topLeft = mTextBox->getTopLeft();
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, 0);
        
        glPushMatrix();
        glTranslatef(SESSION_LABEL_META_OFFSET_X, SESSION_LABEL_META_OFFSET_Y, 0);
        glColor3f(0,0,0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &mVertexTrapezoid[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
        
        glColor3f(1, 1, 1);
        glTranslatef(topLeft.x, topLeft.y, 0);
        gl::draw(mTextBox->getTexture());
#ifdef SESSION_VIEW_LABEL_SESSION_META_DEBUG_DRAW
        mTextBox->debugDraw();
#endif
        glPopMatrix();
    }
    
    void SubLabel::update(){}
    
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
    
    void SubLabel::show(){
        
    }
    
    void SubLabel::hide(){
        
    }
    
    void SubLabel::on(){
        
    }
    
    void SubLabel::off(){
        
    }
}