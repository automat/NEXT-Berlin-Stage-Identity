#include "world/board/Board.h"

#include <iostream>
#include <vector>

#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assert.hpp>

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Resources.h"
#include "Config.h"
#include "util/ShaderUtil.h"
#include "layout/quote/QuoteAlign.h"

using namespace std;
using namespace boost::assign;
using namespace ci;

/*--------------------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------------------*/

Board::Board(Grid* grid, const LayoutArea& area, Oscillator* oscillator, vector<Quote>* quotes) :
    mGrid(grid),
    mArea(area),
    mOscillator(oscillator),
    mQuotes(quotes){

    const vector<Cell*>& gridCells = mGrid->getCells();
    // Create diverfields according to layoutarea
    for(vector<Cell*>::const_iterator itr = gridCells.begin(); itr != gridCells.end(); ++itr){
        const Cell* cell = *itr;
        const Vec3f& pos =cell->getCenter();
        if(area.contains(pos)){
            //const Index& index = cell->getIndex();
            mDiverFields += new DiverField(pos,Rand::randInt(DIVER_FIELD_NUM_DIVERS_MIN, DIVER_FIELD_NUM_DIVERS_MAX));
            //mIndexDiverFieldMap[index] = mDiverFields.back();
        }
    }
    
    setQuote((*mQuotes)[0]);
    
#ifdef BOARD_LIVE_EDIT_MATERIAL_SHADER
    mSharedFileWatcher = SharedFileWatcher::Get();
    utils::loadShader(loadFile(RES_ABS_GLSL_BOARD_QUOTE_FIELD_VERT),
                      loadFile(RES_ABS_GLSL_BOARD_QUOTE_FIELD_FRAG),
                      &mShaderQuoteFields);
#else
    utils::loadShader(LoadResource(RES_GLSL_BOARD_QUOTE_FIELD_VERT),
                      LoadResource(RES_GLSL_BOARD_QUOTE_FIELD_FRAG),
                      &mShaderQuoteFields);
#endif

    
}

/*--------------------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------------------*/

void Board::deleteDiverFields(){
    //mIndexDiverFieldMap.clear();
    while (!mDiverFields.empty()) delete mDiverFields.back(), mDiverFields.pop_back();
}

void Board::deleteQuoteFields(){
    //mIndexQuoteFieldMap.clear();
    while (!mQuoteFields.empty()) delete mQuoteFields.back(), mQuoteFields.pop_back();
}

Board::~Board(){
    deleteDiverFields();
    deleteQuoteFields();
}

/*--------------------------------------------------------------------------------------------*/
// Draw / Update
/*--------------------------------------------------------------------------------------------*/

void Board::draw(const CameraOrtho& camera, bool useMaterialShaders){
#ifndef BOARD_SKIP_DRAW_FIELD_DIVER
    for(vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr){
#ifdef DEBUG_BOARD_FIELD_DIVER
        (*itr)->debugDrawArea();
        (*itr)->debugDrawPathSurface();
        (*itr)->debugDrawDivers();
#endif
        (*itr)->draw();
    }
#endif
    
#ifndef BOARD_SKIP_DRAW_QUOTE_DIVER
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0.0, 0.9);
    if(useMaterialShaders){
        mShaderQuoteFields.bind();
        mQuoteCurrent->getTexture().bind();
        mShaderQuoteFields.uniform("uTexture", 0);
    }
    for(vector<QuoteField*>::const_iterator itr = mQuoteFields.begin(); itr != mQuoteFields.end(); ++itr){
#ifdef DEBUG_BOARD_FIELD_QUOTE
        (*itr)->debugDrawArea();
        (*itr)->debugDrawPathSurface();
        (*itr)->debugDrawDivers();
#endif
#ifdef DEBUG_BOARD_FIELD_QUOTE_TEXCOORDS
        (*itr)->debugDrawDiverIndices(camera);
#endif
        (*itr)->draw();
    }
    if(useMaterialShaders){
        mQuoteCurrent->getTexture().unbind();
        mShaderQuoteFields.unbind();
    }
    glDisable(GL_POLYGON_OFFSET_FILL);
    
#endif
}

void Board::update(){
#ifdef BOARD_LIVE_EDIT_MATERIAL_SHADER
    utils::watchShaderSource(mSharedFileWatcher,
                             loadFile(RES_ABS_GLSL_BOARD_QUOTE_FIELD_VERT),
                             loadFile(RES_ABS_GLSL_BOARD_QUOTE_FIELD_FRAG),
                             &mShaderQuoteFields);
#endif
    float t = app::getElapsedSeconds();
#ifndef BOARD_SKIP_DRAW_FIELD_DIVER
    for (vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr) {
        (*itr)->update(mOscillator,t);
    }
#endif
#ifndef BOARD_SKIP_DRAW_FIELD_QUOTE
    for (vector<QuoteField*>::const_iterator itr = mQuoteFields.begin(); itr != mQuoteFields.end(); ++itr) {
        (*itr)->update(mOscillator,t);
    }
#endif
}

/*--------------------------------------------------------------------------------------------*/
// Quote Handling
/*--------------------------------------------------------------------------------------------*/

void Board::setQuote(Quote& quote){
    deleteQuoteFields();
    
    const vector<QuoteLine>& lines = quote.getLines();
    for(vector<QuoteLine>::const_iterator itr = lines.begin(); itr != lines.end(); ++itr){
        mQuoteFields += new QuoteField( mGrid->getCell(itr->getIndices().front())->getCenter(),
                                       Rand::randInt(QUOTE_FIELD_NUM_DIVERS_MIN, QUOTE_FIELD_NUM_DIVERS_MAX),
                                       *itr );
    }
    
    mQuoteCurrent = &quote;
}