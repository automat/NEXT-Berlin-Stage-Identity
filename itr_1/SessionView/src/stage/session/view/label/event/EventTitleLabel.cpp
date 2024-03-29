#include "EventTitleLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>



namespace next {
    using namespace boost;
    typedef EaseInOutQuad AnimEaseInOut;

    Font EventTitleLabel::sFontDefault;
    Font EventTitleLabel::sFontExceed;

    EventTitleLabel::EventTitleLabel() : AbstractLabel(), mDidExceedNumLineMax(false){
        static bool __fontsInitialized(false);
        if(!__fontsInitialized){
            sFontDefault = Font(app::loadResource(RES_TRANSCRIPT_BOLD),
                                SESSION_LABEL_SESSION_TITLE_FONT_SIZE * SESSION_LABEL_EVENT_TITLE_FONT_SCALAR);
            sFontExceed  = Font(app::loadResource(RES_TRANSCRIPT_BOLD),
                                SESSION_LABEL_EVENT_TITLE_EXCEED_FONT_SIZE * SESSION_LABEL_EVENT_TITLE_FONT_SCALAR);
        }

        string supportedChars = gl::TextureFont::defaultChars() + "–";
        
        //
        //  Default
        //
        mTextBox->setFont(sFontDefault, supportedChars);
        
        mTextBox->setWidth(SESSION_LABEL_EVENT_TITLE_DEFAULT_BOX_WIDTH);
        mTextBox->setFontSize(  SESSION_LABEL_EVENT_TITLE_FONT_SIZE);
        mTextBox->setLineHeight(SESSION_LABEL_EVENT_TITLE_LINE_HEIGHT);
        mTextBox->setColorFont( SESSION_LABEL_EVENT_TITLE_FONT_COLOR);
        
        mTextBox->setColorDropShadow( SESSION_LABEL_EVENT_TITLE_SHADOW_COLOR);
        mTextBox->setDropShadowOffset(SESSION_LABEL_EVENT_TITLE_SHADOW_OFFSET);
        mTextBox->setDropShadowScale( SESSION_LABEL_EVENT_TITLE_SHADOW_STRENGTH);
        mTextBox->dropShadow();
        
        //mTextBox->setColorUnderline( SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR);
        mTextBox->gradientUnderline();
        mTextBox->setColorGradientUnderline(SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_START,
                                            SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_END);
        mTextBox->setUnderlineHeight(SESSION_LABEL_EVENT_TITLE_UNDERLINE_HEIGHT);
        mTextBox->underline();
        
        //
        //  Exceeded
        //
        mTextBoxExceed = new TextBox();
        mTextBoxExceed->setFont(sFontExceed, supportedChars);
        
        mTextBoxExceed->setWidth(     SESSION_LABEL_EVENT_TITLE_EXCEED_BOX_WIDTH);
        mTextBoxExceed->setFontSize(  SESSION_LABEL_EVENT_TITLE_EXCEED_FONT_SIZE);
        mTextBoxExceed->setLineHeight(SESSION_LABEL_EVENT_TITLE_EXCEED_LINE_HEIGHT);
        mTextBoxExceed->setColorFont( SESSION_LABEL_EVENT_TITLE_FONT_COLOR);
        
        mTextBoxExceed->setColorDropShadow( SESSION_LABEL_EVENT_TITLE_SHADOW_COLOR);
        mTextBoxExceed->setDropShadowOffset(SESSION_LABEL_EVENT_TITLE_SHADOW_OFFSET);
        mTextBoxExceed->setDropShadowScale( SESSION_LABEL_EVENT_TITLE_SHADOW_STRENGTH);
        mTextBoxExceed->dropShadow();
        
        //mTextBoxExceed->setColorUnderline( SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR);
        mTextBoxExceed->gradientUnderline();
        mTextBoxExceed->setColorGradientUnderline(SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_START,
                                                  SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_END);
        mTextBoxExceed->setUnderlineHeight(SESSION_LABEL_EVENT_TITLE_UNDERLINE_HEIGHT);
        mTextBoxExceed->underline();
        
        setPosition(SESSION_LABEL_EVENT_TITLE_POS);
    }
    
    EventTitleLabel::~EventTitleLabel(){
        delete mTextBoxExceed;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Update / draw
    /*--------------------------------------------------------------------------------------------*/
    
    
    void EventTitleLabel::draw(){
        if(mTextBox->empty()){
            return;
        }
        Vec2f topLeft = mTextBox->getTopLeft();
        const gl::Texture& texture = mTextBox->getTexture();
        Vec2f quadPos;
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, 0);
        glColor4f(1,1,1,1);
        glTranslatef(topLeft.x, topLeft.y, 0);
        
        texture.enableAndBind();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        for (vector<LineQuad>::const_iterator itr = mLineQuads.begin(); itr != mLineQuads.end(); ++itr) {
            quadPos = itr->posState();
            
            glPushMatrix();
            glColor4f(1,1,1, itr->alphaState());
                glTranslatef(quadPos.x, quadPos.y, 0);
                glTexCoordPointer(2, GL_FLOAT, 0, &itr->texcoords[0]);
                glVertexPointer(2, GL_FLOAT, 0, &itr->vertices[0]);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glPopMatrix();
        }
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        texture.unbind();
        texture.disable();

#ifdef SESSION_VIEW_LABEL_EVENT_TITLE_DEBUG_DRAW
        mTextBox->debugDraw();
#endif
        
        glPopMatrix();
    }

