#include "world/board/Board.h"

#include <iostream>
#include <vector>

#include <boost/assign/list_inserter.hpp>
#include <boost/assert.hpp>

#include "cinder/gl/gl.h"

#include "Config.h"

using namespace std;
using namespace boost::assign;
using namespace ci;

/*--------------------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------------------*/

Board::Board(Grid* grid, const LayoutArea& area){
    
    // Create diverfields according to layoutarea
    const vector<Cell*>& gridCells = grid->getCells();
    for(vector<Cell*>::const_iterator itr = gridCells.begin(); itr != gridCells.end(); ++itr){
        const Cell* cell = *itr;
        const Vec3f& pos =cell->getCenter();
        if(area.contains(pos)){
            const Index index = cell->getIndex();
            mDiverFieldMap[index] = new DiverField(index,pos);
        }
    }
    
    
}

Board::~Board(){
    delete mOscillator;
}


/*--------------------------------------------------------------------------------------------*/
//  Draw / Update
/*--------------------------------------------------------------------------------------------*/


void Board::draw(){
#ifdef DEBUG_BOARD_FIELD_DIVER_DRAW
    gl::disableDepthRead();
    for (DiverFieldMap::const_iterator itr = mDiverFieldMap.begin(); itr != mDiverFieldMap.end(); ++itr) {
        itr->second->debugDrawArea();
    }
    gl::enableDepthRead();
#endif
    
}

void Board::update(){
    
}
