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

#include "Utils.h"
#include "FrustumOrtho.h"
#include "Path.h"
#include "Diver.h"

using namespace ci;
using namespace std;

class Cell {
    /*--------------------------------------------------------------------------------------------*/
    // Member
    /*--------------------------------------------------------------------------------------------*/
protected:
    
    Vec3f  mPos;    // global position of cell
    Vec3f  mSize;   // global size of cell, 1
    int    mId[2];  // x,y id of cell
    bool   mActive;
    
    Oscillator* mOscillator;
    
    int            mNumDivers;
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
    
#ifdef CELL_CALCULATE_NORMALS
    vector<Vec3f>    mMeshVertexBuffer; // keep copy of vertices
#endif
    vector<Vec3f>    mMeshNormalBuffer;     // buffer for normals
    vector<uint32_t> mMeshIndexScheme;      // indices order, used for normal calculation
    vector<uint32_t> mMeshIndexBuffer;      // indices order, send to buffer object concatenated
    
    vector<uint32_t> mDiverIndicesFolded;   // indices order, when folded
    vector<uint32_t> mDiverIndicesUnfolded; // indices order, when unfolded
    vector<uint32_t> mDiverIndicesBuffer;   // indices order, when translated to specific diver
    
    //
    // free all pointer data
    //
    void freeData(){
        while (!mPaths.empty())  delete mPaths.back(), mPaths.pop_back();
        while (!mDivers.empty()) delete mDivers.back(), mDivers.pop_back();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Path Management
    /*--------------------------------------------------------------------------------------------*/
    
    
    
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
            
            mMeshLayout.setDynamicPositions();
            mMeshLayout.setStaticNormals();
            mMeshLayout.setDynamicIndices();
            mMeshLayout.setStaticColorsRGB();

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

            mPaths.push_back(new Path(start, end));
            mDivers.push_back(new Diver(mPaths.back(),                                                  // path
                                        Rand::randFloat(DIVER_MIN_OFFSET, DIVER_MAX_OFFSET),            // offset
                                        Rand::randFloat(DIVER_MIN_SPEED, DIVER_MAX_SPEED),              // speed
                                        mDiverWidth,
                                        Rand::randFloat(DIVER_MIN_LENGTH,DIVER_MAX_LENGTH),             // length
                                        Rand::randFloat(CELL_DIVER_MIN_HEIGHT,CELL_DIVER_MAX_HEIGHT))); // height
        }
       
        mDiverVerticesCapLen  = 8; // top + back
        mDiverVerticesBodyLen = (CELL_DIVER_NUM_POINTS * 4) * 2; //(top , bottom , left, right ) * 2 ,
        mDiverVerticesLen     = mDiverVerticesBodyLen + mDiverVerticesCapLen;
        mDiverIndicesLen      = (CELL_DIVER_NUM_POINTS - 1) * 6  * 2 * 2 + (mDiverVerticesCapLen/2 * 3);
        mMeshVerticesLen      = (mDiverVerticesBodyLen + mDiverVerticesCapLen) * mNumDivers;
        mMeshIndicesLen       = mDiverIndicesLen  * mNumDivers;
        
        
        mDiverIndicesFolded.resize(mDiverIndicesLen);
        mDiverIndicesUnfolded.resize(0);
        mDiverIndicesBuffer.resize(mDiverIndicesLen);
        mMeshIndexBuffer.resize(mMeshIndicesLen);
        mMeshIndexScheme.resize(mMeshIndicesLen);
#ifdef CELL_CALCULATE_NORMALS
        mMeshNormalBuffer.resize(0);
        mMeshVertexBuffer.resize(mMeshVerticesLen);
#endif
        
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

            // bottom lower triangle
            mDiverIndicesUnfolded.push_back(v00);
            mDiverIndicesUnfolded.push_back(v03);
            mDiverIndicesUnfolded.push_back(v04);
            // bottom upper triangle
            mDiverIndicesUnfolded.push_back(v03);
            mDiverIndicesUnfolded.push_back(v07);
            mDiverIndicesUnfolded.push_back(v04);

            // top lower triangle
            mDiverIndicesUnfolded.push_back(v01);
            mDiverIndicesUnfolded.push_back(v02);
            mDiverIndicesUnfolded.push_back(v05);
            // bottom upper triangle
            mDiverIndicesUnfolded.push_back(v02);
            mDiverIndicesUnfolded.push_back(v06);
            mDiverIndicesUnfolded.push_back(v05);

