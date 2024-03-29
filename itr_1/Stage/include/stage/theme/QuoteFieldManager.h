//
//  QuoteManager.h
//  Stage
//
//  Created by Henryk Wollik on 25/04/14.
//
//

#ifndef Stage_QuoteManager_h
#define Stage_QuoteManager_h

#include <vector>

#include "data/quote/Quote.h"

#include "stage/grid/Grid.h"
#include "stage/theme/field/QuoteField.h"
#include "stage/theme/Offset.h"
#include "stage/Oscillator.h"

#include <boost/random/uniform_int_distribution.hpp>


namespace next {
    using namespace std;
    class QuoteFieldManager{
        
        /*--------------------------------------------------------------------------------------------*/
        //  QuoteFieldManager
        /*--------------------------------------------------------------------------------------------*/
        
        vector<Quote>*        mQuotes;
        size_t                mNumQuotes;
        int                   mIndexQuotes;
        int                   mIndexSelected;
        
        
        vector<int>      mRandomIndicesQuotes;
        std::mt19937     mRandBase;
        
        bool                  mActive;
        int                   mPlayCount;
        int                   mMaxPlays;
        std::function<void()> mCallbackUpdate;
        std::function<void()> mCallbackFinish;
        
        
        Quote*                       mQuoteSelected;
        vector<vector<QuoteField*>>  mQuoteFields;
        vector<QuoteField*>*         mQuoteFieldsSelected;
        vector<vector<Offset>>       mOffsets;
        vector<Offset>*              mOffsetsSelected;
        
        size_t                       mNumQuoteFields;
        int                          mIndexQuoteFields;

        void onQuoteAtTarget();
        void onQuoteAtEnd();
        void nextQuote();
        
        int getRandomIndex(int index);
        
    public:
        QuoteFieldManager(vector<Quote>* quotes, Grid* grid);
        ~QuoteFieldManager();

        void debugDraw();

        void update(Oscillator* osc, float t);
        void draw(const CameraOrtho& camera);
        
        const gl::Texture& getTexture();
        
        void play(int num,const std::function<void ()>& callbackUpdate, const std::function<void()>& callbackFinish);
        
        bool isActive();
        
        int getQuoteIndex();
        size_t getNumQuotes();
    };
}

#endif
