//
//  GridCell.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridCell_h
#define grid_01_GridCell_h

#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"

#include "cinder/Vector.h"
#include "cinder/Matrix44.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"

#include "cinder/gl/Vbo.h"

#include "FrustumOrtho.h"

#include "Path.h"
#include "Diver.h"

using namespace ci;
using namespace std;

class Cell {
    const int MIN_NUM_DIVERS   = 1; // header lazyness
    const int MAX_NUM_DIVERS   = 1;
    const int DIVER_NUM_POINTS = 10;

    
    Vec3f  mPos;
    Vec3f  mSize;
    int    mId[2];
   
    int            mNumDivers;
    vector<Path*>  mPaths;
    vector<Diver*> mDivers;
    
    bool   mActive;
    Perlin mPerlin;
    
    gl::VboMesh::Layout mVboMeshLayout;
    gl::VboMesh         mVboMesh;
    
    void freeData(){
        while (!mPaths.empty())  delete mPaths.back(), mPaths.pop_back();
        while (!mDivers.empty()) delete mDivers.back(), mDivers.pop_back();
        
    }
    
    
public:
    Cell(int* id,const Vec3f& pos, Vec3f size = Vec3f(1,1,1)) :
        mPos(pos),
        mSize(size),
        mActive(true){
            mId[0] = id[0];
            mId[1] = id[1];
            
            mPerlin = Perlin(2,clock() & 65535);
            
            mVboMeshLayout.setDynamicPositions();
            mVboMeshLayout.setStaticIndices();
            //mVboMeshLayout.setDynamicTexCoords2d();
            //mVboMeshLayout.setDynamicNormals();
            //mVboMeshLayout.setStaticColorsRGB();
            
            this->reset();
    }
    
    ~Cell(){
        freeData();
    }
    
    inline void reset(){
        mNumDivers = Rand::randInt(MIN_NUM_DIVERS,MAX_NUM_DIVERS);
        
        freeData();

        Vec3f start,end;
        float marginX = -0.5f + 1.0f / mNumDivers * 0.5f;
        float width   = 1.0f / (float)mNumDivers;
        
        int i = -1;
        while (++i < mNumDivers) {
            Vec3f end(marginX + float(i) / float(mNumDivers), 0.0, -0.5f);
            Vec3f start(end.x, end.y, 0.5f);
            mPaths.push_back(new Path(start, end, width));
            mDivers.push_back(new Diver(mPaths.back()));
        }
        
        int diverVerticesLen = DIVER_NUM_POINTS * 2;
        int diverIndicesLen  = (diverVerticesLen / 2 - 2) * 3;
        int meshVerticesLen  = diverVerticesLen * mNumDivers;
        int meshIndicesLen   = diverIndicesLen * mNumDivers;
        
        mVboMesh.reset();
        mVboMesh = gl::VboMesh(meshVerticesLen,meshIndicesLen,mVboMeshLayout,GL_POINTS);
        
        vector<uint32_t> indices;
        
        /*
        int j;
        i = -1;
        while(++i < mNumDivers){
            j = -1;
            while (++j < DIVER_NUM_POINTS - 1) {
                indices.push_back(0);
                indices.push_back(0);
                indices.push_back(0);
            
                indices.push_back(0);
                indices.push_back(0);
                indices.push_back(0);
            }
        }
         */
        i = -1;
        while (++i < meshVerticesLen) {
            indices.push_back(i);
        }
        
        mVboMesh.bufferIndices(indices);
    }
    
    inline void debugArea(){
        static const float unitPoints[] = {-0.5,0,-0.5,0.5,0,-0.5,0.5,0, 0.5,-0.5,0, 0.5};
        
        glColor3f(1,0,0);
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, mPos.z);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(6, 0xAAAA);
        glVertexPointer(3, GL_FLOAT, 0, &unitPoints[0]);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisable(GL_LINE_STIPPLE);
        
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
            return;
        }
        
        glColor3f(1, 0, 0);
        
        static const float unitPoints[] = {-0.5,0,-0.5,0.5,0,-0.5,0.5,0, 0.5,-0.5,0, 0.5};
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, mPos.z);
        
        
        /*
        gl::disableAlphaBlending();
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &unitPoints[0]);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        */
        
        gl::VboMesh::VertexIter itr = mVboMesh.mapVertexBuffer();
        int i = -1;
        while (i < mVboMesh.getNumVertices()) {
            ++itr;
            i += 2;
        }
        
        glPointSize(15.0f);
        gl::draw(mVboMesh);
        
        gl::VboMesh::VertexIter vItr = mVboMesh.mapVertexBuffer();
        for (vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            (*itr)->debugDraw();
        }
        
        for (vector<Path*>::const_iterator itr = mPaths.begin(); itr != mPaths.end(); ++itr) {
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
 
        float time = app::getElapsedSeconds();
        float time0125 = time * 0.125f;
        float scale    = 0.75f;
        
        for(vector<Path*>::const_iterator itr = mPaths.begin(); itr != mPaths.end(); ++itr){
            for(vector<Vec3f>::iterator _itr = (*itr)->getPoints().begin(); _itr != (*itr)->getPoints().end(); _itr++){
                _itr->y = mPerlin.fBm(_itr->x,_itr->z,time0125) * scale;
            }
        }
        
        for (vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            (*itr)->update(time);
        }
    }
    
    inline void checkFrustum(const FrustumOrtho& frustum,const Matrix44f& transform){
        mActive = frustum.contains(transform.transformPointAffine(mPos));
    }
    
    inline void activate(){
        mActive = true;
    }
    
    inline void deactivate(){
        mActive = false;
    }
};



#endif
