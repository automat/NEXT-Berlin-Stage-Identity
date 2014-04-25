#include "stage/theme/QuoteFieldManager.h"
#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>
#include "cinder/Rand.h"
#include "Config.h"
#include "cinder/CinderMath.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

namespace next {
    using namespace ci;
    using namespace boost::assign;
    
    QuoteFieldManager::QuoteFieldManager(vector<Quote>* quotes, vector<QuoteField*>* quoteFields, Grid* grid) :
        mQuotes(quotes),
        mQuoteFields(quoteFields),
        mGrid(grid),
        mIndexQuotes(0){
        
            setQuote((*mQuotes).front());
    }
    
    QuoteFieldManager::~QuoteFieldManager(){
        
    }
    
    void QuoteFieldManager::update(){
        static float t(0);
        t+=0.0125f;
        
        float ta = -1 + (sinf(t) * 0.5f + 0.5f) * 3.0f;
        
        vector<QuoteField*>& quoteFields = (*mQuoteFields);
        for(vector<QuoteField*>::iterator itr = quoteFields.begin(); itr != quoteFields.end(); itr++){
            (*itr)->updateDivers(ta);
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
        Rectf rect(0,0,400,400);
        gl::drawStrokedRect(rect);
        
        float stepV = rect.y2 / static_cast<float>(quoteFields.size());
        float stepH = rect.x2 / static_cast<float>(quoteFields.size());
        Rectf rectField(0,0,rect.x2,stepV);
        
        float row = 0;
        for (vector<QuoteField*>::iterator itr = quoteFields.begin(); itr != quoteFields.end(); ++itr) {
            glPushMatrix();
            glTranslatef(0, stepV * static_cast<float>(row++), 0);
            gl::drawStrokedRect(rectField);
            
            const vector<Diver*>& divers = (*itr)->getDivers();
            for(vector<Diver*>::const_iterator itr = divers.begin(); itr != divers.end(); itr++){
                Diver* diver = (*itr);
                Vec2f pos;
                pos.x  = lmap<float>(diver->getOffset(), -1, 2, 0, rect.x2);
                pos.y  = diver->getPos().x * stepV;
                gl::drawSolidCircle(pos, 5.0f);
            }
            
            glPopMatrix();
        }
        
        
        
        
        glPopMatrix();
        
    }
    
    const Quote* QuoteFieldManager::getCurrQuote(){
        return &(*mQuotes)[mIndexQuotes];
    }
    
}