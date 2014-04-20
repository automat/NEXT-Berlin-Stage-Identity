#include "EventMetaLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


namespace next {
    using namespace boost;
    EventMetaLabel::EventMetaLabel() : AbstractLabel(){
        mTextBox->setFont(Font(app::loadResource(RES_AKKURAT_BOLD),
                               SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR));
        
        mTextBox->setWidth(     SESSION_LABEL_META_BOX_WIDTH);
        mTextBox->setFontSize(  SESSION_LABEL_META_FONT_SIZE);
        mTextBox->setColorFont( SESSION_LABEL_EVENT_META_FONT_COLOR);
        
        mTextBoxSub = new TextBox();
        mTextBoxSub->setFont(Font(app::loadResource(RES_AKKURAT_LIGHT),
                                  SESSION_LABEL_META_FONT_SIZE * SESSION_LABEL_META_FONT_SCALAR));
        mTextBoxSub->setWidth(200);
        mTextBoxSub->setFontSize(SESSION_LABEL_META_FONT_SIZE);
        mTextBoxSub->setColorFont(SESSION_LABEL_EVENT_META_FONT_COLOR);

        setPosition(SESSION_LABEL_EVENT_META_POS);
    }
    
    EventMetaLabel::~EventMetaLabel(){
        delete mTextBoxSub;
    }
    
    void EventMetaLabel::draw(){
        if(mTextBox->getString().empty() ||
           mTextBoxSub->getString().empty()){
            return;
        }
        
        Vec2f topLeft = mTextBox->getTopLeft();
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, 0);
        glColor4f(1,1,1,1);
        glTranslatef(topLeft.x, topLeft.y, 0);
        gl::draw(mTextBox->getTexture());
        glTranslatef(mTextBoxWidth, 0, 0);
        gl::draw(mTextBoxSub->getTexture());
        glPopMatrix();
    }
    
    void EventMetaLabel::update(){
        
    }
    
    void EventMetaLabel::setType(const string& str){
        mTextBox->setString(str);
        mTextBoxWidth = mTextBox->getCalculatedSize().x + SESSION_LABEL_EVENT_META_TYPE_INDEX_SPACING;
    }
    
    void EventMetaLabel::setIndex(const string& str){
        mTextBoxSub->setString(str);
    }
    
    
    
    
}