#include "SpeakerLabel.h"

#include "EventTitleLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>



namespace next {
    using namespace boost;
    typedef EaseInOutQuad AnimEaseInOut;
    
    SpeakerLabel::SpeakerLabel() :
        AbstractLabel(),
        mTextBoxNameHeight(0){
        mTextBox->setFont(Font(app::loadResource(RES_TRANSCRIPT_BOLD),SESSION_LABEL_SESSION_TITLE_FONT_SIZE * 2));
        
        mTextBox->setWidth(     1500);
        mTextBox->setFontSize(  SESSION_LABEL_EVENT_TITLE_FONT_SIZE);
        mTextBox->setLineHeight(SESSION_LABEL_EVENT_TITLE_LINE_HEIGHT);
            mTextBox->setColorFont( ColorAf::white());
        
        mTextBoxCompany = new TextBox();
        
        mTextBoxCompany->setFont(Font(app::loadResource(RES_AKKURAT_LIGHT),SESSION_LABEL_SESSION_TITLE_FONT_SIZE * 2));
        
        mTextBoxCompany->setWidth(     1500);
        mTextBoxCompany->setFontSize(  SESSION_LABEL_EVENT_TITLE_FONT_SIZE);
        mTextBoxCompany->setLineHeight(SESSION_LABEL_EVENT_TITLE_LINE_HEIGHT);
        mTextBoxCompany->setColorFont( SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR);
           // mTextBoxCompany->setString("Microsoft");
        
        //setPosition(SESSION_LABEL_EVENT_TITLE_POS);
        
       // setString("Regine Haschka-Helmer");
        
        mScale = 1.0f / (SESSION_LABEL_EVENT_TITLE_FONT_SIZE * 8);
            
            set("Christian Deilmann", "Singularity University");
        
        
        
        
        
    }
    
    SpeakerLabel::~SpeakerLabel(){
        delete mTextBoxCompany;
    }
    
    
    void SpeakerLabel::setPosition(const Vec3f &pos){
        mPos = pos.xz();
        mTransform.setToIdentity();
        mTransform *= Matrix44f::createTranslation(Vec3f(0,pos.y,0));
        mTransform *= Matrix44f::createRotation(Vec3f(M_PI_2,M_PI_2,0));
        
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Update / draw
    /*--------------------------------------------------------------------------------------------*/
    
    
    void SpeakerLabel::draw(){
        if(mTextBox->getString().empty()){
            return;
        }
        Vec2f topLeft = mTextBox->getTopLeft();
        const gl::Texture& texture = mTextBox->getTexture();
        Vec2f quadPos;
        
        Vec2f size = mTextBox->getSize();
        
        glPushMatrix();
        glTranslatef(mPos.x, 0 , mPos.y);
        glColor4f(1,1,1,1);
        glMultMatrixf(&mTransform[0]);
        glScalef(mScale, mScale, 1.0f);
        //glTranslatef(topLeft.x, 0, topLeft.y);
        //glScalef(1.0f/size.x, 1.0f, 1.0f/size.y);
        /*
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
         */
        gl::draw(mTextBox->getTexture());
        //mTextBox->debugDraw();
        
        glTranslatef(0, mTextBoxNameHeight, 0);
        gl::draw(mTextBoxCompany->getTexture());
        //mTextBoxCompany->debugDraw();
        
        
        
        glPopMatrix();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Set
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerLabel::set(const string& name, const string& company){
        if(mString == name){
            return;
        }
        
        mTextBox->setString(name);
        mString = name;
        mTextBoxNameHeight = mTextBox->getCalculatedSize().y;
        
        mTextBoxCompany->setString(company);
        
        
        cout << mTextBox->getCalculatedSize().y << endl;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Show / Hide
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerLabel::show(){
        /*
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
         */
    }
    
    void SpeakerLabel::hide(){
        /*
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
         */
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // On / Off
    /*--------------------------------------------------------------------------------------------*/
    
    
    void SpeakerLabel::on(){
        /*
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
         */
        
    }
    
    void SpeakerLabel::off(){
        /*
        for (vector<LineQuad>::iterator itr = mLineQuads.begin(); itr != mLineQuads.end(); ++itr) {
            itr->posState = itr->posTarget;
            
            tween(&itr->alphaState, 1.0f, 0.0f,
                  SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_OFF,
                  AnimEaseInOut());
        }
         */
    }
    
    
    /*--------------------------------------------------------------------------------------------*/
    // Gen Textured Lines
    /*--------------------------------------------------------------------------------------------*/
    /*
    void EventTitleLabel::genQuads(){
        mLineQuads.clear();
        
        Vec2f textureSize   = mTextBox->getCalculatedSize();
        float textureWidth  = textureSize.x;
        float textureHeight = textureSize.y;
        
        int   numLines = mTextBox->getNumLines();
        vector<vector<Vec2f>> texcoords = mTextBox->getTexcoords();
        
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
     */
    
    
}