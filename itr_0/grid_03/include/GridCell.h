//
//  GridCell.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridCell_h
#define grid_01_GridCell_h

#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>

#include "cinder/Vector.h"
#include "cinder/Matrix44.h"
#include "GridDiverPath.h"
#include "GridDiver.h"
#include "cinder/Rand.h"

#include "cinder/Perlin.h"
#include "cinder/Frustum.h"
#include "FrustumOrtho.h"

using namespace ci;
using namespace std;

struct GridCellId{
    int x;
    int y;
    GridCellId(int x, int y):x(x),y(y){};
};

class GridCell {
    Vec3f      mPos;
    Matrix44f  mModelViewMatrix;
    Vec3f      mSize;
    int        mNumDivers;
    GridCellId mId;
    bool       mActive;
    Perlin     mPerlin;
    
    vector<GridDiverPath*>  mPaths;
    vector<GridDiver*> mDivers;
    
    
public:
    GridCell(Vec3f pos, Vec3f size, GridCellId id) :
    mPos(pos),
    mSize(size),
    mId(id),
    mActive(true){
        
        mPerlin = Perlin(2,clock() & 65535);
        this->reset();
    }
    
    inline void reset(){
        mNumDivers = Rand::randInt(2,30);
        
        // free prev divers
        while (!mPaths.empty()) delete mPaths.back(), mPaths.pop_back();
        while (!mDivers.empty()) delete mDivers.back(), mDivers.pop_back();

        float marginX = -0.5f + 1.0f / mNumDivers * 0.5f; 
        Vec3f start,end;
        int i = -1;
        while (++i < mNumDivers) {
            Vec3f end(marginX + float(i) / float(mNumDivers), 0.0, -0.5f);
            Vec3f start(end.x, end.y, 0.5f);
            mDivers.push_back(new GridDiver(start,end));
            mPaths.push_back(new GridDiverPath(start, end));
        }
        
        
        
    }
    
    inline void debugArea(){
        static const float unitPoints[]  = {
            -0.5,0,-0.5,
            0.5,0,-0.5,
            0.5,0, 0.5,
            -0.5,0, 0.5
        };
        glColor3f(1,0,0);
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, mPos.z);
        
        //glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewMatrix);
        
        
        gl::enableAlphaBlending();
        
        gl::disableAlphaBlending();
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &unitPoints[0]);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    
        if(mActive){
            glColor3f(1, 1, 1);
        } else {
            glColor3f(1, 0, 0);
        }
        static const Vec3f zero(0,0,0);
        
        glPointSize(5);
        glVertexPointer(3, GL_FLOAT, 0, &zero.x);
        glDrawArrays(GL_POINTS, 0, 1);
        glPointSize(1);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        glPopMatrix();
    }
    
    inline void debugDraw(){
        if(!mActive){
            
        }
        glColor3f(1, 0, 0);
        
        static const float unitPoints[]  = {
            -0.5,0,-0.5,
             0.5,0,-0.5,
             0.5,0, 0.5,
            -0.5,0, 0.5
        };
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, mPos.z);
        
        
        /*
        gl::disableAlphaBlending();
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &unitPoints[0]);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        */
        
        for (vector<GridDiver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            //(*itr)->debugDraw();
        }
        
        for (vector<GridDiverPath*>::const_iterator itr = mPaths.begin(); itr != mPaths.end(); ++itr) {
            (*itr)->debugDraw();
        }
        
        glPopMatrix();
    }
    
    inline void draw(){
        
    }
    
    inline void update(){
        if(!mActive){
            return;
        }
        /*
        for(vector<GridDiver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
            GridDiver* diver = *itr;
            
            const Vec3f& pos = diver->getPos();
            diver->setVel(mPerlin.fBm(pos.x,pos.z,app::getElapsedSeconds()) * 1.0f);
            diver->update();
        }
         */
        float time = app::getElapsedSeconds() * 0.125f;
        for(vector<GridDiverPath*>::const_iterator itr = mPaths.begin(); itr != mPaths.end(); ++itr){
            for(vector<Vec3f>::iterator _itr = (*itr)->getPoints().begin(); _itr != (*itr)->getPoints().end(); _itr++){
                _itr->y = mPerlin.fBm(_itr->x,_itr->z,time) * 0.5f;
                
            }
            
        }
    }
    
    inline void checkFrustum(const FrustumOrtho& frustum,const Matrix44f& transform){
        static const Vec3f zero;
        mActive = frustum.contains(transform.transformPointAffine(mPos));
        
        
        ///cout << centerTransformed << endl;
        
    }
    
    
    inline void activate(){
        mActive = true;
    }
    
    inline void deactivate(){
        mActive = false;
    }
};



#endif