            // right lower triangle
            mDiverIndicesUnfolded.push_back(v08);
            mDiverIndicesUnfolded.push_back(v09);
            mDiverIndicesUnfolded.push_back(v12);
            // right upper triangle
            mDiverIndicesUnfolded.push_back(v09);
            mDiverIndicesUnfolded.push_back(v13);
            mDiverIndicesUnfolded.push_back(v12);

            //left lower triangle
            mDiverIndicesUnfolded.push_back(v11);
            mDiverIndicesUnfolded.push_back(v10);
            mDiverIndicesUnfolded.push_back(v15);
            //left upper triangle
            mDiverIndicesUnfolded.push_back(v10);
            mDiverIndicesUnfolded.push_back(v14);
            mDiverIndicesUnfolded.push_back(v15);
        }

        // current diver step 0 + just tube vertices, leaving 8 cap vertices
        index  =  mDiverVerticesBodyLen;
        
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
        mDiverIndicesUnfolded.push_back(v00);
        mDiverIndicesUnfolded.push_back(v02);
        mDiverIndicesUnfolded.push_back(v01);
        // front upper triangle
        mDiverIndicesUnfolded.push_back(v02);
        mDiverIndicesUnfolded.push_back(v03);
        mDiverIndicesUnfolded.push_back(v01);
        
        // back lower triangle
        mDiverIndicesUnfolded.push_back(v04);
        mDiverIndicesUnfolded.push_back(v06);
        mDiverIndicesUnfolded.push_back(v05);
        // back upper triangle
        mDiverIndicesUnfolded.push_back(v06);
        mDiverIndicesUnfolded.push_back(v07);
        mDiverIndicesUnfolded.push_back(v05);
        
        //
        //  End unfolded indices
        //
        
        //
        //  Setup vbo mesh
        //
        
        mMesh.reset();
        mMesh = gl::VboMesh(mMeshVerticesLen,mMeshIndicesLen,mMeshLayout,GL_TRIANGLES);
        
        vector<Colorf> meshColors;  // buffer colors for debug
     
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
                meshColors.push_back(Utils::toColor(down));
                meshColors.push_back(Utils::toColor(down));
                
                meshColors.push_back(Utils::toColor(up));
                meshColors.push_back(Utils::toColor(up));
               
                meshColors.push_back(Utils::toColor(left));
                meshColors.push_back(Utils::toColor(right));
                
                meshColors.push_back(Utils::toColor(left));
                meshColors.push_back(Utils::toColor(right));
                
                mMeshNormalBuffer.push_back(down);
                mMeshNormalBuffer.push_back(down);
                mMeshNormalBuffer.push_back(up);
                mMeshNormalBuffer.push_back(up);
                mMeshNormalBuffer.push_back(left);
                mMeshNormalBuffer.push_back(right);
                mMeshNormalBuffer.push_back(left);
                mMeshNormalBuffer.push_back(right);
                
                ++j;
            }

            meshColors.push_back(Utils::toColor(front));
            meshColors.push_back(Utils::toColor(front));
            meshColors.push_back(Utils::toColor(front));
            meshColors.push_back(Utils::toColor(front));
            
            meshColors.push_back(Utils::toColor(back));
            meshColors.push_back(Utils::toColor(back));
            meshColors.push_back(Utils::toColor(back));
            meshColors.push_back(Utils::toColor(back));
            
            mMeshNormalBuffer.push_back(front);
            mMeshNormalBuffer.push_back(front);
            mMeshNormalBuffer.push_back(front);
            mMeshNormalBuffer.push_back(front);
            
            mMeshNormalBuffer.push_back(back);
            mMeshNormalBuffer.push_back(back);
            mMeshNormalBuffer.push_back(back);
            mMeshNormalBuffer.push_back(back);
            
            unfold(i);
        }
        
        vector<uint32_t> indexBufferCopy(mMeshIndexBuffer);
        mMeshIndexScheme = indexBufferCopy;


        
        mMesh.bufferColorsRGB(meshColors);
        mMesh.bufferIndices(mMeshIndexBuffer);
        mMesh.unbindBuffers();
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
        mOffset += CELL_OFFSET_SPEED;
      
        for(vector<Path*>::const_iterator itr = mPaths.begin(); itr != mPaths.end(); ++itr){
            for(vector<Vec3f>::iterator _itr = (*itr)->getPoints().begin(); _itr != (*itr)->getPoints().end(); _itr++){
                _itr->y = mOscillator->getValue(mPos.x + _itr->x,
                                                mPos.z + _itr->z + mOffset,0,
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
        
        static const Vec3f up(0,1,0);
        static const Vec3f down(0,-1,0);
        static const Vec3f left(-1,0,0);
        static const Vec3f right(1,0,0);
        static const Vec3f front(0,0,-1);
        static const Vec3f back(0,0,1);
        Vec3f tempNormal;
        
        // update geometry
        float diverWidth_2 = mDiverWidth * 0.5f;
        float diverHeight_2;
        bool  diverIsOut;
        bool  diverIsOutPrev;
        int   diverIndex = 0;
   
#ifdef CELL_CALCULATE_NORMALS
        int   vertexIndex = 0;
#endif
        
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
            /*
            if(diver->isOut()){
                diver->updateInOut();
                i = -1;
                while(++i < DIVER_NUM_POINTS){
                    ++vbItr;++vbItr;++vbItr;++vbItr;
                    ++vbItr;++vbItr;++vbItr;++vbItr;
#ifdef CELL_CALCULATE_NORMALS
                    vertexIndex += 8;
#endif
                }
                ++vbItr;++vbItr;++vbItr;++vbItr;
                ++vbItr;++vbItr;++vbItr;++vbItr;
#ifdef CELL_CALCULATE_NORMALS
                vertexIndex += 8;
#endif
                if(diverIsOut && !diverIsOutPrev){
                    fold(diverIndex);
                }
                diverIndex++;
                continue;
            }
             */
             
            //
            //  diver is visible,
            //  update all vertices, copies and normals
            //
            
            diverHeight_2 = diver->getHeight() * 0.5f;
            const vector<Vec3f>& points = diver->getPoints();
            
            i = -1;
            while(++i < DIVER_NUM_POINTS){
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
                
#ifdef CELL_CALCULATE_NORMALS
                mMeshVertexBuffer[vertexIndex++].set(x0,y0,z); // buffer copy bottom
                mMeshVertexBuffer[vertexIndex++].set(x1,y0,z);
                mMeshVertexBuffer[vertexIndex++].set(x0,y1,z); // buffer copy top
                mMeshVertexBuffer[vertexIndex++].set(x1,y1,z);
                mMeshVertexBuffer[vertexIndex++].set(x0,y0,z); // buffer copy left/right
                mMeshVertexBuffer[vertexIndex++].set(x1,y0,z);
                mMeshVertexBuffer[vertexIndex++].set(x0,y1,z); // buffer copy left/right
                mMeshVertexBuffer[vertexIndex++].set(x1,y1,z);
#endif
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
            
#ifdef CELL_CALCULATE_NORMALS
            // buffer copy front
            mMeshVertexBuffer[vertexIndex++].set(x0,y0,z);
            mMeshVertexBuffer[vertexIndex++].set(x1,y0,z);
            mMeshVertexBuffer[vertexIndex++].set(x0,y1,z);
            mMeshVertexBuffer[vertexIndex++].set(x1,y1,z);
#endif
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
            
#ifdef CELL_CALCULATE_NORMALS
            // buffer copy back
            mMeshVertexBuffer[vertexIndex++].set(x0,y0,z);
            mMeshVertexBuffer[vertexIndex++].set(x1,y0,z);
            mMeshVertexBuffer[vertexIndex++].set(x0,y1,z);
            mMeshVertexBuffer[vertexIndex++].set(x1,y1,z);
#endif
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
        mMeshNormalBuffer.assign(mMeshNormalBuffer.size(),Vec3f::zero());
        
        size_t numTriangles = mMeshIndicesLen / 3;
        int index0,index1,index2;
        int i, i3;
        i = -1;
        while(++i < numTriangles){
            i3 = i * 3;
            index0 = mMeshIndexScheme[i3    ];
            index1 = mMeshIndexScheme[i3 + 1];
            index2 = mMeshIndexScheme[i3 + 2];
            
            
            Vec3f e0 = mMeshVertexBuffer[ index1 ] - mMeshVertexBuffer[ index0 ];
            Vec3f e1 = mMeshVertexBuffer[ index2 ] - mMeshVertexBuffer[ index0 ];
            Vec3f normal = e0.cross(e1).normalized();
            
            mMeshNormalBuffer[ index0 ] += normal;
            mMeshNormalBuffer[ index1 ] += normal;
            mMeshNormalBuffer[ index2 ] += normal;
            
            
        }
        
        std::for_each(mMeshNormalBuffer.begin(),
                      mMeshNormalBuffer.end(),
                      std::mem_fun_ref(&Vec3f::normalize));
        
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
};



#endif
