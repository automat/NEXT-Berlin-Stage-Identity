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
    QuoteFieldManager::Offset::Offset() : origin(0), target(0), duration(1), time(0){}
    
    QuoteFieldManager::Offset::Offset(float origin, float target, float duration) : time(0){
        reset(origin, target, duration);
    }
    
    void QuoteFieldManager::Offset::reset(float origin, float target, float duration){
        this->origin = this->value = origin;
        this->target = target;
        this->dist   = this->target - this->origin;
        this->time   = 0;
        this->duration = duration;
    }
    
    void QuoteFieldManager::Offset::update(){
        if (time > duration) return;
        
        value = dist * time++ / duration + origin;
        //cout << value << endl;
        
    }

    //
    
    QuoteFieldManager::QuoteFieldManager(vector<Quote>* quotes, vector<QuoteField*>* quoteFields, Grid* grid) :
        mQuotes(quotes),
        mQuoteFields(quoteFields),
        mGrid(grid),
        mIndexQuotes(0){
        
            mOffset.reset(-1, 2, 30.0 * APP_FPS);
            
            cout << 1.0 * APP_FPS << endl;
            
            setQuote((*mQuotes).front());
    }
    
    QuoteFieldManager::~QuoteFieldManager(){
        
    }
    
    void QuoteFieldManager::update(){
        static float t(0.5);
        
        t+=0.025f;
        t = t > 1.0f ? 0.5f : t;
        
        
        //float ta = -2 + (sinf(t) * 0.5f + 0.5f) * 5.0f;
        
        float ta = t;
        
        mOffset.update();
        vector<QuoteField*>& quoteFields = (*mQuoteFields);
        
  
        
        for(vector<QuoteField*>::iterator itr = quoteFields.begin(); itr != quoteFields.end(); itr++){
            if(t > 0.75f)break;
            (*itr)->updateDivers(t);//mOffset.value);
        }
    }
    
    void QuoteFieldManager::setQuote(const next::Quote &quote){
        
        const vector<QuoteLine>& lines = quote.getLines();
        for(vector<QuoteLine>::const_iterator itr = lines.begin(); itr != lines.end(); ++itr){
            (*mQuoteFields) += new QuoteField(mGrid->getCell(itr->getIndices().front())->getCenter(),
                                              Rand::randInt(QUOTE_FIELD_NUM_DIVERS_MIN, QUOTE_FIELD_NUM_DIVERS_MAX),
                                              *itr );
        }
    }
    
    void QuoteFieldManager::debugDraw(){
        vector<QuoteField*>& quoteFields = (*mQuoteFields);
        
        glPushMatrix();
        glTranslatef(50, 50, 0);
        glColor3f(0, 0, 1);
        Rectf rect(0,0,app::getWindowWidth() * 0.5f - 50,400);
        gl::drawStrokedRect(rect);
        
        float stepV = rect.y2 / static_cast<float>(quoteFields.size());
        float stepH = rect.x2 / static_cast<float>(3);
        
        Rectf rectFieldH(0,0,rect.x2,stepV);
        Rectf rectFieldV(0,0,stepH,rect.y2);
        
        glColor3f(1, 0, 0);
        float i = 0;
        while (i < 3) {
            glPushMatrix();
            glTranslatef(stepH * i, 0, 0);
            gl::drawStrokedRect(rectFieldV);
            glPopMatrix();
            ++i;
        }
        
        
        Vec2f startVisible(stepH,0);
        Vec2f endVisible;
        Vec2f pos;
        
        float diverOffset;
        float diverLength;
        float rowDiver;
        float stepFieldV;
        bool  offsetInNormRange; // offset >= 0 && offset <= 1
        
        i = 0;
        for (vector<QuoteField*>::iterator itr = quoteFields.begin(); itr != quoteFields.end(); ++itr) {
            const vector<Diver*>& divers = (*itr)->getDivers();
            
            rowDiver   = 0;
            stepFieldV = stepV / static_cast<float>(divers.size());
           
            glPushMatrix();
            glTranslatef(0, stepV * i++, 0);
            glColor3f(0, 0, 1);
            
            gl::drawStrokedRect(rectFieldH);
            
            for(vector<Diver*>::const_iterator itr = divers.begin(); itr != divers.end(); itr++){
                Diver* diver = (*itr);
                diverOffset  = diver->getOffset();
                diverLength  = diver->getLength();
                
                pos.x  = MAX(0,lmap<float>(diverOffset, -1, 2, 0, rect.x2));
                pos.y  = stepFieldV * rowDiver++;
     
                offsetInNormRange = diverOffset >= 0 && diverOffset <= 1;
                
                startVisible.y = endVisible.y = pos.y;
                startVisible.x = stepH;//MAX(stepH, MIN(pos.x - static_cast<float>(diver->getLength()), stepH * 2));
                endVisible.x   = pos.x;//MAX(stepH, MIN(pos.x, stepH * 2));
                
                if(!offsetInNormRange){
                    glColor3f(0, 0, 1);
                } else {
                    glColor3f(1, 1, 1);
                }
                
                //gl::drawLine(startVisible, endVisible);
                gl::drawSolidCircle(pos, 2.0f);
            }
            glPopMatrix();
        }

        glPopMatrix();
        
    }
    
    const Quote* QuoteFieldManager::getCurrQuote(){
        return &(*mQuotes)[mIndexQuotes];
    }
    
}