//
//  Grid.h
//  QuoteLayout
//
//  Created by Henryk Wollik on 17/03/14.
//
//

#ifndef QuoteLayout_Grid_h
#define QuoteLayout_Grid_h

#include <vector>
#include <boost/assign/std/vector.hpp>
#include <cassert>
#include "Cell.h"
#include "cinder/Vector.h"


using namespace std;
using namespace boost::assign;
using namespace ci;

class Grid {
private:
    int mNumCellsX;
    int mNumCellsY;
    vector<Cell*> mCells;
    vector<int*> mTest;
    
    // No copy
    Grid();
    Grid(const Grid& other);
    Grid& operator=(const Grid& rhs);
    
public:
    Grid(int numCellsX, int numCellsY) :
        mNumCellsX(numCellsX),mNumCellsY(numCellsY){
            int size_2 = mNumCellsX / 2;
            int i,j;
            i = -1;
            while (++i < mNumCellsX) {
                j = -1;
                while(++j < mNumCellsY){
                    mCells.push_back(new Cell(Cell::Index(j,i),Vec3f(-size_2 + j, 0, -size_2 + i)));
                }
            }
    }
    
    ~Grid(){
        while (!mCells.empty()) {
            delete mCells.back();
            mCells.pop_back();
        }
    }
    
    inline const Cell* getCell(int n) const{
        assert( n > -1 && n < mCells.size());
        return mCells[n];
    }
    
    inline Cell* getCell(int n){
        assert( n > -1 && n < mCells.size());
        return mCells[n];
    }
    
    inline const vector<Cell*>& getCells() const{
        return mCells;
    }
    /*
    
    inline vector<Cell*>* getCells(){
        return &mCells;
    }*/
    
    inline int getNumCellsX() const{
        return mNumCellsX;
    }
    
    inline int getNumCellsX(){
        return mNumCellsX;
    }
    
    inline int getNumCellsY() const{
        return mNumCellsY;
    }
    
    inline int getNumCellsY(){
        return mNumCellsY;
    }
};



#endif
