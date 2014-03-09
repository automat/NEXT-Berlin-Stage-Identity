//
//  AbstractCell.h
//  grid_07
//
//  Created by Henryk Wollik on 08/03/14.
//
//

#ifndef grid_07_AbstractCell_h
#define grid_07_AbstractCell_h

#include <boost/assign/std/vector.hpp>
#include "Config.h"
#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"

#include "cinder/Vector.h"
#include "cinder/Matrix44.h"
#include "cinder/Rand.h"
#include "Oscillator.h"

#include "cinder/gl/Vbo.h"

#include "Utils.h"
#include "FrustumOrtho.h"
#include "Path.h"
#include "Diver.h"

using namespace ci;
using namespace std;
using namespace boost::assign;

class AbstractCell {
protected:
    int   mNumDiversMin;
    int   mNumDiversMax;
    
    float mPathAmplitude;
    float mOffsetSpeed;
    int   mDiverUnitNumPoints;
    float mDiverOffsetMin;
    float mDiverOffsetMax;
    float mDiverSpeedMin;
    float mDiverSpeedMax;
    float mDiverLengthMin;
    float mDiverLengthMax;
    float mDiverHeightMin;
    float mDiverHeightMax;
    
    
    
    Vec3f  mPos;    // global position of cell
    Vec3f  mSize;   // global size of cell, 1
    int    mId[2];  // x,y id of cell
    bool   mActive;
    
    Oscillator* mOscillator;
    
    int            mNumDivers;
    int            mDiverNumPoints;
    vector<Path*>  mPaths;
    vector<Diver*> mDivers;
    float          mOffset;
    float          mDiverWidth;
    
    int mDiverVerticesBodyLen;
    int mDiverVerticesCapLen;
    int mDiverVerticesLen;
    int mDiverIndicesLen;
    
    int mMeshVerticesLen;
    int mMeshIndicesLen;
    
    gl::VboMesh::Layout mMeshLayout;
    gl::VboMesh         mMesh;
    
    vector<Vec3f>    mMeshVertexBuffer;     // keep copy of vertices
    vector<Vec3f>    mMeshNormalBuffer;     // buffer for normals
    vector<Vec3f>    mMeshNormalTopBuffer;  // buffer for all top normals
    vector<uint32_t> mMeshIndexScheme;      // indices order, used for normal calculation
    vector<uint32_t> mMeshIndexBuffer;      // indices order, send to buffer object concatenated
    
    vector<uint32_t> mDiverIndicesFolded;   // indices order, when folded
    vector<uint32_t> mDiverIndicesUnfolded; // indices order, when unfolded
    vector<uint32_t> mDiverIndicesBuffer;   // indices order, when translated to specific diver
    
