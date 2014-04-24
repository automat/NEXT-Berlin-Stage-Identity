#include "SessionMetaLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <ctime>


namespace next {
    using namespace boost;
    typedef EaseInOutQuad AnimEaseInOut;

    /*--------------------------------------------------------------------------------------------*/
    //  Contructor / Destructor
    /*--------------------------------------------------------------------------------------------*/

    SessionMetaLabel::SessionMetaLabel() :
        AbstractLabel(),
        mAlphaState(0),
        mTextBoxTimeWidth(0),
        mTargetTimestamp(0),
        mReachedTargetTimestamp(false){
            mTextBox->setFont(      Font(app::loadResource(RES_AKKURAT_BOLD), SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR));
            mTextBox->setWidth(     SESSION_LABEL_SESSION_META_BOX_WIDTH);
            mTextBox->setFontSize(  SESSION_LABEL_META_FONT_SIZE);
            mTextBox->setColorFont( SESSION_LABEL_SESSION_META_FONT_COLOR);
            
            mTextBoxTimeRemaining = new TextBox();
            mTextBoxTimeRemaining->setFont(      Font(app::loadResource(RES_AKKURAT_REGULAR), SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR));
            mTextBoxTimeRemaining->setWidth(     SESSION_LABEL_META_SUB_BOX_WIDTH);
            mTextBoxTimeRemaining->setFontSize(  SESSION_LABEL_META_FONT_SIZE);
            mTextBoxTimeRemaining->setColorFont( SESSION_LABEL_SESSION_META_FONT_COLOR);
            
            setPosition(SESSION_LABEL_SESSION_META_POS);
    }

    SessionMetaLabel::~SessionMetaLabel() {
        delete mTextBoxTimeRemaining;
    }

    /*--------------------------------------------------------------------------------------------*/
    // Draw
    /*--------------------------------------------------------------------------------------------*/

    void SessionMetaLabel::draw(){
        if(mTextBox->getString().empty()){
            return;
        }
        
        Vec2f topLeft = mTextBox->getTopLeft();
        float alpha   = mAlphaState();
        
        glPushMatrix();
            glTranslatef(mPos.x, mPos.y, 0);
            
            glPushMatrix();
                glTranslatef(SESSION_LABEL_META_OFFSET_X, SESSION_LABEL_META_OFFSET_Y, 0);
                glColor4f(0,0,0,alpha);
                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(2, GL_FLOAT, 0, &mVertexTrapezoid[0]);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();
        
            glPushMatrix();
                glTranslatef(0, SESSION_LABEL_META_CLOCK_MARGIN_TOP, 0);
                    ColorAf clockColor = COLOR_NEXT_FUCHSIA;
                            clockColor.a = alpha;
                    glColor4f(clockColor.r,clockColor.g,clockColor.b,clockColor.a);
                gl::draw(mClockImageRef);
            glPopMatrix();
        
            glPushMatrix();
            
                glTranslatef(SESSION_LABEL_META_CLOCK_MARGIN_RIGHT,0,0);
                
                glColor4f(1, 1, 1, alpha);
                glTranslatef(topLeft.x, topLeft.y, 0);
                gl::draw(mTextBox->getTexture());
#ifdef SESSION_VIEW_LABEL_SESSION_META_DEBUG_DRAW
                mTextBox->debugDraw();
#endif
                glColor4f(1,1,1,alpha);
                glTranslatef(mTextBoxTimeWidth, 0, 0);
                gl::draw(mTextBoxTimeRemaining->getTexture());
#ifdef SESSION_VIEW_LABEL_SESSION_META_DEBUG_DRAW
                mTextBoxTimeRemaining->debugDraw();
#endif
                glColor4f(1, 1, 1, 1);
            glPopMatrix();
        glPopMatrix();
    }
    
    void SessionMetaLabel::set(const string& timeStart, const string& endTime, time_t timestamp, const gl::Texture& clockImageRef){
        mTextBox->setString(timeStart + " - " + endTime);
        
        mTargetTimestamp = timestamp;
        mClockImageRef   = clockImageRef.weakClone();
        
        mTextBoxTimeWidth = mTextBox->getCalculatedSize().x + SESSION_LABEL_EVENT_META_TYPE_INDEX_SPACING;
    }
    
    void SessionMetaLabel::update(){
        if(mReachedTargetTimestamp){
            return;
        }
        
        time_t timestamp  = time(0);
        time_t diffTarget = mTargetTimestamp - timestamp;
       

        void (^updateTimeRemainingLabel)(const string& string) = ^(const string& string) {
            mTextBoxTimeRemaining->setString(string);
            updateTrapezoid();
        };
        
        if(diffTarget > 0){
            string timeFormat;
            string timeSuffix;
            
            if (diffTarget > 60){
                timeFormat = diffTarget > 5400 /*90 min*/ ? "90+" : toString(static_cast<int>(round(static_cast<float>(diffTarget) / 60.0f)));
                timeSuffix = "min";
                
                if ((diffTarget % 30) == 0) { //just update every 30 seconds
                    updateTimeRemainingLabel("in " + timeFormat + " " + timeSuffix);
                }
            } else {
                timeFormat = toString(diffTarget);
                timeSuffix = "sec";
                updateTimeRemainingLabel("in " + timeFormat + " " + timeSuffix);
            }
        } else {
            updateTimeRemainingLabel("Now");
            mReachedTargetTimestamp = true;
        }
        
    }
    
    void SessionMetaLabel::updateTrapezoid(){
        float textBoxesWidth = mTextBoxTimeWidth + mTextBoxTimeRemaining->getCalculatedSize().x;
        float trapezoidWidth = SESSION_LABEL_META_OFFSET_X * -2.0f + textBoxesWidth + SESSION_LABEL_META_CLOCK_MARGIN_RIGHT;
        
        static const float slope = 14.5f;
        
        mVertexTrapezoid[0] = Vec2f(slope,0);
        mVertexTrapezoid[1] = Vec2f(trapezoidWidth + slope,0);
        mVertexTrapezoid[2] = Vec2f(0,SESSION_LABEL_META_BOX_HEIGHT);
        mVertexTrapezoid[3] = Vec2f(trapezoidWidth, SESSION_LABEL_META_BOX_HEIGHT);
    }

    /*--------------------------------------------------------------------------------------------*/
    // Trigger state
    /*--------------------------------------------------------------------------------------------*/

    void SessionMetaLabel::on(){
        tween(&mAlphaState, 0.0f, 1.0f,
              SESSION_LABEL_SESSION_META_TIME_ALPHA_ON,
              AnimEaseInOut());
    }
    
    void SessionMetaLabel::off(){
        tween(&mAlphaState, 1.0f, 0.0f,
              SESSION_LABEL_SESSION_META_TIME_ALPHA_OFF,
              AnimEaseInOut());
    }
}