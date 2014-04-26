#include "stage/theme/QuoteFieldManager.h"
#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>
#include "cinder/Rand.h"
#include "Config.h"
#include "cinder/CinderMath.h"
#include "util/MathUtil.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

namespace next {
    using namespace ci;
    using namespace boost::assign;
    
    /*--------------------------------------------------------------------------------------------*/
    //  Offset
    /*--------------------------------------------------------------------------------------------*/
    
    QuoteFieldManager::Offset::Offset() :
        mOrigin(0),
        mTarget(0),
        mDuration(1),
        mTime(0),
        mLoop(false){}
    
    QuoteFieldManager::Offset::Offset(float origin, float target, float duration, bool loop) : mTime(0){
        reset(origin, target, duration, loop);
    }
    
    void QuoteFieldManager::Offset::reset(float origin, float target, float duration, bool loop){
        mOrigin   = mValue = origin;
        mTarget   = target;
        mDist     = mTarget - mOrigin;
        mTime     = 0;
        mDuration = duration * APP_FPS;
        mLoop     = loop;
    }
    
    void QuoteFieldManager::Offset::update(){
        if (mTime > mDuration){
            if(mLoop){
                mTime = 0;
            } else {
                return;
            }
        }
        mValue = mDist * mTime++ / mDuration + mOrigin;
    }
    
    float QuoteFieldManager::Offset::getValue(){
        return mValue;
    }

    /*--------------------------------------------------------------------------------------------*/
    //  QuoteFieldManager
    /*--------------------------------------------------------------------------------------------*/
    
    QuoteFieldManager::QuoteFieldManager(vector<Quote>* quotes, vector<QuoteField*>* quoteFields, Grid* grid) :
        mQuotes(quotes),
        mQuoteFields(quoteFields),
        mGrid(grid),
        mIndexQuotes(0){
        
            mOffset.reset(-1, 3, 10.0f, true);
            setQuote((*mQuotes).front());
    }
    
    QuoteFieldManager::~QuoteFieldManager(){}
    
    void QuoteFieldManager::update(){
        vector<QuoteField*>& quoteFields = (*mQuoteFields);

        for(vector<QuoteField*>::iterator itr = quoteFields.begin(); itr != quoteFields.end(); itr++){
            (*itr)->updateDivers(mOffset.getValue());
        }

        mOffset.update();
    }
    
    void QuoteFieldManager::setQuote(const next::Quote &quote){
        
        const vector<QuoteLine>& lines = quote.getLines();
        for(vector<QuoteLine>::const_iterator itr = lines.begin(); itr != lines.end(); ++itr){
            (*mQuoteFields) += new QuoteField(mGrid->getCell(itr->getIndices().front())->getCenter(),
                                              Rand::randInt(QUOTE_FIELD_NUM_DIVERS_MIN, QUOTE_FIELD_NUM_DIVERS_MAX),
                                              *itr );
        }
    }
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Debug Draw
    /*--------------------------------------------------------------------------------------------*/
#ifdef DEBUG_THEME_FIELD_QUOTE_MANAGER
    void QuoteFieldManager::debugDraw(){
        vector<QuoteField*>& quoteFields = (*mQuoteFields);
        
        Rectf rect(0,0,app::getWindowWidth() * 0.25f - 50,400);
        float stepV   = rect.y2 / static_cast<float>(quoteFields.size());
        float stepH   = rect.x2 / static_cast<float>(3); //-1,0,1,2
        float stepH_2 = stepH * 2.0f;
        
        Rectf rectFieldH(0,0,rect.x2,stepV);
        Rectf rectFieldV(0,0,stepH,rect.y2);
        
        Vec2f tailLeft;
        Vec2f tailRight;
        Vec2f tailLeftVisible;
        Vec2f tailRightVisible;
        Vec2f pos;
        
        float diverOffset;
        float diverLength;
        float rowDiver;
        float stepFieldV;
        bool  offsetInNormRange; // offset >= 0 && offset <= 1
        
        //
        
        glPushMatrix();
        glTranslatef(50, 50, 0);
        
        glColor4f(0, 0, 0, 0.45f);
        gl::drawSolidRect(rect);
        
        glColor3f(0, 0, 1);
        gl::drawStrokedRect(rect);
        
        glColor3f(1, 0, 0);
        float i = 0;
        while (i < 3) {
            glPushMatrix();
            glTranslatef(stepH * i, 0, 0);
            gl::drawStrokedRect(rectFieldV);
            glPopMatrix();
            ++i;
        }
        
        i = 0;
        for (vector<QuoteField*>::iterator itr = quoteFields.begin(); itr != quoteFields.end(); ++itr) {
            const vector<Diver*>& divers = (*itr)->getDivers();
            
            rowDiver   = 0;
            stepFieldV = stepV / static_cast<float>(divers.size());
           
            glPushMatrix();
            glTranslatef(0, stepV * i++, 0);
            
            glColor4f(0, 0, 0, 0.0125f);
            gl::drawSolidRect(rectFieldH);
            
            glColor3f(0, 0, 1);
            gl::drawStrokedRect(rectFieldH);
            
            for(vector<Diver*>::const_iterator _itr = divers.begin(); _itr != divers.end(); _itr++){
                Diver* diver = (*_itr);
                diverOffset  = diver->getOffset();
                diverLength  = diver->getLength();
                
                pos.x  = MAX(0,lmap<float>(diverOffset, -1, 2, 0, rect.x2));
                pos.y  = stepFieldV * rowDiver++ + stepFieldV * 0.5f;
     
                offsetInNormRange = diverOffset >= 0 && diverOffset <= 1;
                
                tailLeft.y  = tailRight.y = tailLeftVisible.y = tailRightVisible.y = pos.y;
                tailRight.x = pos.x;
                tailLeft.x  = MAX(0, tailRight.x - diverLength * stepH);

                glColor3f(0, 0, 1);
                gl::drawLine(tailLeft, tailRight);
                
                if(!offsetInNormRange){
                    glColor3f(0, 0, 1);
                } else {
                    glColor3f(1, 1, 1);
                }
                
                gl::drawSolidCircle(pos, 2.0f);

                tailLeftVisible.x  = MAX(stepH,tailLeft.x);
                tailRightVisible.x = MAX(stepH,MIN(stepH_2,tailRight.x));

                if(tailLeftVisible.x == tailRightVisible.x){
                    continue;
                }

                glColor3f(1,1,1);
                gl::drawSolidCircle(tailLeftVisible, 2);
                gl::drawSolidCircle(tailRightVisible, 2);
                glLineWidth(2);
                gl::drawLine(tailLeftVisible,tailRightVisible);
                glLineWidth(1);
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
#endif
    
    /*--------------------------------------------------------------------------------------------*/
    //  get
    /*--------------------------------------------------------------------------------------------*/
    
    const Quote* QuoteFieldManager::getCurrQuote(){
        return &(*mQuotes)[mIndexQuotes];
    }
    
}