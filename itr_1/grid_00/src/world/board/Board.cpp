#include "world/board/Board.h"

#include <iostream>
#include <vector>

#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assert.hpp>

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Config.h"
#include "layout/quote/QuoteAlign.h"

using namespace std;
using namespace boost::assign;
using namespace ci;

/*--------------------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------------------*/



Board::Board(Grid* grid, const LayoutArea& area, vector<Quote>* quotes) :
    mGrid(grid),
    mArea(area),
    mQuotes(quotes){
    mOscillator = new Oscillator();
#if defined(DEBUG_SINGLE_DIVER_FIELD) || \
    defined(DEBUG_SINGLE_QUOTE_FIELD)
    
#ifdef DEBUG_SINGLE_DIVER_FIELD
        {
            Cell* cell = grid->getCells()[84];
            mDiverFields += new DiverField(cell->getCenter(),2);
            mIndexDiverFieldMap[cell->getIndex()] = mDiverFields.back();
        }
#endif

#ifdef DEBUG_SINGLE_QUOTE_FIELD
        vector<Index> tempQuoteLineIndices   = {Index(6,6),Index(10,0)};
        vector<Vec2f> tempQuoteLineTexcoords = {Vec2f(),Vec2f(1,0),Vec2f(0,1),Vec2f(1,1)};
        mTempQuoteLine = QuoteLine(tempQuoteLineIndices,tempQuoteLineTexcoords,QuoteAlign::CENTER);
    
        mQuoteFields += new QuoteField(mGrid->getCell(mTempQuoteLine.getIndices().front())->getCenter(),1,mTempQuoteLine);
#endif
    
#else
        const vector<Cell*>& gridCells = mGrid->getCells();
        // Create diverfields according to layoutarea
        for(vector<Cell*>::const_iterator itr = gridCells.begin(); itr != gridCells.end(); ++itr){
            const Cell* cell = *itr;
            const Vec3f& pos =cell->getCenter();
            if(area.contains(pos)){
                const Index& index = cell->getIndex();
                mDiverFields += new DiverField(pos,Rand::randInt(DIVER_FIELD_NUM_DIVERS_MIN, DIVER_FIELD_NUM_DIVERS_MAX));
                mIndexDiverFieldMap[index] = mDiverFields.back();
            }
        }

        setQuote((*mQuotes)[0]);
#endif

}

/*--------------------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------------------*/

void Board::deleteDiverFields(){
    mIndexDiverFieldMap.clear();
    while (!mDiverFields.empty()) delete mDiverFields.back(), mDiverFields.pop_back();
}

void Board::deleteQuoteFields(){
    mIndexQuoteFieldMap.clear();
    while (!mQuoteFields.empty()) delete mQuoteFields.back(), mQuoteFields.pop_back();
}

Board::~Board(){
    deleteDiverFields();
    deleteQuoteFields();
    delete mOscillator;
}

/*--------------------------------------------------------------------------------------------*/
//  Gen
/*--------------------------------------------------------------------------------------------*/

void Board::setQuote(Quote& quote){
    deleteQuoteFields();
    
    const vector<QuoteLine>& lines = quote.getLines();
    for(vector<QuoteLine>::const_iterator itr = lines.begin(); itr != lines.end(); ++itr){
        cout << itr->getIndices().front() << endl;
        mQuoteFields += new QuoteField( mGrid->getCell(itr->getIndices().front())->getCenter(),
                                        Rand::randInt(QUOTE_FIELD_NUM_DIVERS_MIN, QUOTE_FIELD_NUM_DIVERS_MAX),
                                       *itr );
    }
    
    mQuoteCurrent = &quote;
}

/*--------------------------------------------------------------------------------------------*/
//  Draw
/*--------------------------------------------------------------------------------------------*/

