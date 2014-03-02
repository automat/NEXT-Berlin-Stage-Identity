//
//  GridCell.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridCell_h
#define grid_01_GridCell_h

#include "Settings.h"
#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"

#include "cinder/Vector.h"
#include "cinder/Matrix44.h"
#include "cinder/Rand.h"
#include "Oscillator.h"

#include "cinder/gl/Vbo.h"

#include "FrustumOrtho.h"

#include "Path.h"
#include "Diver.h"

using namespace ci;
using namespace std;

class Cell {
    Vec3f  mPos;
    Vec3f  mSize;
    int    mId[2];
   
    int            mNumDivers;
    vector<Path*>  mPaths;
    vector<Diver*> mDivers;
    
    float mDiverWidth;
    
    
    bool   mActive;
    Oscillator* mOscillator;
    float mOffset;
    
    int mDiverVerticesLen;
    int mDiverIndicesLen;
    int mVboMeshVerticesLen;
    int mVboMeshIndicesLen;
    
    gl::VboMesh::Layout mVboMeshLayout;
    gl::VboMesh         mVboMesh;
    
    void freeData(){
        while (!mPaths.empty())  delete mPaths.back(), mPaths.pop_back();
        while (!mDivers.empty()) delete mDivers.back(), mDivers.pop_back();
    }
    
    // collapse all vertices of a diver into one by setting
    // its indices to its first index
    inline void fold(Diver* diver, int index){
        
    }
    
    // reorder all indices to their vertices,
    // recreating the orignal shape
    inline void unfold(Diver* diver, int index){
        
    }
    
    
public:
    Cell(int* id,const Vec3f& pos,Oscillator* oscillator, Vec3f size = Vec3f(1,1,1)) :
        mPos(pos),
        mSize(size),
        mActive(true){
            mId[0] = id[0];
            mId[1] = id[1];
            mOffset = 0.0f;
            mOscillator = oscillator;
            
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
        mNumDivers  = Rand::randInt(CELL_MIN_NUM_DIVERS,CELL_MAX_NUM_DIVERS);
        mDiverWidth = 1.0f / (float)mNumDivers;
        
        freeData();

        
        Vec3f start,end;
        float marginX = -0.5f + mDiverWidth * 0.5f;
        
        int i = -1;
        while (++i < mNumDivers) {
            Vec3f end(marginX + float(i) / float(mNumDivers), 0.0, -0.5f);
            Vec3f start(end.x, end.y, 0.5f);
            mPaths.push_back(new Path(start, end, mDiverWidth));
            mDivers.push_back(new Diver(mPaths.back(),Rand::randFloat(CELL_DIVER_MIN_HEIGHT,CELL_DIVER_MAX_HEIGHT)));
        }
       
        int diverSides      = 1;
        mDiverVerticesLen   = CELL_DIVER_NUM_POINTS * 2 * diverSides; //top l/r, bottom l/r
        mDiverIndicesLen    = (mDiverVerticesLen / 2 - 1) * 6;
        mVboMeshVerticesLen = mDiverVerticesLen * mNumDivers;
        mVboMeshIndicesLen  = mDiverIndicesLen  * mNumDivers;
        
        mVboMesh.reset();
        mVboMesh = gl::VboMesh(mVboMeshVerticesLen,mVboMeshIndicesLen,mVboMeshLayout,GL_TRIANGLES);
        
        vector<uint32_t> indices;
     
        int j;
        int v0,v1,v2,v3;
        int index;
        i = -1;
        while (++i < mNumDivers) {
            j = -1;
            while (++j < mDiverVerticesLen - 2) {
                index = i * mDiverVerticesLen + j;
                indices.push_back(index);
                indices.push_back(index+2);
                indices.push_back(index+1);
            }
        }
        
        /*
        i = -1;
        while (++i < mVboMeshVerticesLen - 2) {
            v0 = i;
            v1 = i+2;
            v2 = i+1;
            v3 = i+3;
            
            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);
            
            //indices.push_back(v3);
            //indices.push_back(v2);
            //indices.push_back(v1);
            
            cout
            << indices[indices.size() - 3] << ", "
            << indices[indices.size() - 2] << ", "
            << indices[indices.size() - 1]
            << endl;

            //indices.push_back(i);

        }
         */
        
        //cout << indices.size() << " / " << mDiverIndicesLen << endl;
        
        /*
        cout
        << indices[indices.size() - 6] << ", "
        << indices[indices.size() - 5] << ", "
        << indices[indices.size() - 4]
        << endl;
        
        cout
        << indices[indices.size() - 3] << ", "
        << indices[indices.size() - 2] << ", "
        << indices[indices.size() - 1]
        << endl;
        */
        
        /*
        i = -1;
        while (++i < mVboMeshVerticesLen) {
            indices.push_back(i);
        }*/
        
        mVboMesh.bufferIndices(indices);
        
        //cout << indices.size() << endl;
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
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, mPos.z);
        
#ifdef CELL_DEBUG_DRAW_DIVER
        for (vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            (*itr)->debugDraw();
        }
#endif
#ifdef CELL_DEBUG_DRAW_PATH
        for (vector<Path*>::const_iterator itr = mPaths.begin(); itr != mPaths.end(); ++itr) {
            (*itr)->debugDraw();
        }
#endif
        glPopMatrix();
    }
    
    inline void draw(){
        if(!mActive){
            return;
        }
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, mPos.z);
        float prevPointSize;
        glGetFloatv(GL_CURRENT_POINT_SIZE_APPLE,&prevPointSize);
        glPointSize(3.0f);
        glColor3f(1, 0, 1);
        gl::draw(mVboMesh);
        glPointSize(prevPointSize);
        
        glPopMatrix();
    }
    
    inline void updatePaths(){
        float scale  = 0.75f;
        mOffset     += 0.01f;
        for(vector<Path*>::const_iterator itr = mPaths.begin(); itr != mPaths.end(); ++itr){
            for(vector<Vec3f>::iterator _itr = (*itr)->getPoints().begin(); _itr != (*itr)->getPoints().end(); _itr++){
                _itr->y = mOscillator->getValue(mPos.x + _itr->x,
                                                mPos.z + _itr->z + mOffset,0,
                                                mId[0], mId[1]) * scale;
            }
        }
    }
    
    inline void updateDivers(){
        float t = app::getElapsedSeconds();
        for (vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            (*itr)->update(t);
        }
        
    }
    
    inline void update(float t){
        if(!mActive){
            return;
        }
        
        // update geometry
        float diverWidth_2 = mDiverWidth * 0.5f;
        float diverHeight_2;
        float x0,x1,y0,y1;
        int i;

        gl::VboMesh::VertexIter vbItr = mVboMesh.mapVertexBuffer();
        for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            diverHeight_2 = (*itr)->getHeight() * 0.5f;
            const vector<Vec3f>& points = (*itr)->getPoints();
       
            i = -1;
            while(++i < points.size()){
                const Vec3f& point = points[i];
                x0 = point.x - diverWidth_2;
                x1 = point.x + diverWidth_2;
                y0 = point.y - diverHeight_2;
                y1 = point.y + diverHeight_2;
       
                vbItr.setPosition(x0, y0, point.z);
                ++vbItr;
                vbItr.setPosition(x1, y0, point.z);
                ++vbItr;
                /*
                continue;
                vbItr.setPosition(x0, y1, point.z);
                ++vbItr;
                vbItr.setPosition(x1, y1, point.z);
                ++vbItr;
                 */
            }
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
