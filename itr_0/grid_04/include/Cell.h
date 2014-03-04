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
    /*--------------------------------------------------------------------------------------------*/
    // Member
    /*--------------------------------------------------------------------------------------------*/

    Vec3f  mPos;
    Vec3f  mSize;
    int    mId[2];
   
    int             mNumDivers;
    vector<Path*>   mPaths;
    vector<Diver*>  mDivers;
    
    
    vector<uint32_t> mVboMeshFoldIndices;
    vector<uint32_t> mVboMeshUnfoldIndices;
    vector<uint32_t> mVboMeshBufferIndices;
    
    vector<Vec3f>  mPathData;
    
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
    
    //
    // free all pointer data
    //
    void freeData(){
        while (!mPaths.empty())  delete mPaths.back(), mPaths.pop_back();
        while (!mDivers.empty()) delete mDivers.back(), mDivers.pop_back();
    }
    
    inline Colorf toColor(const Vec3f& normal){
        return Colorf(normal.x * 0.5f + 0.5f,normal.y * 0.5f + 0.5f,normal.z * 0.5f + 0.5f);
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Fold / Unfold
    /*--------------------------------------------------------------------------------------------*/

    //
    // collapse all vertices of a diver into one vertex by setting
    // all its indices to its first index
    //
    inline void fold(int index){
        index = index * mDiverIndicesLen;
        size_t offset = sizeof(uint32_t) * index;
        size_t size   = sizeof(uint32_t) * mDiverIndicesLen;
        
        gl::Vbo& indexBuffer = mVboMesh.getIndexVbo();
        indexBuffer.bind();
        indexBuffer.bufferSubData(offset, size, &mVboMeshFoldIndices[0]);
        indexBuffer.unbind();
    }
    
    //
    // reorder all indices to their vertices,
    // recreating the orignal shape
    //
    inline void unfold(int index){
        int vertexIndex = index * mDiverVerticesLen;
        int i = -1;
        while(++i < mDiverIndicesLen){
            mVboMeshBufferIndices[i] = mVboMeshUnfoldIndices[i] + vertexIndex;
        }
        
        size_t offset = sizeof(uint32_t) * vertexIndex;
        size_t size   = sizeof(uint32_t) * mDiverIndicesLen;
        
        gl::Vbo& indexBuffer = mVboMesh.getIndexVbo();
        indexBuffer.bind();
        indexBuffer.bufferSubData(offset, size, &mVboMeshBufferIndices[0]);
        indexBuffer.unbind();
    }
    
    
public:
    
    /*--------------------------------------------------------------------------------------------*/
    // Constructor / Destructor
    /*--------------------------------------------------------------------------------------------*/

    
    Cell(int* id,const Vec3f& pos,Oscillator* oscillator, Vec3f size = Vec3f(1,1,1)) :
        mPos(pos),
        mOscillator(oscillator),
        mSize(size),
        mActive(true),
        mOffset(0.0f){
            mId[0] = id[0];
            mId[1] = id[1];
            
            mVboMeshLayout.setDynamicPositions();
            mVboMeshLayout.setDynamicIndices();
            mVboMeshLayout.setStaticColorsRGB();

            this->reset();
    }
    
    ~Cell(){
        freeData();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Geom init
    /*--------------------------------------------------------------------------------------------*/
    
    
    inline void reset(){
        freeData();
        
        mNumDivers  = Rand::randInt(CELL_MIN_NUM_DIVERS,CELL_MAX_NUM_DIVERS);
        mDiverWidth = 1.0f / (float)mNumDivers;
        
        Vec3f start,end;
        float marginX = -0.5f + mDiverWidth * 0.5f;
        
        int i = -1;
        while (++i < mNumDivers) {
            Vec3f end(marginX + float(i) / float(mNumDivers), 0.0, -0.5f);
            Vec3f start(end.x, end.y, 0.5f);
            //mPaths.push_back(new Path(&mPathData,addPath(start, end),mDiverWidth));
            mPaths.push_back(new Path(start, end, mDiverWidth));
            mDivers.push_back(new Diver(mPaths.back(),Rand::randFloat(CELL_DIVER_MIN_HEIGHT,CELL_DIVER_MAX_HEIGHT)));
        }
       
#ifdef CELL_DIVER_DRAW_FRONT_BACK
        mDiverVerticesCapLen = 4 * 2;
#else 
        mDiverVerticesCapLen = 0;
#endif
        mDiverVerticesTubeLen = (CELL_DIVER_NUM_POINTS * 4) * 2; //(top , bottom , left, right ) * 2 ,
        mDiverVerticesLen     = mDiverVerticesTubeLen + mDiverVerticesCapLen;
        mDiverIndicesLen      = (CELL_DIVER_NUM_POINTS - 1) * 6  * 2 * 2 + (mDiverVerticesCapLen/2 * 3);// + ((mDiverVerticesCapLen / 2 - 1) * 3);
        mVboMeshVerticesLen   = (mDiverVerticesTubeLen + mDiverVerticesCapLen) * mNumDivers;
        mVboMeshIndicesLen    = mDiverIndicesLen  * mNumDivers;
        
        
        mVboMeshFoldIndices.resize(0);
        mVboMeshUnfoldIndices.resize(0);
        mVboMeshBufferIndices.resize(mDiverIndicesLen);
        
        //
        // setup a vector of 0 indices to use when folding a diver
        //
        i = -1;
        while(++i < mDiverIndicesLen){
            mVboMeshFoldIndices.push_back(0);
        }
        
        //
        // setup a vector of unit indices to use when unfolding a diver
        //
        int v00,v01,v02,v03;
        int v04,v05,v06,v07;
        int v08,v09,v10,v11;
        int v12,v13,v14,v15;
        int index;
        int j;
        i = -1;
        while (++i < CELL_DIVER_NUM_POINTS - 1) {
            index = i * 8;
            
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
            mVboMeshUnfoldIndices.push_back(v00);
            mVboMeshUnfoldIndices.push_back(v03);
            mVboMeshUnfoldIndices.push_back(v04);
            // bottom upper triangle
            mVboMeshUnfoldIndices.push_back(v03);
            mVboMeshUnfoldIndices.push_back(v07);
            mVboMeshUnfoldIndices.push_back(v04);
#endif
#ifdef CELL_DIVER_DRAW_TOP
            // top lower triangle
            mVboMeshUnfoldIndices.push_back(v01);
            mVboMeshUnfoldIndices.push_back(v02);
            mVboMeshUnfoldIndices.push_back(v05);
            // bottom upper triangle
            mVboMeshUnfoldIndices.push_back(v02);
            mVboMeshUnfoldIndices.push_back(v06);
            mVboMeshUnfoldIndices.push_back(v05);
#endif
#ifdef CELL_DIVER_DRAW_RIGHT
            // right lower triangle
            mVboMeshUnfoldIndices.push_back(v08);
            mVboMeshUnfoldIndices.push_back(v09);
            mVboMeshUnfoldIndices.push_back(v12);
            // right upper triangle
            mVboMeshUnfoldIndices.push_back(v09);
            mVboMeshUnfoldIndices.push_back(v13);
            mVboMeshUnfoldIndices.push_back(v12);
#endif
#ifdef CELL_DIVER_DRAW_LEFT
            //left lower triangle
            mVboMeshUnfoldIndices.push_back(v11);
            mVboMeshUnfoldIndices.push_back(v10);
            mVboMeshUnfoldIndices.push_back(v15);
            //left upper triangle
            mVboMeshUnfoldIndices.push_back(v10);
            mVboMeshUnfoldIndices.push_back(v14);
            mVboMeshUnfoldIndices.push_back(v15);
#endif
        }
#ifdef CELL_DIVER_DRAW_FRONT_BACK
        // current diver step 0 + just tube vertices, leaving 8 cap vertices
        index  =  mDiverVerticesTubeLen;
        
        // front
        v00 = index;
        v01 = index+1;
        v02 = index+2;
        v03 = index+3;
        
        // back
        v04 = index+4;
        v05 = index+5;
        v06 = index+6;
        v07 = index+7;
        
        // front lower triangle
        mVboMeshUnfoldIndices.push_back(v00);
        mVboMeshUnfoldIndices.push_back(v02);
        mVboMeshUnfoldIndices.push_back(v01);
        // front upper triangle
        mVboMeshUnfoldIndices.push_back(v02);
        mVboMeshUnfoldIndices.push_back(v03);
        mVboMeshUnfoldIndices.push_back(v01);
        
        // back lower triangle
        mVboMeshUnfoldIndices.push_back(v04);
        mVboMeshUnfoldIndices.push_back(v06);
        mVboMeshUnfoldIndices.push_back(v05);
        // back upper triangle
        mVboMeshUnfoldIndices.push_back(v06);
        mVboMeshUnfoldIndices.push_back(v07);
        mVboMeshUnfoldIndices.push_back(v05);
#endif
        
        //
        //  Setup vbo mesh
        //
        
        mVboMesh.reset();
        mVboMesh = gl::VboMesh(mVboMeshVerticesLen,mVboMeshIndicesLen,mVboMeshLayout,GL_TRIANGLES);
        
        vector<uint32_t> indices; // buffer target
        vector<Colorf>   colors;  // buffer colors for debug
     
        int offset = 0;
        
        const static Vec3f up(0,1,0);
        const static Vec3f down(0,-1,0);
        const static Vec3f left(-1,0,0);
        const static Vec3f right(1,0,0);
        const static Vec3f front(0,0,-1);
        const static Vec3f back(0,0,1);
        
        i = -1;
        while (++i < mNumDivers) {
            j = 0;
            while (j < CELL_DIVER_NUM_POINTS) {
                
                colors.push_back(toColor(down));
                colors.push_back(toColor(down));
                
                colors.push_back(toColor(up));
                colors.push_back(toColor(up));
                
                colors.push_back(toColor(left));
                colors.push_back(toColor(right));
                
                colors.push_back(toColor(left));
                colors.push_back(toColor(right));
                
                ++j;
            }
#ifdef CELL_DIVER_DRAW_FRONT_BACK
            colors.push_back(toColor(front));
            colors.push_back(toColor(front));
            colors.push_back(toColor(front));
            colors.push_back(toColor(front));
            
            colors.push_back(toColor(back));
            colors.push_back(toColor(back));
            colors.push_back(toColor(back));
            colors.push_back(toColor(back));
#endif
            // step one complete diver forward
            offset = mDiverVerticesLen * i;
            
            j = 0;
            while (j < CELL_DIVER_NUM_POINTS - 1) {
                //current diver beginning + step quads * 2
                index = offset + j * 8;
        
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
                j++;
            }
#ifdef CELL_DIVER_DRAW_FRONT_BACK
            // current diver step 0 + just tube vertices, leaing 8 cap vertices
            index  = offset + mDiverVerticesTubeLen;

            // front
            v00 = index;
            v01 = index+1;
            v02 = index+2;
            v03 = index+3;
            
            // back
            v04 = index+4;
            v05 = index+5;
            v06 = index+6;
            v07 = index+7;
            
            // front lower triangle
            indices.push_back(v00);
            indices.push_back(v02);
            indices.push_back(v01);
            // front upper triangle
            indices.push_back(v02);
            indices.push_back(v03);
            indices.push_back(v01);
      
            // back lower triangle
            indices.push_back(v04);
            indices.push_back(v06);
            indices.push_back(v05);
            // back upper triangle
            indices.push_back(v06);
            indices.push_back(v07);
            indices.push_back(v05);
#endif
        }
        
        mVboMesh.bufferIndices(indices);
        mVboMesh.bufferColorsRGB(colors);
        mVboMesh.unbindBuffers();
        
        //fold(0);
        fold(1);
        unfold(1);
        //fold(3);
        //fold(5);
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Visual Debug
    /*--------------------------------------------------------------------------------------------*/
    
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
    
    /*--------------------------------------------------------------------------------------------*/
    // Draw
    /*--------------------------------------------------------------------------------------------*/
    
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
    
    /*--------------------------------------------------------------------------------------------*/
    // Update
    /*--------------------------------------------------------------------------------------------*/
    
    //! Update all paths with oscillator
    inline void updatePaths(){
        if(!mActive){
            return;
        }
        
        static const float scale  = 0.75f;
        mOffset += 0.01f;
        
        for(vector<Path*>::const_iterator itr = mPaths.begin(); itr != mPaths.end(); ++itr){
            for(vector<Vec3f>::iterator _itr = (*itr)->getPoints().begin(); _itr != (*itr)->getPoints().end(); _itr++){
                _itr->y = mOscillator->getValue(mPos.x + _itr->x,
                                                mPos.z + _itr->z + mOffset,0,
                                                mId[0], mId[1]) * scale;
            }
        }
    }
    
    inline void updateDiversFolds(){
        if(!mActive){
            return;
        }
  
        int vboMeshVertexIndex = 0;
        for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
            Diver* const diver = *itr;
            
            if(diver->isOut() && !(diver->isOutPrev())){
              //  fold(vboMeshVertexIndex);
            } else if (!(diver->isOut()) && diver->isOutPrev()){
              //  unfold(vboMeshVertexIndex);
            }
            
            vboMeshVertexIndex += mDiverVerticesLen;
        }
    }
    
    //! Update all divers
    inline void updateDivers(){
        if(!mActive){
            return;
        }
        for (vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            (*itr)->update();
        }
    }
    
    //! Update vbo mesh geometry
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
            }
            
#ifdef CELL_DIVER_DRAW_FRONT_BACK
            const Vec3f& start = points[0];
            const Vec3f& end   = points[points.size() - 1];
            
            // start
            x0 = start.x - diverWidth_2;
            x1 = start.x + diverWidth_2;
            y0 = start.y - diverHeight_2;
            y1 = start.y + diverHeight_2;
            
            vbItr.setPosition(x0,y0,start.z);
            ++vbItr;
            
            vbItr.setPosition(x1,y0,start.z);
            ++vbItr;
            
            vbItr.setPosition(x0,y1,start.z);
            ++vbItr;
            
            vbItr.setPosition(x1,y1,start.z);
            ++vbItr;
            
            // end
            x0 = end.x - diverWidth_2;
            x1 = end.x + diverWidth_2;
            y0 = end.y - diverHeight_2;
            y1 = end.y + diverHeight_2;
            
            vbItr.setPosition(x0,y0,end.z);
            ++vbItr;
            
            vbItr.setPosition(x1,y0,end.z);
            ++vbItr;
            
            vbItr.setPosition(x0,y1,end.z);
            ++vbItr;
            
            vbItr.setPosition(x1,y1,end.z);
            ++vbItr;
#endif
        }

    }
    
    //! Check if a cell lies within the orthographics camera frustum
    inline void checkFrustum(const FrustumOrtho& frustum,const Matrix44f& transform){
        mActive = frustum.contains(transform.transformPointAffine(mPos));
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // State
    /*--------------------------------------------------------------------------------------------*/
    
    inline void activate(){
        mActive = true;
    }
    
    inline void deactivate(){
        mActive = false;
    }
};



#endif
