#include "EventMetaLabel.h"
#include "EventMetaLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


namespace next {
    using namespace boost;
    
    typedef EaseInOutQuad AnimEaseInOut;

    Font EventMetaLabel::sFont;
    
    EventMetaLabel::EventMetaLabel() : AbstractLabel(), mTextBoxFrontWidth(0), mAlphaState(0), mActive(false){
        static bool __fontInitialzed(false);
        if(!__fontInitialzed){
            sFont = Font(app::loadResource(RES_AKKURAT_BOLD),SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR);
        }

        mTextBox->setFont(      sFont);
        mTextBox->setWidth(     SESSION_LABEL_EVENT_BOX_WIDTH);
        mTextBox->setFontSize(  SESSION_LABEL_META_FONT_SIZE);
        mTextBox->setColorFont( SESSION_LABEL_EVENT_META_FONT_COLOR);

        mSubLabel = new PingPongEventMetaSubLabel();
        
        setPosition(SESSION_LABEL_EVENT_META_POS);
    }
    
    EventMetaLabel::~EventMetaLabel(){
        delete mSubLabel;
    }

    void EventMetaLabel::draw(){
        if(mTextBox->getString().empty()){
            return;
        }
        
        Vec2f topLeft = mTextBox->getTopLeft();
        float alpha   = mAlphaState();
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, 0);
        
                glPushMatrix();
                    glTranslatef(mTextBoxFrontWidth, 0, 0);
                    mSubLabel->draw();
                glPopMatrix();
        
                glPushMatrix();
                    glPushMatrix();
                        glTranslatef(SESSION_LABEL_META_OFFSET_X, SESSION_LABEL_META_OFFSET_Y, 0);
                        glColor4f(0,0,0, alpha);
                        glEnableClientState(GL_VERTEX_ARRAY);
                        glVertexPointer(2, GL_FLOAT, 0, &mVertexTrapezoidIndex[0]);
                        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                        glDisableClientState(GL_VERTEX_ARRAY);
                    glPopMatrix();
        
                    glColor4f(1,1,1,alpha);
                    glTranslatef(topLeft.x, topLeft.y, 0);
                    gl::draw(mTextBox->getTexture());
#ifdef SESSION_VIEW_LABEL_EVENT_META_DEBUG_DRAW
                    mTextBox->debugDraw();
#endif
                    glColor4f(1,1,1,1);
                glPopMatrix();
       glPopMatrix();
    }
    
    void EventMetaLabel::set(const string& type, const string& index){
        mTextBox->setString(index);
        mTextBoxFrontWidth = mTextBox->getCalculatedSize().x + SESSION_LABEL_EVENT_META_TYPE_INDEX_SPACING;

        float trapezoidIndexWidth = mTextBoxFrontWidth + SESSION_LABEL_META_OFFSET_X * -0.5f;
        static const float slope = 14.5f;
        
        mVertexTrapezoidIndex[0] = Vec2f(slope,0);
        mVertexTrapezoidIndex[1] = Vec2f(trapezoidIndexWidth + slope,0);
        mVertexTrapezoidIndex[2] = Vec2f(0, SESSION_LABEL_META_BOX_HEIGHT);
        mVertexTrapezoidIndex[3] = Vec2f(trapezoidIndexWidth, SESSION_LABEL_META_BOX_HEIGHT);
        
        if(mActive){
            mSubLabel->hide();
            mSubLabel->swap();
        }
        
        mSubLabel->set(type);
        if(mActive){
            mSubLabel->show();
        }
    }
    
    void EventMetaLabel::on(){
        mActive = true;
        tween(&mAlphaState, 0.0f, 1.0f,
              SESSION_LABEL_EVENT_META_ANIM_TIME_ALPHA_ON,
              AnimEaseInOut());
        mSubLabel->on();
    }
    
    void EventMetaLabel::off(){
        mActive = false;
        tween(&mAlphaState, 1.0f, 0.0f,
              1.5f,
              AnimEaseInOut());
        mSubLabel->off();
        mSubLabel->swap();
    }
    
    
    
}