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
#include "cinder/Frustum.h"
#include "FrustumOrtho.h"
#include <vector>
#include "cinder/Rect.h"
#include "cinder/Matrix44.h"

#include "Cell.h"

using namespace std;
using namespace cinder;

class Controller {
    int mSizeX;
    int mSizeY;
    
    Matrix44f mTransform;
    Rectf     mBounds;
    
    vector<Cell*> mCells;
    
public:
    Controller(int sizeX = 3, int sizeY = 3) :
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
                mCells.push_back(new Cell(Vec3f(-sizeX_2 + j,0,-sizeY_2 + i), // pos
                                              Vec3f(1,1,1),                       // vel
                                              CellId(i,j)));
            }
        }
        
        mBounds.x1 = -sizeX_2 - 0.5f;
        mBounds.y1 = -sizeX_2 - 0.5f;
        mBounds.x2 = mBounds.x1 + sizeX;
        mBounds.y2 = mBounds.y1 + sizeY;
    }
    
    
    //! draw boundaries and diver path
    inline void debugArea(){
        glPushMatrix();
        glMultMatrixf(&mTransform[0]);
        glLineWidth(3);
        
        float vertices[] = {
            mBounds.x1,0,mBounds.y1,
            mBounds.x2,0,mBounds.y1,
            mBounds.x2,0,mBounds.y2,
            mBounds.x1,0,mBounds.y2
        };
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        
        glColor3f(1, 0, 1);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        
        glLineWidth(1);
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->debugArea();
        }
        glPopMatrix();
    }
    
    
    
    
    inline void draw(){
        glPushMatrix();
        glMultMatrixf(&mTransform[0]);
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->debugDraw();
        }
        glPopMatrix();
    }
    
    inline void update(){
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->update();
        }
    }
    
    inline void checkFrustum(const FrustumOrtho& frustum){
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->checkFrustum(frustum,mTransform);
        }
    }
    
    inline void transform(float scale){
        mTransform = mTransform.identity();
        mTransform.scale(scale);
    }
};



#endif