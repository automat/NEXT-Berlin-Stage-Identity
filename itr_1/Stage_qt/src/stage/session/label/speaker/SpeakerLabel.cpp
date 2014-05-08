#include "stage/session/label/speaker/SpeakerLabel.h"
#include "stage/session/label/event/EventTitleLabel.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>



namespace next {
    using namespace boost;
    typedef EaseInOutQuad AnimEaseInOut;

    /*--------------------------------------------------------------------------------------------*/
    //  Shared
    /*--------------------------------------------------------------------------------------------*/

    map<string, TextBoxTexture> SpeakerLabel::sMapNameTexture;
    map<string, TextBoxTexture> SpeakerLabel::sMapCompanyTexture;
    
    void SpeakerLabel::Map(map<uint32_t,next::Speaker>* speakers){
        Font fontName(   app::loadResource(RES_AKKURAT_BOLD),  SESSION_LABEL_SPEAKER_FONT_SIZE * SESSION_LABEL_SPEAKER_FONT_SCALE);
        Font fontCompany(app::loadResource(RES_AKKURAT_LIGHT), SESSION_LABEL_SPEAKER_FONT_SIZE * SESSION_LABEL_SPEAKER_FONT_SCALE);
        
        TextBox textBoxName;
        TextBox textBoxCompany;
        
        string supportedChars = gl::TextureFont::defaultChars() + "åü";
        
        textBoxName.setFont(      fontName, supportedChars);
        textBoxName.setWidth(     SESSION_LABEL_SPEAKER_BOX_WIDTH);
        textBoxName.setFontSize(  SESSION_LABEL_SPEAKER_FONT_SIZE);
        textBoxName.setColorFont( SESSION_LABEL_SPEAKER_NAME_FONT_COLOR);
        
        textBoxCompany.setFont(      fontCompany, supportedChars);
        textBoxCompany.setWidth(     SESSION_LABEL_SPEAKER_BOX_WIDTH);
        textBoxCompany.setFontSize(  SESSION_LABEL_SPEAKER_FONT_SIZE);
        textBoxCompany.setColorFont( SESSION_LABEL_SPEAKER_COMPANY_FONT_COLOR);
        textBoxCompany.setLineHeight(SESSION_LABEL_SPEAKER_COMPANY_LINE_HEIGHT);
        
        
        for(map<uint32_t, next::Speaker>::const_iterator itr = speakers->begin(); itr != speakers->end(); ++itr){
            const string& name    = itr->second.name;
            const string& company = itr->second.companyName;
            
            textBoxName.setString(name);
            
            sMapNameTexture[name] = TextBoxTexture();
            sMapNameTexture[name].calculatedSize = textBoxName.getCalculatedSize();
            sMapNameTexture[name].texcoords      = textBoxName.getTexcoords();
            sMapNameTexture[name].topleft        = textBoxName.getTopLeft();
            sMapNameTexture[name].texture        = gl::Texture(Surface(textBoxName.getTexture()));
            
            textBoxCompany.setString(company);
            sMapCompanyTexture[company] = TextBoxTexture();
            sMapCompanyTexture[company].calculatedSize = textBoxCompany.getCalculatedSize();
            sMapCompanyTexture[company].texcoords      = textBoxCompany.getTexcoords();
            sMapCompanyTexture[company].topleft        = textBoxCompany.getTopLeft();
            sMapCompanyTexture[company].texture        = gl::Texture(Surface(textBoxCompany.getTexture()));
        }
    }
   
    /*--------------------------------------------------------------------------------------------*/
    //  Constructor / Destructor
    /*--------------------------------------------------------------------------------------------*/

    
    SpeakerLabel::SpeakerLabel() :
        AbstractLabel(),
        mTextBoxCompanyOffsetY(0),
        mOffsetStateName(0),
        mOffsetStateCompany(0),
        mKeyName(""),
        mKeyCompany(""){
            mScale = 1.0f / (SESSION_LABEL_EVENT_TITLE_FONT_SIZE * 6);
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Set pos override
    /*--------------------------------------------------------------------------------------------*/

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
        if(mKeyName.empty()){
            return;
        }
 
        const TextBoxTexture& name = sMapNameTexture[mKeyName];
        float alphaName  = mAlphaStateName();
        float offsetName = mOffsetStateName();
        
        Vec2f pos = mPos + name.topleft;
        
        glPushMatrix();
        glTranslatef(pos.x, 0 , pos.y);
        
            glMultMatrixf(&mTransform[0]);
            glScalef(mScale, mScale, 1.0f);
            
            glColor4f(1,1,1, alphaName);
            glPushMatrix();
                glTranslatef(offsetName,0,0);
                gl::draw(name.texture);
            glPopMatrix();
        
            if(!mKeyCompany.empty()){
                const TextBoxTexture& company = sMapCompanyTexture[mKeyCompany];
                
                float alphaCompany         = mAlphaStateCompany();
                float offsetCompany        = mOffsetStateCompany();
                float offsetCompanyTexture = name.calculatedSize.y * SESSION_LABEL_SPEAKER_LINE_HEIGHT;
                
                glPushMatrix();
                    glTranslatef(0, offsetCompanyTexture, 0);
                    glColor4f(1,1,1, alphaCompany);
                    glTranslatef(offsetCompany,0,0);
                    gl::draw(company.texture);
                glPopMatrix();
            }
            glColor4f(1,1,1,1);
        
        glPopMatrix();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Set
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerLabel::set(const string& name, const string& company){
        mKeyName    = name;
        mKeyCompany = company;
        
        mAlphaStateName     = 0.0f;
        mAlphaStateCompany  = 0.0f;
        mOffsetStateName    = 0.0f;
        mOffsetStateCompany = 0.0f;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Show / Hide
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerLabel::show(){
        tween(&mAlphaStateName,    0.0f, 1.0f, 0.75f, AnimEaseInOut());
        tween(&mAlphaStateCompany, 0.0f, 1.0f, 0.75f, AnimEaseInOut());
        tween(&mOffsetStateName,   -100.0f, 0.0f, 1.0f, AnimEaseInOut());
        tween(&mOffsetStateCompany, 100.0f, 0.0f, 1.0f, AnimEaseInOut());
    }
    
    void SpeakerLabel::hide(){
        tween(&mAlphaStateName,    0.0f, 0.25f, AnimEaseInOut());
        tween(&mAlphaStateCompany, 0.0f, 0.25f, AnimEaseInOut());
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // On / Off
    /*--------------------------------------------------------------------------------------------*/

    void SpeakerLabel::on(){
        tween(&mAlphaStateName,    0.0f, 1.0f, 2.0f, AnimEaseInOut());
        tween(&mAlphaStateCompany, 0.0f, 1.0f, 2.5f, AnimEaseInOut());
    }
    
    void SpeakerLabel::off(){
        tween(&mAlphaStateName,    0.0f, 1.0f, AnimEaseInOut());
        tween(&mAlphaStateCompany, 0.0f, 1.0f, AnimEaseInOut());
    }
}