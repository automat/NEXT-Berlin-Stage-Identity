#include "SessionTitleLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


namespace next {
    using namespace boost;
    SessionTitleLabel::SessionTitleLabel() : AbstractLabel(){
        mTextBox->setFont(Font(app::loadResource(RES_TRANSCRIPT_BOLD),
                               SESSION_LABEL_SESSION_TITLE_FONT_SIZE * SESSION_LABEL_SESSION_TITLE_FONT_SCALAR));
        
        mTextBox->setWidth(SESSION_LABEL_SESSION_TITLE_BOX_WIDTH);
        
        mTextBox->setFontSize( SESSION_LABEL_SESSION_TITLE_FONT_SIZE);
        mTextBox->setLineHeight(SESSION_LABEL_SESSION_TITLE_LINE_HEIGHT);
        mTextBox->setColorFont( SESSION_LABEL_SESSION_TITLE_FONT_COLOR);
        
        mTextBox->setColorDropShadow( SESSION_LABEL_SESSION_TITLE_SHADOW_COLOR);
        mTextBox->setDropShadowOffset(SESSION_LABEL_SESSION_TITLE_SHADOW_OFFSET);
        mTextBox->setDropShadowScale( SESSION_LABEL_SESSION_TITLE_SHADOW_STRENGTH);
        mTextBox->dropShadow();
        
        mTextBox->setColorUnderline(SESSION_LABEL_SESSION_TITLE_UNDERLINE_COLOR);
        mTextBox->setUnderlineHeight(SESSION_LABEL_SESSION_TITLE_UNDERLINE_HEIGHT);
        mTextBox->underline();
        
        setPosition(SESSION_LABEL_SESSION_TITLE_POS);
    }
    
    void SessionTitleLabel::draw(){
        if(mTextBox->getString().empty()){
            return;
        }
        
        Vec2f topLeft = mTextBox->getTopLeft();
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, 0);
        glColor3f(1,1,1);
        glTranslatef(topLeft.x, topLeft.y, 0);
        gl::draw(mTextBox->getTexture());
        glPopMatrix();
    }
    
    void SessionTitleLabel::setString(const string& str){
        deque<string> tokens;
        split(tokens, str, is_any_of(" "));
        
        for(auto& token : tokens){
            token[0] = toupper(token[0]);
        }
        string _str = algorithm::join(tokens, " ");
        AbstractLabel::setString(_str);
    }
    
    void SessionTitleLabel::update(){
        
    }
    
    
}