    /*--------------------------------------------------------------------------------------------*/
    // Set
    /*--------------------------------------------------------------------------------------------*/
    
    void EventTitleLabel::set(const string& title){
        if(mString == title){
            return;
        }
        
        if(mDidExceedNumLineMax){
            mDidExceedNumLineMax = false;
        }
        
        mTextBox->setString(title);
        mString = title;
        
        
        if(mTextBox->getNumLines() > SESSION_LABEL_EVENT_TITLE_MAX_LINES && !mDidExceedNumLineMax){
            mTextBoxExceed->setString(title);
            mDidExceedNumLineMax = true;
        }

        genQuads();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Show / Hide
    /*--------------------------------------------------------------------------------------------*/
    
    void EventTitleLabel::show(){
        float size = static_cast<float>(MAX(1, mLineQuads.size() - 1));
        float index = 0;
        float offset;
        Vec2f offsetPos;
        
        for (vector<LineQuad>::iterator itr = mLineQuads.begin(); itr != mLineQuads.end(); ++itr) {
            offset      = 1.0f + index++ / size;
            offsetPos.x = offset * SESSION_LABEL_EVENT_TITLE_ANIM_OFFSET_IN;
            
            tween(&itr->posState, itr->posTarget - offsetPos,  itr->posTarget,
                  SESSION_LABEL_EVENT_TITLE_ANIM_TIME_OFFSET_IN,
                  AnimEaseInOut());
            
            tween(&itr->alphaState, 0.0f, 1.0f,
                  SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_IN,
                  AnimEaseInOut());
         }
    }
    
    void EventTitleLabel::hide(){
        float size = static_cast<float>(MAX(1, mLineQuads.size() - 1));
        float index = 0;
        float offset;
        Vec2f offsetPos;
        
        for (vector<LineQuad>::iterator itr = mLineQuads.begin(); itr != mLineQuads.end(); ++itr) {
            offset      = 1.0f + index++ / size;
            offsetPos.x = offset * SESSION_LABEL_EVENT_TITLE_ANIM_OFFSET_OUT;
            
            tween(&itr->posState, itr->posTarget, itr->posTarget + offsetPos,
                  SESSION_LABEL_EVENT_TITLE_ANIM_TIME_OFFSET_OUT,
                  AnimEaseInOut());
            
            tween(&itr->alphaState, 1.0f, 0.0f,
                  SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_OUT,
                  AnimEaseInOut());
        }
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // On / Off
    /*--------------------------------------------------------------------------------------------*/
    
    
    void EventTitleLabel::on(){
        float size = static_cast<float>(MAX(1, mLineQuads.size() - 1));
        float index = 0;
        float offset;
        Vec2f offsetPos;
        
        
        for (vector<LineQuad>::iterator itr = mLineQuads.begin(); itr != mLineQuads.end(); ++itr) {
            offset      = 1.0f + index++ / size;
            offsetPos.x = offset * SESSION_LABEL_EVENT_TITLE_ANIM_OFFSET_IN;
            
            tween(&itr->posState, itr->posTarget - offsetPos,  itr->posTarget,
                  SESSION_LABEL_EVENT_TITLE_ANIM_TIME_OFFSET_IN,
                  AnimEaseInOut());
            
            tween(&itr->alphaState, 0.0f, 1.0f,
                  SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_ON,
                  AnimEaseInOut());
        }
        
    }
    
    void EventTitleLabel::off(){
        for (vector<LineQuad>::iterator itr = mLineQuads.begin(); itr != mLineQuads.end(); ++itr) {
            itr->posState = itr->posTarget;
            
            tween(&itr->alphaState, 1.0f, 0.0f,
                  SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_OFF,
                  AnimEaseInOut());
        }
    }
    
    
    /*--------------------------------------------------------------------------------------------*/
    // Gen Textured Lines
    /*--------------------------------------------------------------------------------------------*/
    
    void EventTitleLabel::genQuads(){
        mLineQuads.clear();
        
        
        TextBox* textBox = mDidExceedNumLineMax ? mTextBoxExceed : mTextBox;
        
        Vec2f textureSize   = textBox->getCalculatedSize();
        float textureWidth  = textureSize.x;
        float textureHeight = textureSize.y;
        
        int   numLines = textBox->getNumLines();
        vector<vector<Vec2f>> texcoords = textBox->getTexcoords();
        
        Vec2f topLeft;
        float width, height;
        
        int i;
        i = -1;
        while(++i < numLines){
            const vector<Vec2f>& _texcoords = texcoords[i];
            
            LineQuad quad;
            quad.posTarget = topLeft = Vec2f(_texcoords[0].x * textureWidth, _texcoords[0].y * textureHeight);;
            quad.posState  = quad.posTarget;
            quad.texcoords = _texcoords;
            
            vector<Vec2f>& vertices = quad.vertices;
            width  = topLeft.x + _texcoords[1].x * textureWidth;
            height = _texcoords[2].y * textureHeight - topLeft.y;
            
            vertices.push_back(Vec2f());
            vertices.push_back(Vec2f(width,0));
            vertices.push_back(Vec2f(0,height));
            vertices.push_back(Vec2f(width,height));
            
            mLineQuads.push_back(quad);
        }
    }
    
    
}