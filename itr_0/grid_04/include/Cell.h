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
    
    int mDiverVerticesTubeLen;
    int mDiverVerticesCapLen;
    int mDiverVerticesLen;
    int mDiverIndicesLen;
    int mVboMeshVerticesLen;
    int mVboMeshIndicesLen;
    
    gl::VboMesh::Layout mVboMeshLayout;
    gl::VboMesh         mVboMesh;
    
#ifdef CELL_DEBUG_DRAW_MESH_INDICES
    //  need to copy all vertices, strange bug with mapVertexBuffer followed by gl draw calls
    vector<Vec3f> mVboMeshVerticesCopy;
#endif
    
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
        int j = 0;
        
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
            mVboMeshLayout.setStaticColorsRGB();
            
            //mVboMeshLayout.setDynamicTexCoords2d();
            //mVboMeshLayout.setDynamicNormals();
            //mVboMeshLayout.setStaticColorsRGB();
            
            this->reset();
    }
    
    ~Cell(){
        freeData();
    }
    
    inline Colorf toColor(const Vec3f& normal){
        return Colorf(normal.x * 0.5f + 0.5f,normal.y * 0.5f + 0.5f,normal.z * 0.5f + 0.5f);
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
       
     
        mDiverVerticesCapLen  = 0; // just end, we cant see the front at all
        mDiverVerticesTubeLen = (CELL_DIVER_NUM_POINTS * 4) * 2; //(top , bottom , left, right ) * 2 ,
        mDiverVerticesLen     = mDiverVerticesTubeLen + mDiverVerticesCapLen;
        mDiverIndicesLen      = (CELL_DIVER_NUM_POINTS - 1) * 6  * 2 * 2 + (mDiverVerticesCapLen/2 * 3);// + ((mDiverVerticesCapLen / 2 - 1) * 3);
        mVboMeshVerticesLen   = (mDiverVerticesTubeLen + mDiverVerticesCapLen) * mNumDivers;
        mVboMeshIndicesLen    = mDiverIndicesLen  * mNumDivers;
        
        mVboMesh.reset();
        mVboMesh = gl::VboMesh(mVboMeshVerticesLen,mVboMeshIndicesLen,mVboMeshLayout,GL_TRIANGLES);

        
        vector<uint32_t> indices; // buffer target
        vector<Colorf>   colors;  // buffer colors for debug
     
        int j;
        int v00,v01,v02,v03;
        int v04,v05,v06,v07;
        int v08,v09,v10,v11;
        int v12,v13,v14,v15;
        int index;
        int uniqueVerticesLen = mDiverVerticesTubeLen / 2;
        
        colors.resize(0);
        
        
        
        
        
        i = -1;
        while (++i < mNumDivers) {
            j = 0;
            while (j < CELL_DIVER_NUM_POINTS) {
                
                static const Vec3f up(0,1,0);
                static const Vec3f down(0,-1,0);
                static const Vec3f left(-1,0,0);
                static const Vec3f right(1,0,0);
                static const Vec3f multiplier(0.5f,0.5f,0.5f);
                
                
                colors.push_back(toColor(down));
                colors.push_back(toColor(down));
                colors.push_back(toColor(up));
                colors.push_back(toColor(up));
                colors.push_back(toColor(left));
                colors.push_back(toColor(left));
                colors.push_back(toColor(right));
                colors.push_back(toColor(right));
                
                //colors.push_back(Colorf(0,1,0));
                //colors.push_back(Colorf(0,1,0));
                //colors.push_back(Colorf(0,0.5,0));
                //colors.push_back(Colorf(0,0.5,0));
                
                //colors.push_back(Colorf(1,0,0));
                //colors.push_back(Colorf(1,0,0));
                ++j;
                //cout << j << endl;
            }
            
            j = 0;
            while (j < uniqueVerticesLen - 4) {
                index = (i * uniqueVerticesLen + j) * 2;
        
                // bottom -> j / top ->j(next step)
                // the quads are verticaly sliced
                
                // first quad
                v00 = index + 0;
                v01 = index + 2;
                v02 = index + 3;
                v03 = index + 1;
                
                // next quad
                v04 = index + 8;
                v05 = index + 10;
                v06 = index + 11;
                v07 = index + 9;
                
                // first quad duplicate
                v08 = index + 4;
                v09 = index + 6;
                v10 = index + 7;
                v11 = index + 5;
                
                // next quad duplicate
                v12 = index + 12;
                v13 = index + 14;
                v14 = index + 15;
                v15 = index + 13;
                
#ifdef CELL_DIVER_DRAW_BOTTOM
                // bottom lower triangle
                indices.push_back(v00);
                indices.push_back(v03);
                indices.push_back(v04);
                 // bottom upper triangle
                indices.push_back(v03);
                indices.push_back(v07);
                indices.push_back(v04);
#endif
#ifdef CELL_DIVER_DRAW_TOP
                // top lower triangle
                indices.push_back(v01);
                indices.push_back(v02);
                indices.push_back(v05);
                // bottom upper triangle
                indices.push_back(v02);
                indices.push_back(v06);
                indices.push_back(v05);
#endif
#ifdef CELL_DIVER_DRAW_RIGHT
                // right lower triangle
                indices.push_back(v08);
                indices.push_back(v09);
                indices.push_back(v12);
                // right upper triangle
                indices.push_back(v09);
                indices.push_back(v13);
                indices.push_back(v12);
#endif
#ifdef CELL_DIVER_DRAW_LEFT
                //left lower triangle
                indices.push_back(v11);
                indices.push_back(v10);
                indices.push_back(v15);
                 //left upper triangle
                indices.push_back(v10);
                indices.push_back(v14);
                indices.push_back(v15);
#endif
                
                
                
                
                
                
                
                
                /*
                indices.push_back(v4);
                indices.push_back(v5);
                indices.push_back(v7);
                
                indices.push_back(v5);
                indices.push_back(v6);
                indices.push_back(v7);
                */
                /*
                // side left
                indices.push_back(v1);
                indices.push_back(v5);
                indices.push_back(v2);
                
                indices.push_back(v5);
                indices.push_back(v6);
                indices.push_back(v2);
                
                //side right
                indices.push_back(v0);
                indices.push_back(v4);
                indices.push_back(v3);
                
                indices.push_back(v4);
                indices.push_back(v7);
                indices.push_back(v3);
                 */
                
                j+=4;
            }
            /*
            if(mDiverVerticesCapLen == 0)continue;
            
            index = (mDiverVerticesTubeLen * (i+1)) + (mDiverVerticesCapLen * i);
            
            v0 = index;
            v1 = v0 + 1;
            v2 = v0 + 2;
            v3 = v2 + 1;
            
            v4 = v0 + 4;
            v5 = v4 + 1;
            v6 = v4 + 2;
            v7 = v6 + 1;
            
            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);
            
            indices.push_back(v2);
            indices.push_back(v3);
            indices.push_back(v1);
             */
        }
        
        //cout << "Buffer colors: " << colors.size() << " " << mVboMesh.getNumVertices() <<endl;
        
        mVboMesh.bufferIndices(indices);
        mVboMesh.bufferColorsRGB(colors);
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
        glColor3f(1, 0, 1);
        gl::draw(mVboMesh);
        glPopMatrix();
    }
    
    inline void updatePaths(){
        if(!mActive){
            return;
        }
        
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
        if(!mActive){
            return;
        }
        
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
                
                // bottom
                vbItr.setPosition(x0, y0, point.z);
                ++vbItr;
                
                vbItr.setPosition(x1, y0, point.z);
                ++vbItr;
                
                // top
                vbItr.setPosition(x0, y1, point.z);
                ++vbItr;
                
                vbItr.setPosition(x1, y1, point.z);
                ++vbItr;
                
                // copy top for left / right
                vbItr.setPosition(x0, y0, point.z);
                ++vbItr;
                
                vbItr.setPosition(x1, y0, point.z);
                ++vbItr;
                
                // copy bottom for left / right
                vbItr.setPosition(x0, y1, point.z);
                ++vbItr;
                
                vbItr.setPosition(x1, y1, point.z);
                ++vbItr;
                
                int pos = vbItr.getIndex();

            }
            
            if(mDiverVerticesCapLen == 0) continue;
           
            const Vec3f& end = points[points.size() - 1];
            
            // end
            x0 = end.x - diverWidth_2;
            x1 = end.x + diverWidth_2;
            y0 = end.y - diverHeight_2;
            y1 = end.y + diverHeight_2;
            
            vbItr.setPosition(0,0,0);
            ++vbItr;
            
            vbItr.setPosition(0,0,0);
            ++vbItr;
            
            vbItr.setPosition(0,0,0);
            ++vbItr;
            
            vbItr.setPosition(0,0,0);
            ++vbItr;
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
