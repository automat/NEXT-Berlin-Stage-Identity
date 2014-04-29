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

    Font SpeakerLabel::sFontName;
    Font SpeakerLabel::sFontCompany;

    /*--------------------------------------------------------------------------------------------*/
    //  Constructor / Destructor
    /*--------------------------------------------------------------------------------------------*/

    SpeakerLabel::SpeakerLabel() :
        AbstractLabel(),
        mTextBoxCompanyOffsetY(0),
        mOffsetStateName(0),
        mOffsetStateCompany(0){
            static bool __fontInitialized(false);
            if(!__fontInitialized){
                sFontName    = Font(app::loadResource(RES_AKKURAT_BOLD),  SESSION_LABEL_SPEAKER_FONT_SIZE * SESSION_LABEL_SPEAKER_FONT_SCALE);
                sFontCompany = Font(app::loadResource(RES_AKKURAT_LIGHT), SESSION_LABEL_SPEAKER_FONT_SIZE * SESSION_LABEL_SPEAKER_FONT_SCALE);
            }

            string supportedChars = gl::TextureFont::defaultChars() + "å";
            
            
            mTextBox->setFont(      sFontName, supportedChars);
            mTextBox->setWidth(     SESSION_LABEL_SPEAKER_BOX_WIDTH);
            mTextBox->setFontSize(  SESSION_LABEL_SPEAKER_FONT_SIZE);
            mTextBox->setColorFont( SESSION_LABEL_SPEAKER_NAME_FONT_COLOR);

            mTextBoxCompany = new TextBox();
            mTextBoxCompany->setFont(      sFontCompany, supportedChars);
            mTextBoxCompany->setWidth(     SESSION_LABEL_SPEAKER_BOX_WIDTH);
            mTextBoxCompany->setFontSize(  SESSION_LABEL_SPEAKER_FONT_SIZE);
            mTextBoxCompany->setColorFont( SESSION_LABEL_SPEAKER_COMPANY_FONT_COLOR);
            mTextBoxCompany->setLineHeight(SESSION_LABEL_SPEAKER_COMPANY_LINE_HEIGHT);
            
            mScale = 1.0f / (SESSION_LABEL_EVENT_TITLE_FONT_SIZE * 7);
    }
    
    SpeakerLabel::~SpeakerLabel(){
        delete mTextBoxCompany;
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
        if(mTextBox->empty()){
            return;
        }
        float alphaName     = mAlphaStateName();
        float alphaCompany  = mAlphaStateCompany();
        float offsetName    = mOffsetStateName();
        float offsetCompany = mOffsetStateCompany();
        
        Vec2f pos = mPos + mTextBox->getTopLeft();
        
        glPushMatrix();
        glTranslatef(pos.x, 0 , pos.y);
        
            glMultMatrixf(&mTransform[0]);
            glScalef(mScale, mScale, 1.0f);
            
            glColor4f(1,1,1, alphaName);
            glPushMatrix();
                glTranslatef(offsetName,0,0);
                gl::draw(mTextBox->getTexture());
#ifdef SESSION_VIEW_LABEL_SPEAKER_DEBUG_DRAW
                mTextBox->debugDraw();
#endif
            glPopMatrix();
        
        if(!mTextBoxCompany->empty()){
            glPushMatrix();
            glTranslatef(0, mTextBoxCompanyOffsetY, 0);
            glColor4f(1,1,1, alphaCompany);
            glTranslatef(offsetCompany,0,0);
            gl::draw(mTextBoxCompany->getTexture());
#ifdef SESSION_VIEW_LABEL_SPEAKER_DEBUG_DRAW
            mTextBoxCompany->debugDraw();
#endif
            glPopMatrix();
        }
        
            glColor4f(1,1,1,1);
        
        glPopMatrix();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Set
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerLabel::set(const string& name, const string& company){
        mTextBox->setString(name);
        mTextBoxCompanyOffsetY = mTextBox->getCalculatedSize().y * SESSION_LABEL_SPEAKER_LINE_HEIGHT;
        
        if(!company.empty()){
            mTextBoxCompany->setString(company);
        }
        
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