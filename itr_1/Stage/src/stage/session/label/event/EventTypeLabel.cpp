#include "stage/session/label/event/EventTypeLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


namespace next {

    using namespace boost;
    typedef EaseInOutQuad AnimEaseInOut;
    
    
    map<string, TextBoxTexture> EventTypeLabel::sMapTypeTexture;
    
    void EventTypeLabel::Map(map<uint32_t, next::Event> *events){
        Font font(app::loadResource(RES_AKKURAT_BOLD), SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR);
        
        TextBox* textBox = new TextBox();
        textBox->setFont(font);
        textBox->setWidth(     SESSION_LABEL_SESSION_META_BOX_WIDTH);
        textBox->setFontSize(  SESSION_LABEL_META_FONT_SIZE);
        textBox->setColorFont( SESSION_LABEL_SESSION_META_FONT_COLOR);
        
        for(map<uint32_t, next::Event>::const_iterator itr = events->begin(); itr != events->end(); ++itr){
            const string& type = itr->second.type;
            textBox->setString(type);
            
            sMapTypeTexture[type] = TextBoxTexture();
            sMapTypeTexture[type].calculatedSize = textBox->getCalculatedSize();
            sMapTypeTexture[type].texcoords      = textBox->getTexcoords();
            sMapTypeTexture[type].topleft        = textBox->getTopLeft();
            sMapTypeTexture[type].texture        = gl::Texture(Surface(textBox->getTexture()));
        }
        
        delete textBox;
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/

    EventTypeLabel::EventTypeLabel() :
        AbstractLabel(),
        mTextBoxWidth(0),
        mAlphaState(0.0f),
        mPositionState(Vec2f()){}

    /*--------------------------------------------------------------------------------------------*/
    //  Draw
    /*--------------------------------------------------------------------------------------------*/

    void EventTypeLabel::draw(){
        if(mType.empty()){
            return;
        }
        Vec2f offset  = mPositionState();
        Vec2f pos     = mPos + offset;
        
        const TextBoxTexture& type = sMapTypeTexture[mType];
        const Vec2f& topLeft       = type.topleft;
        
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
            gl::draw(type.texture);

        glColor4f(1, 1, 1, 1);
        glPopMatrix();
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Set
    /*--------------------------------------------------------------------------------------------*/

    void EventTypeLabel::set(const string& str){
        mType = str;
        mTextBoxWidth = sMapTypeTexture[mType].calculatedSize.x + SESSION_LABEL_EVENT_META_TYPE_INDEX_SPACING;
        float trapezoidWidth = mTextBoxWidth + SESSION_LABEL_META_OFFSET_X * -1;
        static const float slope = 14.5f;
        
        mVertexTrapezoid[0] = Vec2f(slope,0);
        mVertexTrapezoid[1] = Vec2f(trapezoidWidth + slope,0);
        mVertexTrapezoid[2] = Vec2f(0,SESSION_LABEL_META_BOX_HEIGHT);
        mVertexTrapezoid[3] = Vec2f(trapezoidWidth, SESSION_LABEL_META_BOX_HEIGHT);
    }
    
    float EventTypeLabel::getWidth(){
        return mTextBoxWidth;
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Tigger state
    /*--------------------------------------------------------------------------------------------*/

    void EventTypeLabel::show(){
        const static Vec2f offset(SESSION_LABEL_EVENT_META_ANIM_OFFSET_IN,0);
        const static Vec2f zero;
        
        tween(&mPositionState, offset, zero,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_OFFSET_SHOW,
              AnimEaseInOut());
        
        tween(&mAlphaState, 0.0f, 1.0f,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_ALPHA_SHOW,
              AnimEaseInOut());
    }
    
    void EventTypeLabel::hide(){
        tween(&mAlphaState, 0.0f,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIN_TIME_ALPHA_HIDE,
              AnimEaseInOut());
    }
    
    void EventTypeLabel::on(){
        tween(&mAlphaState, 1.0f,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_ALPHA_ON,
              AnimEaseInOut());
     }
    
    void EventTypeLabel::off(){
        tween(&mAlphaState, 0.0f,
              SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_ALPHA_OFF,
              AnimEaseInOut());
    }
}