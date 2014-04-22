#include "SessionMetaLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


namespace next {
    using namespace boost;
    SessionMetaLabel::SessionMetaLabel() : AbstractLabel(), mTextBoxFrontWidth(0){
        mTextBox->setFont(      Font(app::loadResource(RES_AKKURAT_BOLD), SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR));
        mTextBox->setWidth(     SESSION_LABEL_SESSION_META_BOX_WIDTH);
        mTextBox->setFontSize(  SESSION_LABEL_META_FONT_SIZE);
        mTextBox->setColorFont( SESSION_LABEL_SESSION_META_FONT_COLOR);
        
        mTextBoxSub = new TextBox();
        mTextBoxSub->setFont(      Font(app::loadResource(RES_AKKURAT_REGULAR), SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR));
        mTextBoxSub->setWidth(     SESSION_LABEL_META_SUB_BOX_WIDTH);
        mTextBoxSub->setFontSize(  SESSION_LABEL_META_FONT_SIZE);
        mTextBoxSub->setColorFont( SESSION_LABEL_SESSION_META_FONT_COLOR);
        
        setPosition(SESSION_LABEL_SESSION_META_POS);
    }

    SessionMetaLabel::~SessionMetaLabel() {
        delete mTextBoxSub;
    }

    void SessionMetaLabel::draw(){
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
        glTranslatef(mTextBoxFrontWidth, 0, 0);
        gl::draw(mTextBoxSub->getTexture());
#ifdef SESSION_VIEW_LABEL_SESSION_META_DEBUG_DRAW
        mTextBoxSub->debugDraw();
#endif
        glPopMatrix();
    }
    
    void SessionMetaLabel::set(const string& timeStart, const string& endTime, time_t timestamp){
        mTextBox->setString(timeStart + " - " + endTime);
        mTextBoxSub->setString("in 23 min");
        mTextBoxFrontWidth = mTextBox->getCalculatedSize().x + SESSION_LABEL_EVENT_META_TYPE_INDEX_SPACING;
        
        float textBoxesWidth = mTextBoxFrontWidth + mTextBoxSub->getCalculatedSize().x;
        float trapezoidWidth = textBoxesWidth + SESSION_LABEL_META_OFFSET_X * -2;
        
        static const float slope = 14.5f;
        
        mVertexTrapezoid[0] = Vec2f(slope,0);
        mVertexTrapezoid[1] = Vec2f(trapezoidWidth + slope,0);
        mVertexTrapezoid[2] = Vec2f(0,SESSION_LABEL_META_BOX_HEIGHT);
        mVertexTrapezoid[3] = Vec2f(trapezoidWidth, SESSION_LABEL_META_BOX_HEIGHT);
        
    }
}