void Board::draw(const CameraOrtho& camera){
    //  Debug Draw
#ifndef DEBUG_TYPESETTER
#if defined(DEBUG_BOARD_FIELD_DIVER_AREA_DRAW) || \
    defined(DEBUG_BOARD_FIELD_DIVER_PATH_SURFACE_DRAW) || \
    defined(DEBUG_BOARD_FIELD_DIVER_DIVER) || \
    defined(DEBUG_BOARD_FIELD_QUOTE_AREA_DRAW) || \
    defined(DEBUG_BOARD_FIELD_QUOTE_PATH_SURFACE_DRAW) || \
    defined(DEBUG_BOARD_FIELD_QUOTE_DIVER)
    gl::disableDepthRead();
#if defined(DEBUG_BOARD_FIELD_DIVER_AREA_DRAW) || \
    defined(DEBUG_BOARD_FIELD_DIVER_PATH_SURFACE_DRAW) || \
    defined(DEBUG_BOARD_FIELD_DIVER_DIVER)
    for(vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr){
#ifdef DEBUG_BOARD_FIELD_DIVER_AREA_DRAW
        (*itr)->debugDrawArea();
#endif
#ifdef DEBUG_BOARD_FIELD_DIVER_PATH_SURFACE_DRAW
        (*itr)->debugDrawPathSurface();
#endif
#ifdef DEBUG_BOARD_FIELD_DIVER_DIVER
        (*itr)->debugDrawDivers();
#endif
    }
#endif
    
#if defined(DEBUG_BOARD_FIELD_QUOTE_AREA_DRAW) || \
    defined(DEBUG_BOARD_FIELD_QUOTE_PATH_SURFACE_DRAW) || \
    defined(DEBUG_BOARD_FIELD_QUOTE_DIVER)
    for(vector<QuoteField*>::const_iterator itr = mQuoteFields.begin(); itr != mQuoteFields.end(); ++itr){
#ifdef DEBUG_BOARD_FIELD_QUOTE_AREA_DRAW
        (*itr)->debugDrawArea();
#endif
#ifdef DEBUG_BOARD_FIELD_QUOTE_PATH_SURFACE_DRAW
        (*itr)->debugDrawPathSurface();
#endif
#ifdef DEBUG_BOARD_FIELD_QUOTE_DIVER
        (*itr)->debugDrawDivers();
#endif
    }
    
#endif
    gl::enableDepthRead();
#endif

    //  Draw
#ifndef BOARD_SKIP_DRAW_FIELD_DIVER
    for(vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr){
        (*itr)->draw();
    }
#endif
    
#ifndef BOARD_SKIP_DRAW_QUOTE_DIVER
    //gl::enableAlphaTest();
    //gl::enableAdditiveBlending();

    glEnable(GL_ALPHA_TEST);
    //mQuoteCurrent->getTexture().enableAndBind();
    for(vector<QuoteField*>::const_iterator itr = mQuoteFields.begin(); itr != mQuoteFields.end(); ++itr){
        (*itr)->draw();
    }
    glDisable(GL_ALPHA_TEST);
    //mQuoteCurrent->getTexture().unbind();
    //gl::disableAlphaBlending();
    //gl::disableAlphaTest();
#endif
    
    //  Debug draw indices
#if defined(DEBUG_BOARD_FIELD_QUOTE_DIVER_TEXCOORDS) || \
    defined(DEBUG_BOARD_FIELD_QUOTE_TEXCOORDS)
    gl::disableDepthRead();
    for(vector<QuoteField*>::const_iterator itr = mQuoteFields.begin(); itr != mQuoteFields.end(); ++itr){
#ifdef DEBUG_BOARD_FIELD_QUOTE_DIVER_TEXCOORDS
        (*itr)->debugDrawDiverIndices(camera);
#endif
#ifdef DEBUG_BOARD_FIELD_QUOTE_TEXCOORDS
#endif
    }
    gl::enableDepthRead();
#endif
    
    
#endif
}

/*--------------------------------------------------------------------------------------------*/
//  Update
/*--------------------------------------------------------------------------------------------*/

void Board::update(){
    float t = app::getElapsedSeconds();
    
    for (vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr) {
        (*itr)->update(mOscillator,t);
    }
    for (vector<QuoteField*>::const_iterator itr = mQuoteFields.begin(); itr != mQuoteFields.end(); ++itr) {
        (*itr)->update(mOscillator,t);
    }
}

/*--------------------------------------------------------------------------------------------*/
//  Quote Handling
/*--------------------------------------------------------------------------------------------*/


void Board::setQuote(const Quote &quote, float duration){
    
}
