#include "EventMetaLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


namespace next {
    using namespace boost;
    
    EventMetaLabel::EventMetaLabel() : AbstractLabel(), mTextBoxFrontWidth(0){
        mTextBox->setFont(      Font(app::loadResource(RES_AKKURAT_BOLD),SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR));
        mTextBox->setWidth(     SESSION_LABEL_EVENT_BOX_WIDTH);
        mTextBox->setFontSize(  SESSION_LABEL_META_FONT_SIZE);
        mTextBox->setColorFont( SESSION_LABEL_EVENT_META_FONT_COLOR);

        mSubLabel = new next::SubLabel();
        
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
        
        glPushMatrix();
            glTranslatef(mPos.x, mPos.y, 0);
        
            /*
            glPushMatrix();
                glTranslatef(topLeft.x + mTextBoxFrontWidth, topLeft.y, 0);
                
                glPushMatrix();
                    glTranslatef(SESSION_LABEL_META_OFFSET_X, SESSION_LABEL_META_OFFSET_Y, 0);
                    glColor3f(0,0,0);
                    glEnableClientState(GL_VERTEX_ARRAY);
                    glVertexPointer(2, GL_FLOAT, 0, &mVertexTrapezoidType[0]);
                    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                    glDisableClientState(GL_VERTEX_ARRAY);
                glPopMatrix();
        
                glColor3f(1,1,1);
                gl::draw(mTextBoxSub->getTexture());
        #ifdef SESSION_VIEW_LABEL_EVENT_META_DEBUG_DRAW
                mTextBoxSub->debugDraw();
        #endif
            glPopMatrix();
            */
        
        glPushMatrix();
        glTranslatef(mSubLabel->getWidth(), 0, 0);
        mSubLabel->draw();
        glPopMatrix();
        
        
            glPushMatrix();
                glPushMatrix();
                glTranslatef(SESSION_LABEL_META_OFFSET_X, SESSION_LABEL_META_OFFSET_Y, 0);
                glColor3f(0,0,0);
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(2, GL_FLOAT, 0, &mVertexTrapezoidIndex[0]);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glDisableClientState(GL_VERTEX_ARRAY);
                glPopMatrix();

            
                glColor3f(1,1,1);
                glTranslatef(topLeft.x, topLeft.y, 0);
                gl::draw(mTextBox->getTexture());
    #ifdef SESSION_VIEW_LABEL_EVENT_META_DEBUG_DRAW
                mTextBox->debugDraw();
    #endif
            glPopMatrix();
        
        
        glPopMatrix();
    }
    
    void EventMetaLabel::update(){
        
    }
    
    void EventMetaLabel::set(const string& type, const string& index){
        mTextBox->setString(index);
        //mTextBoxSub->setString(type);
        
        //float textBoxSubWidth   = mTextBoxSub->getCalculatedSize().x;
        float textBoxFrontWidth = mTextBox->getCalculatedSize().x;
        
        mTextBoxFrontWidth = textBoxFrontWidth + SESSION_LABEL_EVENT_META_TYPE_INDEX_SPACING;

        float trapezoidIndexWidth = mTextBoxFrontWidth + SESSION_LABEL_META_OFFSET_X * -0.5f;
        //float trapezoidTypeWidth  = textBoxSubWidth + SESSION_LABEL_META_OFFSET_X * -1.5f;
        
        static const float slope = 14.5f;
        
        mVertexTrapezoidIndex[0] = Vec2f(slope,0);
        mVertexTrapezoidIndex[1] = Vec2f(trapezoidIndexWidth + slope,0);
        mVertexTrapezoidIndex[2] = Vec2f(0, SESSION_LABEL_META_BOX_HEIGHT);
        mVertexTrapezoidIndex[3] = Vec2f(trapezoidIndexWidth, SESSION_LABEL_META_BOX_HEIGHT);
        
        mSubLabel->set(type);
        
        
        
        /*
        mVertexTrapezoidType[0] = Vec2f(slope,0);
        mVertexTrapezoidType[1] = Vec2f(trapezoidTypeWidth + slope, 0);
        mVertexTrapezoidType[2] = Vec2f(0, SESSION_LABEL_META_BOX_HEIGHT);
        mVertexTrapezoidType[3] = Vec2f(trapezoidTypeWidth, SESSION_LABEL_META_BOX_HEIGHT);
         */
    }
    
    
    
}