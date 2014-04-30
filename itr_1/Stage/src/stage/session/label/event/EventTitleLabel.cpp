#include "EventTitleLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>



namespace next {
    using namespace boost;
    typedef EaseInOutQuad AnimEaseInOut;
    
    map<string, TextBoxTexture>   EventTitleLabel::sMapTitleTexture;
    map<string, vector<LineQuad>> EventTitleLabel::sMapTitleLineQuads;
    
    void EventTitleLabel::Map(map<uint32_t, next::Event> *events){
        Font fontDefault(app::loadResource(RES_TRANSCRIPT_BOLD),
                         SESSION_LABEL_SESSION_TITLE_FONT_SIZE * SESSION_LABEL_EVENT_TITLE_FONT_SCALAR);
        Font fontExceed( app::loadResource(RES_TRANSCRIPT_BOLD),
                         SESSION_LABEL_EVENT_TITLE_EXCEED_FONT_SIZE * SESSION_LABEL_EVENT_TITLE_FONT_SCALAR);
        
        string supportedChars = gl::TextureFont::defaultChars() + "–";
        
        TextBox textBoxDefault;
        
        textBoxDefault.setFont(fontDefault, supportedChars);
        textBoxDefault.setWidth(SESSION_LABEL_EVENT_TITLE_DEFAULT_BOX_WIDTH);
        textBoxDefault.setFontSize(  SESSION_LABEL_EVENT_TITLE_FONT_SIZE);
        textBoxDefault.setLineHeight(SESSION_LABEL_EVENT_TITLE_LINE_HEIGHT);
        textBoxDefault.setColorFont( SESSION_LABEL_EVENT_TITLE_FONT_COLOR);
        
        textBoxDefault.setColorDropShadow( SESSION_LABEL_EVENT_TITLE_SHADOW_COLOR);
        textBoxDefault.setDropShadowOffset(SESSION_LABEL_EVENT_TITLE_SHADOW_OFFSET);
        textBoxDefault.setDropShadowScale( SESSION_LABEL_EVENT_TITLE_SHADOW_STRENGTH);
        textBoxDefault.dropShadow();
        
        textBoxDefault.gradientUnderline();
        textBoxDefault.setColorGradientUnderline(SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_START,
                                            SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_END);
        textBoxDefault.setUnderlineHeight(SESSION_LABEL_EVENT_TITLE_UNDERLINE_HEIGHT);
        textBoxDefault.underline();
        
        TextBox textBoxExceed;
        textBoxExceed.setFont(fontExceed, supportedChars);
        
        textBoxExceed.setWidth(     SESSION_LABEL_EVENT_TITLE_EXCEED_BOX_WIDTH);
        textBoxExceed.setFontSize(  SESSION_LABEL_EVENT_TITLE_EXCEED_FONT_SIZE);
        textBoxExceed.setLineHeight(SESSION_LABEL_EVENT_TITLE_EXCEED_LINE_HEIGHT);
        textBoxExceed.setColorFont( SESSION_LABEL_EVENT_TITLE_FONT_COLOR);
        
        textBoxExceed.setColorDropShadow( SESSION_LABEL_EVENT_TITLE_SHADOW_COLOR);
        textBoxExceed.setDropShadowOffset(SESSION_LABEL_EVENT_TITLE_SHADOW_OFFSET);
        textBoxExceed.setDropShadowScale( SESSION_LABEL_EVENT_TITLE_SHADOW_STRENGTH);
        textBoxExceed.dropShadow();
        
        textBoxExceed.gradientUnderline();
        textBoxExceed.setColorGradientUnderline(SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_START,
                                                  SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_END);
        textBoxExceed.setUnderlineHeight(SESSION_LABEL_EVENT_TITLE_UNDERLINE_HEIGHT);
        textBoxExceed.underline();

        
        vector<LineQuad> lineQuads;
        Vec2f textureSize;
        float textureWidth;
        float textureHeight;
        int   numLines;
        Vec2f topLeft;
        float width, height;
       
        int i;
        
        for(map<uint32_t, next::Event>::const_iterator itr = events->begin(); itr != events->end(); itr++){
            const string& name = itr->second.title;
            
            //
            //  Gen texture
            //
            
            textBoxDefault.setString(name);
            numLines = textBoxDefault.getNumLines();
            if(textBoxDefault.getNumLines() > SESSION_LABEL_EVENT_TITLE_MAX_LINES){
                textBoxExceed.setString(name);
                sMapTitleTexture[name] = TextBoxTexture();
                sMapTitleTexture[name].calculatedSize = textBoxExceed.getCalculatedSize();
                sMapTitleTexture[name].texcoords      = textBoxExceed.getTexcoords();
                sMapTitleTexture[name].topleft        = textBoxExceed.getTopLeft();
                sMapTitleTexture[name].texture        = gl::Texture(Surface(textBoxExceed.getTexture()));
                numLines = textBoxExceed.getNumLines();
            } else {
                sMapTitleTexture[name] = TextBoxTexture();
                sMapTitleTexture[name].calculatedSize = textBoxDefault.getCalculatedSize();
                sMapTitleTexture[name].texcoords      = textBoxDefault.getTexcoords();
                sMapTitleTexture[name].topleft        = textBoxDefault.getTopLeft();
                sMapTitleTexture[name].texture        = gl::Texture(Surface(textBoxDefault.getTexture()));
            }
            
            //
            //  Gen quads
            //

            lineQuads.clear();
            
            textureSize   = sMapTitleTexture[name].calculatedSize;
            textureWidth  = textureSize.x;
            textureHeight = textureSize.y;
            
            const vector<vector<Vec2f>>& texcoords = sMapTitleTexture[name].texcoords;
            
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
                
                lineQuads.push_back(quad);
            }
            
            sMapTitleLineQuads[name] = lineQuads;
        }
    }
    
    

    EventTitleLabel::EventTitleLabel() : AbstractLabel(){
        setPosition(SESSION_LABEL_EVENT_TITLE_POS);
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Update / draw
    /*--------------------------------------------------------------------------------------------*/
    
    
    void EventTitleLabel::draw(){
        if(mKeyTitle.empty()){
            return;
        }
        
        const TextBoxTexture&   title = sMapTitleTexture[mKeyTitle];
        const vector<LineQuad>& quads = sMapTitleLineQuads[mKeyTitle];
        
        Vec2f topLeft = title.topleft;
        const gl::Texture& texture = title.texture;
        Vec2f quadPos;
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, 0);
        glColor4f(1,1,1,1);
        glTranslatef(topLeft.x, topLeft.y, 0);
        
        texture.enableAndBind();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        for (vector<LineQuad>::const_iterator itr = quads.begin(); itr != quads.end(); ++itr) {
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
        mKeyTitle = title;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Show / Hide
    /*--------------------------------------------------------------------------------------------*/
    
    void EventTitleLabel::show(){
        vector<LineQuad>& quads = sMapTitleLineQuads[mKeyTitle];
        
        float size = static_cast<float>(MAX(1, quads.size() - 1));
        float index = 0;
        float offset;
        Vec2f offsetPos;
        
        for (vector<LineQuad>::iterator itr = quads.begin(); itr != quads.end(); ++itr) {
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
        vector<LineQuad>& quads = sMapTitleLineQuads[mKeyTitle];
        
        float size = static_cast<float>(MAX(1, quads.size() - 1));
        float index = 0;
        float offset;
        Vec2f offsetPos;
        
        for (vector<LineQuad>::iterator itr = quads.begin(); itr != quads.end(); ++itr) {
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
        vector<LineQuad>& quads = sMapTitleLineQuads[mKeyTitle];
        
        float size = static_cast<float>(MAX(1, quads.size() - 1));
        float index = 0;
        float offset;
        Vec2f offsetPos;
        
        for (vector<LineQuad>::iterator itr = quads.begin(); itr != quads.end(); ++itr) {
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
         vector<LineQuad>& quads = sMapTitleLineQuads[mKeyTitle];
        
        for (vector<LineQuad>::iterator itr = quads.begin(); itr != quads.end(); ++itr) {
            itr->posState = itr->posTarget;
            
            tween(&itr->alphaState, 1.0f, 0.0f,
                  SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_OFF,
                  AnimEaseInOut());
        }
    }
}