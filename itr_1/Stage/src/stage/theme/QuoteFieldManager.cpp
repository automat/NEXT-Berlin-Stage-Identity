#include "stage/theme/QuoteFieldManager.h"
#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>
#include "cinder/Rand.h"
#include "Config.h"
#include "cinder/CinderMath.h"
#include "cinder/Rand.h"

#include <boost/random/mersenne_twister.hpp>

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

namespace next {
    using namespace ci;
    using namespace boost::assign;
   
    /*--------------------------------------------------------------------------------------------*/
    //  QuoteFieldManager
    /*--------------------------------------------------------------------------------------------*/
    
    QuoteFieldManager::QuoteFieldManager(vector<Quote>* quotes, Grid* grid) :
        mQuotes(quotes),
        mNumQuotes(mQuotes->size()),
        mIndexQuoteFields(0),
        mIndexQuotes(0),
        mIndexSelected(-1),
        mActive(false){
            std::random_device rd;
            mRandBase = std::mt19937(rd());
            
            //
            //  preallocate every single quote
            //
            for(vector<Quote>::const_iterator itr = mQuotes->begin(); itr != mQuotes->end(); ++itr){
                mQuoteFields += vector<QuoteField*>();
                mOffsets     += vector<Offset>();
                
                vector<QuoteField*>& quoteFieldsBack = mQuoteFields.back();
                vector<Offset>&      offsetsBack     = mOffsets.back();
                
                const vector<QuoteLine>& lines = itr->lines;
                for(vector<QuoteLine>::const_iterator _itr = lines.begin(); _itr != lines.end(); ++_itr){
                    quoteFieldsBack += new QuoteField(grid->getCell(_itr->getIndicesFront())->getCenter(),
                                                      Rand::randInt(QUOTE_FIELD_NUM_DIVERS_MIN, QUOTE_FIELD_NUM_DIVERS_MAX),
                                                      *_itr );
                    offsetsBack += Offset();
                }
            }
    }
    
    QuoteFieldManager::~QuoteFieldManager(){
        while(!mQuoteFields.empty()){
            while (!mQuoteFields.back().empty()) {
                delete mQuoteFields.back().back();
                mQuoteFields.back().pop_back();
            }
            mQuoteFields.pop_back();
        }
    }
    
    int QuoteFieldManager::getRandomIndex(int index){
        if(index == 0){
            mRandomIndicesQuotes.clear();
            int i = -1, l = mNumQuotes;
            while(++i < l){mRandomIndicesQuotes += i;}
            
            std::shuffle(mRandomIndicesQuotes.begin(), mRandomIndicesQuotes.end(), mRandBase);
            while (mRandomIndicesQuotes.front() == mIndexSelected) { //prevent end == start
                std::shuffle(mRandomIndicesQuotes.begin(), mRandomIndicesQuotes.end(), mRandBase);
            }
        }
        return mRandomIndicesQuotes[index];
    }
    
    void QuoteFieldManager::play(int num, std::function<void ()> callback){
        if(mActive){
            return;
        }
        
        mActive    = true;
        mMaxPlays  = num;
        mPlayCount = 0;
        mCallback  = callback;
        nextQuote();
    }
    