    //
    // free all pointer data
    //
    void freeData(){
        while (!mDivers.empty()) delete mDivers.back(), mDivers.pop_back();
        while (!mPaths.empty())  delete mPaths.back(), mPaths.pop_back();
        
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Fold / Unfold
    /*--------------------------------------------------------------------------------------------*/
    
    //
    // collapse all vertices of a diver into one vertex by setting
    // all its indices to its first index
    
    inline void fold(int index){
        // copy to target buffer
        copy(mDiverIndicesFolded.begin(),
             mDiverIndicesFolded.end(),
             mMeshIndexBuffer.begin() + index * mDiverIndicesLen);
    }
    
    //
    // reorder all indices to their vertices,
    // recreating the orignal shape
    //
    inline void unfold(int index){
        int vertexIndex = index * mDiverVerticesLen;
        int i = -1;
        while(++i < mDiverIndicesLen){
            mDiverIndicesBuffer[i] = mDiverIndicesUnfolded[i] + vertexIndex;
        }
        // copy to target buffer
        copy(mDiverIndicesBuffer.begin(),
             mDiverIndicesBuffer.end(),
             mMeshIndexBuffer.begin() + index * mDiverIndicesLen);
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Geom init
    /*--------------------------------------------------------------------------------------------*/
    
    
    inline void reset_internal(){
        freeData();
        
        mNumDivers  = Rand::randInt(mNumDiversMin,mNumDiversMax);
        mDiverWidth = 1.0f / (float)mNumDivers;
        
        Vec3f start,end;
        float marginX = -0.5f + mDiverWidth * 0.5f;
        
        int i = -1;
        while (++i < mNumDivers) {
            Vec3f end(marginX + float(i) / float(mNumDivers), 0.0, -0.5f);
            Vec3f start(end.x, end.y, mSize.x - 0.5f);
            
            mPaths.push_back(new Path(start, end));
            mDivers.push_back(new Diver(mPaths.back(),                                                  // path
                                        mDiverUnitNumPoints,
                                        Rand::randFloat(mDiverOffsetMin,mDiverOffsetMax),            // offset
                                        Rand::randFloat(mDiverSpeedMin, mDiverSpeedMax),              // speed
                                        mDiverWidth,
                                        Rand::randFloat(mDiverLengthMin,mDiverLengthMax),             // length
                                        Rand::randFloat(mDiverHeightMin,mDiverHeightMax))); // height
        }
        
        mDiverNumPoints = mDivers[0]->getNumPoints();
        
        
        mDiverVerticesCapLen  = 8; // top + back
        mDiverVerticesBodyLen = (mDiverNumPoints * 4) * 2; //(top , bottom , left, right ) * 2 ,
        mDiverVerticesLen     = mDiverVerticesBodyLen + mDiverVerticesCapLen;
        mDiverIndicesLen      = (mDiverNumPoints - 1) * 6  * 2 * 2 + (mDiverVerticesCapLen/2 * 3);
        mMeshVerticesLen      = (mDiverVerticesBodyLen + mDiverVerticesCapLen) * mNumDivers;
        mMeshIndicesLen       = mDiverIndicesLen  * mNumDivers;
        
        
        mDiverIndicesFolded.resize(mDiverIndicesLen);
        mDiverIndicesUnfolded.resize(0);
        mDiverIndicesBuffer.resize(mDiverIndicesLen);
        mMeshIndexBuffer.resize(mMeshIndicesLen);
        mMeshIndexScheme.resize(0);
        
        mMeshNormalBuffer.resize(0);
        mMeshVertexBuffer.resize(mMeshVerticesLen);
        
        //
        // setup a vector of 0 indices to use when folding a diver
        //
        mDiverIndicesFolded.resize(mDiverIndicesLen);
        
        fill(mDiverIndicesFolded.begin(), mDiverIndicesFolded.end(), 0);
        
        //
        // Start unfolded indices
        // setup a vector of unit indices to use when unfolding a diver
        //
        
        int v00,v01,v02,v03,
        v04,v05,v06,v07,
        v08,v09,v10,v11,
        v12,v13,v14,v15;
        int index;
        
        int j;
        i = -1;
        while (++i < mDiverNumPoints - 1) {
            index = i * 8;
            
            // bottom -> j / top ->j(next step)
            // the quads are verticaly sliced
            
            // first quad
            v00 = index +  0; v01 = index + 2;
            v02 = index +  3; v03 = index + 1;
            
            // next quad
            v04 = index +  8; v05 = index + 10;
            v06 = index + 11; v07 = index + 9;
            
            // first quad duplicate
            v08 = index +  4; v09 = index + 6;
            v10 = index +  7; v11 = index + 5;
            
            // next quad duplicate
            v12 = index + 12; v13 = index + 14;
            v14 = index + 15; v15 = index + 13;
            
            // bottom lower & upper triangle
            mDiverIndicesUnfolded += v00,v03,v04;
            mDiverIndicesUnfolded += v03,v07,v04;
            
            // top lower & upper triangle
            mDiverIndicesUnfolded += v01,v02,v05;
            mDiverIndicesUnfolded += v02,v06,v05;
            
            // right lower & upper triangle
            mDiverIndicesUnfolded += v08,v09,v12;
            mDiverIndicesUnfolded += v09,v13,v12;
            
            // left lower & upper triangle
            mDiverIndicesUnfolded += v11,v10,v15;
            mDiverIndicesUnfolded += v10,v14,v15;
        }
        
        // current diver step 0 + just tube vertices, leaving 8 cap vertices
        index = mDiverVerticesBodyLen;
        
        // front
        v00 = index    ; v01 = index + 1;
        v02 = index + 2; v03 = index + 3;
        // back
        v04 = index + 4; v05 = index + 5;
        v06 = index + 6; v07 = index + 7;
        
        // front lower & upper triangle
        mDiverIndicesUnfolded += v00,v02,v01;
        mDiverIndicesUnfolded += v02,v03,v01;
        
        // back lower & upper triangle
        mDiverIndicesUnfolded += v04,v06,v05;
        mDiverIndicesUnfolded += v06,v07,v05;
        
        //
        //  End unfolded indices
        //
        
        //
        //  Redo all for the top
        //
        
        i = -1;
        while (++i < mNumDivers) {
            j = -1;
            while (++j < mDiverNumPoints - 1) {
                index = (i * mDiverNumPoints + j + i) * 8;
                // first quad
                v00 = index + 0; v01 = index + 2;
                v02 = index + 3; v03 = index + 1;
                // next quad
                v04 = index + 8; v05 = index + 10;
                v06 = index + 11;v07 = index + 9;
                // top lower & upper triangle
                mMeshIndexScheme += v01,v02,v05;
                mMeshIndexScheme += v02,v06,v05;
            }
        }
        
        
        //
        //  Setup vbo mesh
        //
        
        //mMesh.reset();
        mMesh.reset();
        mMesh = gl::VboMesh(mMeshVerticesLen,mMeshIndicesLen,mMeshLayout,GL_TRIANGLES);
        
        vector<Colorf> meshColors;  // buffer colors for debug
        
        static const Vec3f up(0,1,0),down(0,-1,0);
        static const Vec3f left(-1,0,0),right(1,0,0);
        static const Vec3f front(0,0,-1),back(0,0,1);
        
        i = -1;
        while (++i < mNumDivers) {
            j = 0;
            while (j < mDiverNumPoints) {
                meshColors += Utils::toColor(down),Utils::toColor(down);
                meshColors += Utils::toColor(  up),Utils::toColor(  up);
                meshColors += Utils::toColor(left),Utils::toColor(right);
                meshColors += Utils::toColor(left),Utils::toColor(right);
                
                mMeshNormalBuffer += down,down;
                mMeshNormalBuffer += up,up;
                mMeshNormalBuffer += left,right;
                mMeshNormalBuffer += left,right;
                
                ++j;
            }
            
            meshColors += Utils::toColor(front),Utils::toColor(front);
            meshColors += Utils::toColor(front),Utils::toColor(front);
            meshColors += Utils::toColor( back),Utils::toColor( back);
            meshColors += Utils::toColor( back),Utils::toColor( back);
            
            mMeshNormalBuffer += front,front,front,front;
            mMeshNormalBuffer += back,back,back,back;
            
            unfold(i);
        }
        
        mMesh.bufferColorsRGB(meshColors);
        mMesh.bufferIndices(mMeshIndexBuffer);
        mMesh.unbindBuffers();
    }
    
public:
    
    AbstractCell(int* id, const Vec3f& pos, Oscillator* oscillator, const Vec3f& size) :
        mPos(pos),
        mOscillator(oscillator),
        mSize(size),
        mOffset(0.0f),
        mActive(true){
            
            mId[0] = id[0];
            mId[1] = id[1];
            
            mMeshLayout.setDynamicPositions();
            mMeshLayout.setStaticNormals();
            mMeshLayout.setDynamicIndices();
            mMeshLayout.setStaticColorsRGB();
    }
    
    ~AbstractCell(){
        freeData();
    }
   
    virtual void reset() = 0;
    
    /*--------------------------------------------------------------------------------------------*/
    // Visual Debug
    /*--------------------------------------------------------------------------------------------*/
    
    inline void debugArea(){
        float points[12];
        
        points[ 1] = points[ 4] = points[ 7] = points[10] = 0;
        points[ 0] = -0.5f;
        points[ 2] = mSize.x - 0.5f;
        points[ 3] = 0.5f;
        points[ 5] = mSize.x - 0.5f;
        points[ 6] = 0.5f;
        points[ 8] = -0.5f;
        points[ 9] = -0.5f;
        points[11] = -0.5f;
        
        glColor3f(1,0,0);
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, mPos.z);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(6, 0xAAAA);
        glVertexPointer(3, GL_FLOAT, 0, &points[0]);
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
        gl::draw(mMesh);
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
        mOffset += mOffsetSpeed;
        for(vector<Path*>::const_iterator itr = mPaths.begin(); itr != mPaths.end(); ++itr){
            for(vector<Vec3f>::iterator _itr = (*itr)->getPoints().begin(); _itr != (*itr)->getPoints().end(); _itr++){
                _itr->y = mOscillator->getValue(mPos.x + _itr->x * mPathAmplitude,
                                                mPos.z + _itr->z * mPathAmplitude + mOffset,0,
                                                mId[0], mId[1]) * scale;
            }
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
    inline void update(){
        if(!mActive){
            return;
        }
        
        static const Vec3f up(0,1,0),down(0,-1,0);
        static const Vec3f left(-1,0,0),right(1,0,0);
        static const Vec3f front(0,0,-1),back(0,0,1);
        Vec3f tempNormal;
        
        // update geometry
        float diverWidth_2 = mDiverWidth * 0.5f;
        float diverHeight_2;
        bool  diverIsOut;
        bool  diverIsOutPrev;
        int   diverIndex = 0;
        int   vertexIndex = 0;
        
        float x0,x1,y0,y1,z;
        
        int i;
        gl::VboMesh::VertexIter vbItr = mMesh.mapVertexBuffer();
        for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            Diver* const diver = *itr;
            
            diver->updateInOut();
            diverIsOut     = diver->isOut();
            diverIsOutPrev = diver->isOutPrev();
            
            //
            // diver is not visible,
            // skip to next
            //
            if(diver->isOut()){
                diver->updateInOut();
                i = -1;
                while(++i < mDiverNumPoints){
                    ++vbItr;++vbItr;++vbItr;++vbItr; // step body forward
                    ++vbItr;++vbItr;++vbItr;++vbItr;
                    vertexIndex += 8;
                }
                
                ++vbItr;++vbItr;++vbItr;++vbItr; // step cap forward
                ++vbItr;++vbItr;++vbItr;++vbItr;
                vertexIndex += 8;
                
                if(diverIsOut && !diverIsOutPrev){
                    fold(diverIndex);
                }
                diverIndex++;
                continue;
            }
            
            
            //
            //  diver is visible,
            //  update all vertices, copies and normals
            //
            
            diverHeight_2 = diver->getHeight() * 0.5f;
            const vector<Vec3f>& points = diver->getPoints();
            
            i = -1;
            while(++i < mDiverNumPoints){
                const Vec3f& point = points[i];
                x0 = point.x - diverWidth_2;
                x1 = point.x + diverWidth_2;
                y0 = point.y - diverHeight_2;
                y1 = point.y + diverHeight_2;
                z  = point.z;
                
                vbItr.setPosition(x0, y0, z);++vbItr; // bottom
                vbItr.setPosition(x1, y0, z);++vbItr;
                vbItr.setPosition(x0, y1, z);++vbItr; // top
                vbItr.setPosition(x1, y1, z);++vbItr;
                vbItr.setPosition(x0, y0, z);++vbItr; // copy top for left / right
                vbItr.setPosition(x1, y0, z);++vbItr;
                vbItr.setPosition(x0, y1, z);++vbItr; // copy bottom for left / right
                vbItr.setPosition(x1, y1, z);++vbItr;
                
                mMeshVertexBuffer[vertexIndex++].set(x0,y0,z); // buffer copy bottom
                mMeshVertexBuffer[vertexIndex++].set(x1,y0,z);
                mMeshVertexBuffer[vertexIndex++].set(x0,y1,z); // buffer copy top
                mMeshVertexBuffer[vertexIndex++].set(x1,y1,z);
                mMeshVertexBuffer[vertexIndex++].set(x0,y0,z); // buffer copy left/right
                mMeshVertexBuffer[vertexIndex++].set(x1,y0,z);
                mMeshVertexBuffer[vertexIndex++].set(x0,y1,z); // buffer copy left/right
                mMeshVertexBuffer[vertexIndex++].set(x1,y1,z);
            }
            
            const Vec3f& start = points[0];
            const Vec3f& end   = points[points.size() - 1];
            
            // front
            x0 = start.x - diverWidth_2;
            x1 = start.x + diverWidth_2;
            y0 = start.y - diverHeight_2;
            y1 = start.y + diverHeight_2;
            z  = start.z;
            
            vbItr.setPosition(x0,y0,z);++vbItr;
            vbItr.setPosition(x1,y0,z);++vbItr;
            vbItr.setPosition(x0,y1,z);++vbItr;
            vbItr.setPosition(x1,y1,z);++vbItr;
            
            // buffer copy front
            mMeshVertexBuffer[vertexIndex++].set(x0,y0,z);
            mMeshVertexBuffer[vertexIndex++].set(x1,y0,z);
            mMeshVertexBuffer[vertexIndex++].set(x0,y1,z);
            mMeshVertexBuffer[vertexIndex++].set(x1,y1,z);
            
            // back
            x0 = end.x - diverWidth_2;
            x1 = end.x + diverWidth_2;
            y0 = end.y - diverHeight_2;
            y1 = end.y + diverHeight_2;
            z  = end.z;
            
            vbItr.setPosition(x0,y0,end.z);++vbItr;
            vbItr.setPosition(x1,y0,end.z);++vbItr;
            vbItr.setPosition(x0,y1,end.z);++vbItr;
            vbItr.setPosition(x1,y1,end.z);++vbItr;
            
            // buffer copy back
            mMeshVertexBuffer[vertexIndex++].set(x0,y0,z);
            mMeshVertexBuffer[vertexIndex++].set(x1,y0,z);
            mMeshVertexBuffer[vertexIndex++].set(x0,y1,z);
            mMeshVertexBuffer[vertexIndex++].set(x1,y1,z);
            
            // fold / unfold diver
            if(diverIsOut && !diverIsOutPrev){
                fold(diverIndex);
            } else if(!diverIsOut && diverIsOutPrev){
                unfold(diverIndex);
            }
            
            diverIndex++;
        }
        
        // upload all indices to the buffer
        size_t offset = 0;
        size_t size   = mMeshIndexBuffer.size() * sizeof(uint32_t);
        gl::Vbo& indexBuffer = mMesh.getIndexVbo();
        indexBuffer.bind();
        indexBuffer.bufferSubData(offset, size, &mMeshIndexBuffer[0]);
        indexBuffer.unbind();
        
        
        updateNormals();
        
        mMesh.bufferNormals(mMeshNormalBuffer);
    }
    
    inline void updateNormals(){
        static const Vec3f zero(0,0,0);
        
        size_t size = mMeshIndexScheme.size();
        int i = -1;
        while (++i < size) {
            mMeshNormalBuffer[mMeshIndexScheme[i]].set(zero);
        }
        
        size_t numTriangles         = size / 3;
        
        Vec3f e0,e1,normal;
        int index0,index1,index2;
        int i3;
        
        i = -1;
        while(++i < numTriangles){
            i3 = i * 3;
            index0 = mMeshIndexScheme[i3    ];
            index1 = mMeshIndexScheme[i3 + 1];
            index2 = mMeshIndexScheme[i3 + 2];
            e0.set(mMeshVertexBuffer[ index1 ] - mMeshVertexBuffer[ index0 ]);
            e1.set(mMeshVertexBuffer[ index2 ] - mMeshVertexBuffer[ index0 ]);
            normal.set(e0.cross(e1).safeNormalized());
            
            mMeshNormalBuffer[ index0 ] += normal;
            mMeshNormalBuffer[ index1 ] += normal;
            mMeshNormalBuffer[ index2 ] += normal;
        }
        
        
        i = -1;
        while(++i < size){
            mMeshNormalBuffer[mMeshIndexScheme[i]].safeNormalize();
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
        for (vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            (*itr)->show();
        }
    }
    
    inline void deactivate(){
        mActive = false;
        for (vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
            (*itr)->hide();
        }
    }




    inline int getNumDivers(){
        return mNumDivers;
    }

    
};



#endif
