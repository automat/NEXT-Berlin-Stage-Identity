//
//  GridController.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridController_h
#define grid_01_GridController_h

#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>

#include <vector>

#include "GridCell.h"

using namespace std;
using namespace cinder;

class GridController {
    int mSizeX;
    int mSizeY;
    
    vector<GridCell*> mCells;
    
    
    
public:
    GridController(int sizeX = 3, int sizeY = 3) :
    mSizeX(sizeX),
    mSizeY(sizeY){
        
        int sizeX_2 = mSizeX / 2;
        int sizeY_2 = mSizeY / 2;
        //  init
        int i,j;
        i = -1;
        while (++i < mSizeY) {
            j = -1;
            while (++j < mSizeX) {
                mCells.push_back(new GridCell(Vec3f(-sizeX_2 + j,0,-sizeY_2 + i), //pos
                                              Vec3f(1,1,1),
                                              GridCellId(i,j)));
            }
        }
        
        
    }
    
    inline void draw(){
        for(vector<GridCell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->debugDraw();
        }
        
    }
    
    inline void update(){
        for(vector<GridCell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->update();
        }
    }
};



#endif