    bool QuoteFieldManager::isActive(){
        return mActive;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Update
    /*--------------------------------------------------------------------------------------------*/

    void QuoteFieldManager::update(Oscillator* osc, float t){
        if (!mActive) {
            return;
        }
        size_t selectedAdress = reinterpret_cast<size_t>(mQuoteFieldsSelected);
        
        int i = 0;
        for(vector<QuoteField*>::const_iterator itr = mQuoteFieldsSelected->begin(); itr != mQuoteFieldsSelected->end();++itr){
            if(reinterpret_cast<size_t>(mQuoteFieldsSelected) != selectedAdress){
                return; //  current adress did change 
            }
            QuoteField* quoteField(*itr);
            Offset&     offset((*mOffsetsSelected)[i++]);
            
            quoteField->update(osc, t);
            offset.update();
            quoteField->updateDivers(offset.getValue());
        }
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Draw
    /*--------------------------------------------------------------------------------------------*/

    void QuoteFieldManager::draw(const CameraOrtho& camera){
        if (!mActive) {
            return;
        }
        
        for(vector<QuoteField*>::const_iterator itr = mQuoteFieldsSelected->begin(); itr != mQuoteFieldsSelected->end(); itr++){
            (*itr)->draw();
        }
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // next
    /*--------------------------------------------------------------------------------------------*/

    void QuoteFieldManager::nextQuote(){
        int randIndex = getRandomIndex(mIndexQuotes);
        
        vector<QuoteField*>* quoteFieldSelected = &mQuoteFields[randIndex];
        vector<Offset>*      offsetsSelected    = &mOffsets[randIndex];
        Quote*               quoteSelected      = &(*mQuotes)[randIndex];
        
        float from, time, delayStep, delay = 0;
        
        delayStep = Rand::randFloat(THEME_FIELD_QUOTE_MANAGER_ANIM_IN_DELAY_STEP_MIN,
                                    THEME_FIELD_QUOTE_MANAGER_ANIM_IN_DELAY_STEP_MAX);
        time      = Rand::randFloat(THEME_FIELD_QUOTE_MANAGER_ANIM_IN_TIME_MIN,
                                    THEME_FIELD_QUOTE_MANAGER_ANIM_IN_TIME_MAX);
        
        int i = -1;
        while (++i < quoteFieldSelected->size()) {
            from = Rand::randFloat(THEME_FIELD_QUOTE_MANAGER_ANUM_IN_FROM_MIN,
                                   THEME_FIELD_QUOTE_MANAGER_ANUM_IN_FROM_MAX);
            (*offsetsSelected)[i].reset(from, 1.925f, time, delay);
            (*offsetsSelected)[i].setCallback(std::bind(&QuoteFieldManager::onQuoteAtTarget, this));
            //reset quotefield offset here
            delay += delayStep;
        }
        
        mQuoteFieldsSelected = quoteFieldSelected;
        mOffsetsSelected     = offsetsSelected;
        mQuoteSelected       = quoteSelected;
        
        mNumQuoteFields   = mQuoteFieldsSelected->size();
        mIndexQuotes      = mPlayCount++ % mNumQuotes;
        mIndexQuoteFields = 0;
        mIndexSelected    = randIndex;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // callback proceed
    /*--------------------------------------------------------------------------------------------*/

    void QuoteFieldManager::onQuoteAtTarget(){
        if(mIndexQuoteFields == mQuoteFieldsSelected->size() - 1){
            float to, time, delayStep, delay = 0;

            delayStep = Rand::randFloat(THEME_FIELD_QUOTE_MANAGER_ANIM_OUT_DELAY_STEP_MIN,
                                        THEME_FIELD_QUOTE_MANAGER_ANIM_OUT_DELAY_STEP_MAX);
            time      = Rand::randFloat(THEME_FIELD_QUOTE_MANAGER_ANIM_OUT_TIME_MIN,
                                        THEME_FIELD_QUOTE_MANAGER_ANIM_OUT_TIME_MAX);
            
            for(vector<Offset>::iterator itr = mOffsetsSelected->begin(); itr != mOffsetsSelected->end(); ++itr){
                to = Rand::randFloat(THEME_FIELD_QUOTE_MANAGER_ANUM_OUT_TO_MIN,
                                     THEME_FIELD_QUOTE_MANAGER_ANUM_OUT_TO_MAX);
                
                itr->reset(itr->getValue(), to, time, delay);
                itr->setCallback(std::bind(&QuoteFieldManager::onQuoteAtEnd, this));
                delay += delayStep;
            }
            mIndexQuoteFields = 0;
        }
        mIndexQuoteFields++;
    }
    
    void QuoteFieldManager::onQuoteAtEnd(){
        if(mIndexQuoteFields == mOffsetsSelected->size() - 1){
            if(mPlayCount == mMaxPlays){
                if(mCallback){
                    mCallback();
                }
                mActive = false;
                return;
            }
            nextQuote();
        }
        mIndexQuoteFields++;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Debug Draw
    /*--------------------------------------------------------------------------------------------*/

    void QuoteFieldManager::debugDraw(){
        if(!mActive){
            return;
        }
        
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
        
        vector<QuoteField*>& quoteFields = *mQuoteFieldsSelected;
        const gl::Texture&   texture     = mQuoteSelected->texture;
        //
        
        glPushMatrix();
            glTranslatef(50, 50, 0);
            
            glPushMatrix();
                glTranslatef(0, rect.y2, 0);
                glColor4f(0, 0, 0, 0.75f);
                gl::drawSolidRect(rect);
                glColor4f(1, 1, 1, 1);
                gl::draw(texture, rect);
            glPopMatrix();
        
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
                    diverOffset  = diver->getOffsetX();
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

    
    /*--------------------------------------------------------------------------------------------*/
    //  get
    /*--------------------------------------------------------------------------------------------*/
    
    const gl::Texture& QuoteFieldManager::getTexture(){
        return mQuoteSelected->texture;
    }

    
}