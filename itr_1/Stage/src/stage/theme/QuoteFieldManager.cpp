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
    //  QuoteFieldManager
    /*--------------------------------------------------------------------------------------------*/
    
    QuoteFieldManager::QuoteFieldManager(vector<Quote>* quotes, vector<QuoteField*>* quoteFields, Grid* grid) :
        mQuotes(quotes),
        mGrid(grid),
        mIndexQuotes(0),
        mIndex(0){
            mNumQuotes = mQuotes->size();
            
            mQuoteFields[0] = &quoteFields[0];
            mQuoteFields[1] = &quoteFields[1];
            
            mIndexQuoteFields[0] = 0;
            mIndexQuoteFields[1] = 0;
            
            mQuotesSelected[0] = &mQuotes->at(0);
            mQuotesSelected[1] = &mQuotes->at(1 % mNumQuotes);
            
            mQuoteFieldsActiveStates[0] = true;
            mQuoteFieldsActiveStates[1] = false;
            
            setQuote(mIndex);
    }
    
    QuoteFieldManager::~QuoteFieldManager(){}
    
    void QuoteFieldManager::update(){
        int k = -1;
        while (++k < 2) {
            vector<QuoteField*>& quoteFields = (*mQuoteFields[k]);
            vector<Offset>&      offsets     = mOffsets[k];
            
            int i = -1;
            while(++i < quoteFields.size()){
                Offset& offset = offsets[i];
                offset.update();
                quoteFields[i]->updateDivers(offset.getValue());
            }
        }
    }
    
    void QuoteFieldManager::setQuote( int index){
        Quote& quote                     = mQuotes->at(mIndexQuotes);
        vector<QuoteField*>& quoteFields = *mQuoteFields[index];
        vector<Offset>&      offsets     = mOffsets[index];
        mQuotesSelected[index]           = &quote;
        
        //
        //  clear
        //
        while (!quoteFields.empty()) delete quoteFields.back(), quoteFields.pop_back();
        offsets.clear();
        
        static const float from     = -1;
        static const float to       = 1.9125f;
        static const float duration = 10.0f;
        
        float delayStep = 2.0f;
        float delay     = 0;
        
        const vector<QuoteLine>& lines = quote.getLines();
        for(vector<QuoteLine>::const_iterator itr = lines.begin(); itr != lines.end(); ++itr){
            quoteFields += new QuoteField(mGrid->getCell(itr->getIndices().front())->getCenter(),
                                              Rand::randInt(QUOTE_FIELD_NUM_DIVERS_MIN, QUOTE_FIELD_NUM_DIVERS_MAX),
                                              *itr );
            offsets += Offset(from, to, duration, delay, false);
            offsets.back().setCallback(std::bind(&QuoteFieldManager::onQuoteAtTarget,this,index));
            delay    += delayStep;
        }
        
        mIndexQuoteFields[index] = 0;
        mNumQuoteFields[index]   = quoteFields.size();
        mIndexQuotes++;
        mIndexQuotes = mIndexQuotes % mNumQuotes;
    }
    
    
    
    void QuoteFieldManager::onQuoteAtTarget(int index){
        mIndexQuoteFields[index]++;
        
        if(mIndexQuoteFields[index] == (mNumQuoteFields[index] - 1)){
            cout <<  index  << " : " << mIndexQuoteFields[index] << " / " << (mNumQuoteFields[index] - 1) << endl;
            
            vector<Offset>& offsets = mOffsets[index];
            for (vector<Offset>::iterator itr = offsets.begin(); itr != offsets.end(); ++itr) {
                itr->reset(itr->getValue(), 3.0f, 2.0f, 0, false);
                itr->setCallback(std::bind(&QuoteFieldManager::setQuote,this,(mIndex - 1)));
                
            }
            
            
            swap();
        }
    }
    
    void QuoteFieldManager::swap(){
        mIndex = 1 - mIndex;
    }
    
    void QuoteFieldManager::draw(){
        
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Debug Draw
    /*--------------------------------------------------------------------------------------------*/
#ifdef DEBUG_THEME_FIELD_QUOTE_MANAGER
    void QuoteFieldManager::debugDraw(){
        
        
        Rectf rect(0,0,200,200);
        
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
        
        int k = -1;
        while (++k < 2) {
            glTranslatef(rect.x2 * k, 0, 0);
            const gl::Texture& texture = (*mQuotesSelected[k]).getTexture();
            
            glPushMatrix();
            glTranslatef(0, rect.y2, 0);
            glColor4f(0, 0, 0, 0.75f);
            gl::drawSolidRect(rect);
            
            if (k != mIndex) {
                glColor3f(0, 0, 1);
                gl::drawStrokedRect(rect);
            } else {
                glColor3f(1,1,1);
                glLineWidth(4);
                gl::drawStrokedRect(rect);
                glLineWidth(1);
                glColor3f(0, 0, 1);
            }
            
            glColor4f(1, 1, 1, 1);
            gl::draw(texture, rect);
            glPopMatrix();
            
            vector<QuoteField*>& quoteFields = *mQuoteFields[k];;
            
            float stepV   = rect.y2 / static_cast<float>(quoteFields.size());
            float stepH   = rect.x2 / static_cast<float>(3); //-1,0,1,2
            float stepH_2 = stepH * 2.0f;
            
            Rectf rectFieldH(0,0,rect.x2,stepV);
            Rectf rectFieldV(0,0,stepH,rect.y2);
            
            
            glColor4f(0, 0, 0, 0.75f);
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
        }
        
        
        glPopMatrix();
    }
#endif
    
    /*--------------------------------------------------------------------------------------------*/
    //  get
    /*--------------------------------------------------------------------------------------------*/
    
    const Quote* QuoteFieldManager::getSelectedQuote(int index){
        return mQuotesSelected[index];
    }

    
}