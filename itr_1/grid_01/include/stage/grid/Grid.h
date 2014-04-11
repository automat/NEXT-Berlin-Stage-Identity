//
//  Grid.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Grid_h
#define grid_00_Grid_h

#include <vector>

#include "cinder/Vector.h"
#include "cinder/Camera.h"

#include "stage/grid/Cell.h"
#include "stage/grid/Index.h"

using namespace std;
using namespace ci;

class Grid {
private:
    Vec2f mSize;
    vector<Cell*> mCells;
    vector<int*> mTest;
    
    // No copy
    Grid();
    Grid(const Grid& other);
    Grid& operator=(const Grid& rhs);
    
    /*--------------------------------------------------------------------------------------------*/
    
public:
    Grid(int numCellsX, int numCellsY);
    
    ~Grid();
    
    const Cell* getCell(int n) const;
    Cell* getCell(int n);
    Cell* getCell(int x, int y);
    Cell* getCell(const Index& index);
    
    inline const vector<Cell*>& getCells() const{
        return mCells;
    }
    
    inline int getNumCellsX() const{
        return mSize.x;
    }
    
    inline int getNumCellsX(){
        return mSize.x;
    }
    
    inline int getNumCellsY() const{
        return mSize.y;
    }
    
    inline int getNumCellsY(){
        return mSize.y;
    }
    
    void debugDrawIndices(const CameraOrtho& camera);
};




#endif
