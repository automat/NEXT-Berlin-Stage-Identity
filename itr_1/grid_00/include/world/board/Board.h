//
//  Board.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Board_h
#define grid_00_Board_h

#include <vector>

#include "layout/geom/LayoutArea.h"

#include "world/Index.h"
#include "world/grid/Grid.h"
#include "world/Oscillator.h"
#include "world/Index.h"

#include "world/board/field/DiverField.h"


using namespace std;
using namespace ci;

class Board{
    vector<DiverField*> mDiverFields;           // diverfields
    IndexDiverFieldMap  mIndexDiverFieldMap;    // index / diverfield map
    
    Grid*       mGrid;
    Oscillator* mOscillator;
    
    
    
public:
    Board(Grid* grid, const LayoutArea& area);
    ~Board();

    void draw();
    void update();
};


#endif
