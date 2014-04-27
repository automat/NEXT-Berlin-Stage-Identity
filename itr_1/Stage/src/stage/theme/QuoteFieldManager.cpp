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
        mNumQuotes(mQuotes->size()),
        mQuoteFields(quoteFields),
        mIndexQuoteFields(0),
        mGrid(grid),
        mIndexQuotes(0),
        mDraw(true),
        mFramesSkipped(-1){

            setQuote();
    }
    
    QuoteFieldManager::~QuoteFieldManager(){}
    
    void QuoteFieldManager::update(){
        int i = -1;
        int l = mQuoteFields->size();
        
        while (++i < l) {
            mOffsets[i].update();
            if(l != mNumQuoteFields){
                mDraw = false;  // prevent drawing corrupt vbo data next frame
                return;         // size has been changed, escape!
            }
        
            mQuoteFields->at(i)->updateDivers(mOffsets[i].getValue());
            l = mQuoteFields->size();
        }
    }
    
    void QuoteFieldManager::draw(){
        if(!mDraw){
            //  skip 5 frames, after reseting the quote fields and its vbos
            //  theres some corrupt data, when resizing the quotefields
            //  make sure we dont draw it
            //  overall: NOT GOOD!
            mFramesSkipped++;
            if(mFramesSkipped == 5){
                mFramesSkipped = -1;
                mDraw = true;
            }
            return;
        }
        
        const vector<QuoteField*>& quoteFields = *mQuoteFields;
        
        int i = -1;
        int l = quoteFields.size();
        
        while (++i < l) {
#ifdef DEBUG_THEME_FIELD_QUOTE
            quoteFields[i]->debugDrawArea();
            quoteFields[i]->debugDrawPathSurface();
            quoteFields[i]->debugDrawDivers();
#endif
#ifdef DEBUG_THEME_FIELD_QUOTE_TEXCOORDS
            
            quoteFields[i]->debugDrawDiverIndices(camera);
#endif
            quoteFields[i]->draw();
        }
    }
    
    /*
    void QuoteFieldManager::setQuote(){
        mOffsets.resize(0);
        while(!mQuoteFields->empty()) delete mQuoteFields->back(), mQuoteFields->pop_back();
        
        float delay     = 0;
        float delayStep = 0.125f;//2.5f;
        float time      = 2.0f;//8.0f;
        
        mQuoteSelected = &(mQuotes->at(mIndexQuotes));
        const vector<QuoteLine>& lines = mQuoteSelected->getLines();
        for(vector<QuoteLine>::const_iterator itr = lines.begin(); itr != lines.end(); ++itr){
            Offset offset(0.0f, 1.925f, time, delay);
            offset.setCallback(std::bind(&QuoteFieldManager::onQuoteAtTarget, this));
            mOffsets += offset;
            
            mQuoteFields->push_back(new QuoteField(mGrid->getCell(itr->getIndicesFront())->getCenter(),
                                                   Rand::randInt(QUOTE_FIELD_NUM_DIVERS_MIN, QUOTE_FIELD_NUM_DIVERS_MAX),
                                                   *itr ));
            delay += delayStep;
        }
        
        mNumQuoteFields   = lines.size();
        mIndexQuotes      = (mIndexQuotes + 1) % mNumQuotes;
        mIndexQuoteFields = 0;
    }*/
    
    void QuoteFieldManager::setQuote(){
        if(mQuoteFields->empty()){
            mOffsets.resize(0);
            while(!mQuoteFields->empty()) delete mQuoteFields->back(), mQuoteFields->pop_back();
        }
        
        float delay     = 0;
        float delayStep = 0.125f;//2.5f;
        float time      = 2.0f;//8.0f;
        
        mQuoteSelected = &(mQuotes->at(mIndexQuotes));
        const vector<QuoteLine>& lines = mQuoteSelected->getLines();
        
        if(mQuoteFields->empty()){
            
            for(vector<QuoteLine>::const_iterator itr = lines.begin(); itr != lines.end(); ++itr){
                Offset offset(0.0f, 1.925f, time, delay);
                offset.setCallback(std::bind(&QuoteFieldManager::onQuoteAtTarget, this));
                mOffsets += offset;
                
                mQuoteFields->push_back(new QuoteField(mGrid->getCell(itr->getIndicesFront())->getCenter(),
                                                       Rand::randInt(QUOTE_FIELD_NUM_DIVERS_MIN, QUOTE_FIELD_NUM_DIVERS_MAX),
                                                       *itr ));
                delay += delayStep;
            }
           // mIndexQuotes      = (mIndexQuotes + 1) % mNumQuotes;
        } else {
            for(vector<Offset>::iterator itr = mOffsets.begin(); itr != mOffsets.end(); ++itr){
                itr->reset(0.0f, 1.925f, time, delay);
                itr->setCallback(std::bind(&QuoteFieldManager::onQuoteAtTarget, this));
            }
        }
        
        
        mNumQuoteFields   = lines.size();
        mIndexQuoteFields = 0;
    }
    
    
    
    void QuoteFieldManager::onQuoteAtTarget(){
        if(mIndexQuoteFields == mQuoteFields->size() - 1){
            float delay     = 0;
            float delayStep = 0.125f; //0.5f;
            float time      = 2.0f;//10.0f;
            for(vector<Offset>::iterator itr = mOffsets.begin(); itr != mOffsets.end(); ++itr){
                itr->reset(itr->getValue(), 3.0f, time, delay);
                itr->setCallback(std::bind(&QuoteFieldManager::onQuoteAtEnd, this));
                delay += delayStep;
            }
            mIndexQuoteFields = 0;
        }
        mIndexQuoteFields++;
        
    }
    
    void QuoteFieldManager::onQuoteAtEnd(){
        if(mIndexQuoteFields == mQuoteFields->size() - 1){
            setQuote();
        }
        mIndexQuoteFields++;
        
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
        
        //int k = -1;
        //while (++k < 2) {
            glTranslatef(0, 0, 0);
            //const gl::Texture& texture = (*mQuotesSelected[k]).getTexture();
        const gl::Texture& texture = mQuoteSelected->getTexture();
        
            glPushMatrix();
            glTranslatef(0, rect.y2, 0);
            glColor4f(0, 0, 0, 0.75f);
            gl::drawSolidRect(rect);
        
        /*
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
         */
            
            glColor4f(1, 1, 1, 1);
            gl::draw(texture, rect);
            glPopMatrix();
            
        
            
            float stepV   = rect.y2 / static_cast<float>(mQuoteFields->size());
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
            for (vector<QuoteField*>::iterator itr = mQuoteFields->begin(); itr != mQuoteFields->end(); ++itr) {
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
        //}
        
        
        glPopMatrix();
    }
#endif
    
    /*--------------------------------------------------------------------------------------------*/
    //  get
    /*--------------------------------------------------------------------------------------------*/
    
    const gl::Texture& QuoteFieldManager::getTexture(){
        return mQuoteSelected->getTexture();
    }

    
}