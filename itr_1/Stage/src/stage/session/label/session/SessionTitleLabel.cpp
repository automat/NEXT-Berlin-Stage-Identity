#include "SessionTitleLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>


namespace next {
    using namespace boost;
    typedef EaseInOutQuad AnimEaseInOut;

    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/

    SessionTitleLabel::SessionTitleLabel() : AbstractLabel(){
        setPosition(SESSION_LABEL_SESSION_TITLE_POS);
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Draw
    /*--------------------------------------------------------------------------------------------*/

    void SessionTitleLabel::draw(){
        if(mTitle.calculatedSize.x == 0){
            return;
        }
        
        const Vec2f& topLeft       = mTitle.topleft;
        const gl::Texture& texture = mTitle.texture;
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

#ifdef SESSION_VIEW_LABEL_SESSION_TITLE_DEBUG_DRAW
        mTextBox->debugDraw();
#endif
        glPopMatrix();
    }
    
    void SessionTitleLabel::set(const string& title){
        deque<string> tokens;
        split(tokens, title, is_any_of(" "));
        
        for(auto& token : tokens){
            token[0] = toupper(token[0]);
        }
        string _str = algorithm::join(tokens, " ");
        
        
        next::TextBox textbox;
        textbox.setFont(Font(app::loadResource(RES_TRANSCRIPT_BOLD),
                               SESSION_LABEL_SESSION_TITLE_FONT_SIZE * SESSION_LABEL_SESSION_TITLE_FONT_SCALAR));
        
        textbox.setWidth(SESSION_LABEL_SESSION_TITLE_BOX_WIDTH);
        
        textbox.setFontSize(  SESSION_LABEL_SESSION_TITLE_FONT_SIZE);
        textbox.setLineHeight(SESSION_LABEL_SESSION_TITLE_LINE_HEIGHT);
        textbox.setColorFont( SESSION_LABEL_SESSION_TITLE_FONT_COLOR);
        
        textbox.setColorDropShadow( SESSION_LABEL_SESSION_TITLE_SHADOW_COLOR);
        textbox.setDropShadowOffset(SESSION_LABEL_SESSION_TITLE_SHADOW_OFFSET);
        textbox.setDropShadowScale( SESSION_LABEL_SESSION_TITLE_SHADOW_STRENGTH);
        textbox.dropShadow();
        
        textbox.setColorUnderline(SESSION_LABEL_SESSION_TITLE_UNDERLINE_COLOR);
        textbox.setUnderlineHeight(SESSION_LABEL_SESSION_TITLE_UNDERLINE_HEIGHT);
        textbox.underline();
       
        textbox.setString(_str);
        
        
        mTitle.topleft        = textbox.getTopLeft();
        mTitle.texcoords      = textbox.getTexcoords();
        mTitle.calculatedSize = textbox.getCalculatedSize();
        mTitle.texture        = gl::Texture(Surface(textbox.getTexture()));
    
        const Vec2f& textureSize = mTitle.calculatedSize;
        float textureWidth       = textureSize.x;
        float textureHeight      = textureSize.y;
        
        int numLines = textbox.getNumLines();
        vector<vector<Vec2f>> texcoords = textbox.getTexcoords();
        
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

    /*--------------------------------------------------------------------------------------------*/
    //  Tirgger State
    /*--------------------------------------------------------------------------------------------*/

    void SessionTitleLabel::on(){
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
    
    void SessionTitleLabel::off(){
        for (vector<LineQuad>::iterator itr = mLineQuads.begin(); itr != mLineQuads.end(); ++itr) {
            tween(&itr->alphaState, 1.0f, 0.0f,
                  SESSION_LABEL_SESSION_TITLE_ANIM_TIME_ALPHA_OFF,
                  AnimEaseInOut());
        }
    }
    

    
}