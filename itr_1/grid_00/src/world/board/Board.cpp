#include "world/board/Board.h"

#include <iostream>
#include <vector>

#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assert.hpp>

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Config.h"

using namespace std;
using namespace boost::assign;
using namespace ci;

/*--------------------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------------------*/

Board::Board(Grid* grid, const LayoutArea& area){
    mOscillator = new Oscillator();
    const vector<Cell*>& gridCells = grid->getCells();
#ifdef DEBUG_SINGLE_DIVER_FIELD
    /*
    Cell* cell = gridCells[84];
    mDiverFields += new DiverField(cell->getCenter(),10);
    mIndexDiverFieldMap[cell->getIndex()] = mDiverFields.back();
    */
    Cell* cell = gridCells[84];
    mDiverFields += new DiverField(cell->getCenter(),40);
    mIndexDiverFieldMap[cell->getIndex()] = mDiverFields.back();
    
#else
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
#endif
}

/*--------------------------------------------------------------------------------------------*/
// Destructor
/*--------------------------------------------------------------------------------------------*/


Board::~Board(){
    mIndexDiverFieldMap.clear();
    while (!mDiverFields.empty()) delete mDiverFields.back(), mDiverFields.pop_back();
    
    delete mOscillator;
}


/*--------------------------------------------------------------------------------------------*/
//  Draw / Update
/*--------------------------------------------------------------------------------------------*/


void Board::draw(){
#if defined(DEBUG_BOARD_FIELD_DIVER_AREA_DRAW) || \
    defined(DEBUG_BOARD_FIELD_DIVER_PATH_SURFACE_DRAW) || \
    defined(DEBUG_BOARD_FIELD_DIVER_DIVER)
    gl::disableDepthRead();
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
    gl::enableDepthRead();
#endif
    for(vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr){
        (*itr)->draw();
    }
}

void Board::update(){
    float t = app::getElapsedSeconds();
    
    for (vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr) {
        (*itr)->update(mOscillator,t);
    }
    
}
