#include "SpeakerLabel.h"

#include "EventTitleLabel.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>



namespace next {
    using namespace boost;
    typedef EaseInOutQuad AnimEaseInOut;

    Font SpeakerLabel::sFontName;
    Font SpeakerLabel::sFontCompany;
    
    SpeakerLabel::SpeakerLabel() :
        AbstractLabel(),
        mTextBoxCompanyOffsetY(0){
            static bool __fontInitialized(false);
            if(!__fontInitialized){
                sFontName    = Font(app::loadResource(RES_AKKURAT_BOLD),  SESSION_LABEL_SPEAKER_FONT_SIZE * SESSION_LABEL_SPEAKER_FONT_SCALE);
                sFontCompany = Font(app::loadResource(RES_AKKURAT_LIGHT), SESSION_LABEL_SPEAKER_FONT_SIZE * SESSION_LABEL_SPEAKER_FONT_SCALE);
            }


            mTextBox->setFont(      sFontName);
            mTextBox->setWidth(     SESSION_LABEL_SPEAKER_BOX_WIDTH);
            mTextBox->setFontSize(  SESSION_LABEL_SPEAKER_FONT_SIZE);
            mTextBox->setColorFont( SESSION_LABEL_SPEAKER_NAME_FONT_COLOR);
            
                
            mTextBoxCompany = new TextBox();
            mTextBoxCompany->setFont(      sFontCompany);
            mTextBoxCompany->setWidth(     SESSION_LABEL_SPEAKER_BOX_WIDTH);
            mTextBoxCompany->setFontSize(  SESSION_LABEL_SPEAKER_FONT_SIZE);
            mTextBoxCompany->setColorFont( SESSION_LABEL_SPEAKER_COMPANY_FONT_COLOR);
            
            mScale = 1.0f / (SESSION_LABEL_EVENT_TITLE_FONT_SIZE * 10);
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
        float alphaName    = mAlphaStateName();
        float alphaCompany = mAlphaStateCompany();
        
        
        Vec2f pos = mPos + mTextBox->getTopLeft();
        
        glPushMatrix();
        glTranslatef(pos.x, 0 , pos.y);
        
        glMultMatrixf(&mTransform[0]);
        glScalef(mScale, mScale, 1.0f);
        
        glColor4f(1,1,1, alphaName);
        gl::draw(mTextBox->getTexture());
#ifdef SESSION_VIEW_LABEL_SPEAKER_DEBUG_DRAW
        mTextBox->debugDraw();
#endif
        
        glTranslatef(0, mTextBoxCompanyOffsetY, 0);
        glColor4f(1,1,1, alphaCompany);
        gl::draw(mTextBoxCompany->getTexture());
#ifdef SESSION_VIEW_LABEL_SPEAKER_DEBUG_DRAW
        mTextBoxCompany->debugDraw();
#endif
        glColor4f(1,1,1,1);
        
        glPopMatrix();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Set
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerLabel::set(const string& name, const string& company){
        mTextBox->setString(name);
        mTextBoxCompanyOffsetY = mTextBox->getCalculatedSize().y * SESSION_LABEL_SPEAKER_LINE_HEIGHT;
        mTextBoxCompany->setString(company);
        
        mAlphaStateName    = 0.0f;
        mAlphaStateCompany = 0.0f;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Show / Hide
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerLabel::show(){
        tween(&mAlphaStateName,    0.0f, 1.0f, 1.0f, AnimEaseInOut());
        tween(&mAlphaStateCompany, 0.0f, 1.0f, 1.5f, AnimEaseInOut());
    }
    
    void SpeakerLabel::hide(){
        tween(&mAlphaStateName,    0.0f, 1.0f, AnimEaseInOut());
        tween(&mAlphaStateCompany, 0.0f, 1.5f, AnimEaseInOut());
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
        tween(&mAlphaStateCompany, 0.0f, 1.5f, AnimEaseInOut());
    }
    
}