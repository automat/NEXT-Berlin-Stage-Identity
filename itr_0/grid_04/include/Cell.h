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

    Vec3f  mPos;    // global position of cell
    Vec3f  mSize;   // global size of cell, 1
    int    mId[2];  // x,y id of cell
    bool   mActive;
    
    int             mNumDivers;
    vector<Path*>   mPaths;
    vector<Diver*>  mDivers;
    
    vector<uint32_t> mDiverIndicesFolded;   // indices order, when folded
    vector<uint32_t> mDiverIndicesUnfolded; // indices order, when unfolded
    vector<uint32_t> mDiverIndicesBuffer;   // indices order, when translated to specific diver
    
    vector<Vec3f>  mPathData;
    
    float mDiverWidth;
    
    
    
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
        indexBuffer.bufferSubData(offset, size, &mDiverIndicesFolded[0]);
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
            mDiverIndicesBuffer[i] = mDiverIndicesUnfolded[i] + vertexIndex;
        }
        
        size_t offset = sizeof(uint32_t) * index * mDiverIndicesLen;
        size_t size   = sizeof(uint32_t) * mDiverIndicesLen;
        
        gl::Vbo& indexBuffer = mVboMesh.getIndexVbo();
        indexBuffer.bind();
        indexBuffer.bufferSubData(offset, size, &mDiverIndicesBuffer[0]);
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
            //mVboMeshLayout.setDynamicNormals();
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
       
        mDiverVerticesCapLen  = 4 * 2;
        mDiverVerticesTubeLen = (CELL_DIVER_NUM_POINTS * 4) * 2; //(top , bottom , left, right ) * 2 ,
        mDiverVerticesLen     = mDiverVerticesTubeLen + mDiverVerticesCapLen;
        mDiverIndicesLen      = (CELL_DIVER_NUM_POINTS - 1) * 6  * 2 * 2 + (mDiverVerticesCapLen/2 * 3);// + ((mDiverVerticesCapLen / 2 - 1) * 3);
        mVboMeshVerticesLen   = (mDiverVerticesTubeLen + mDiverVerticesCapLen) * mNumDivers;
        mVboMeshIndicesLen    = mDiverIndicesLen  * mNumDivers;
        
        
        mDiverIndicesFolded.resize(0);
        mDiverIndicesUnfolded.resize(0);
        mDiverIndicesBuffer.resize(mDiverIndicesLen);
        
        //
        // setup a vector of 0 indices to use when folding a diver
        //
        i = -1;
        while(++i < mDiverIndicesLen){
            mDiverIndicesFolded.push_back(0);
        }
        
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
        
        mVboMesh.reset();
        mVboMesh = gl::VboMesh(mVboMeshVerticesLen,mVboMeshIndicesLen,mVboMeshLayout,GL_TRIANGLES);
        
        vector<uint32_t> indices; // buffer target index
        vector<Vec3f>    normals; // buufer target normal;
        vector<Colorf>   colors;  // buffer colors for debug
     
        int vertexIndex = 0;
        
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
                colors.push_back(Utils::toColor(down));
                colors.push_back(Utils::toColor(down));
                
                colors.push_back(Utils::toColor(up));
                colors.push_back(Utils::toColor(up));
               
                colors.push_back(Utils::toColor(left));
                colors.push_back(Utils::toColor(right));
                
                colors.push_back(Utils::toColor(left));
                colors.push_back(Utils::toColor(right));
                
                normals.push_back(down);
                normals.push_back(down);
                
                normals.push_back(up);
                normals.push_back(up);
                
                normals.push_back(left);
                normals.push_back(right);
                
                normals.push_back(left);
                normals.push_back(right);
                
                ++j;
            }

            colors.push_back(Utils::toColor(front));
            colors.push_back(Utils::toColor(front));
            colors.push_back(Utils::toColor(front));
            colors.push_back(Utils::toColor(front));
            
            colors.push_back(Utils::toColor(back));
            colors.push_back(Utils::toColor(back));
            colors.push_back(Utils::toColor(back));
            colors.push_back(Utils::toColor(back));
            
            normals.push_back(front);
            normals.push_back(front);
            normals.push_back(front);
            normals.push_back(front);
            
            normals.push_back(back);
            normals.push_back(back);
            normals.push_back(back);
            normals.push_back(back);

            vertexIndex = i * mDiverVerticesLen;
            j = -1;
            while (++j < mDiverIndicesLen) {
                indices.push_back(mDiverIndicesUnfolded[j] + vertexIndex);
            }
        }
        
        mVboMesh.bufferIndices(indices);
        //mVboMesh.bufferNormals(normals);
        mVboMesh.bufferColorsRGB(colors);
        mVboMesh.unbindBuffers();
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
        float x0,x1,y0,y1,z;
        int i;

        gl::VboMesh::VertexIter vbItr = mVboMesh.mapVertexBuffer();
        bool diverOut;
        bool diverOutPrev;
        int diverIndex = 0;
        for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            Diver* const diver = *itr;
            
            diverHeight_2 = diver->getHeight() * 0.5f;
            const vector<Vec3f>& points = diver->getPoints();
            
            i = -1;
            while(++i < points.size()){
                const Vec3f& point = points[i];
                x0 = point.x - diverWidth_2;
                x1 = point.x + diverWidth_2;
                y0 = point.y - diverHeight_2;
                y1 = point.y + diverHeight_2;
                z  = point.z;
                
                // bottom
                vbItr.setPosition(x0, y0, z);
                ++vbItr;
                
                vbItr.setPosition(x1, y0, z);
                ++vbItr;
                
                // top
                vbItr.setPosition(x0, y1, z);
                ++vbItr;
                
                vbItr.setPosition(x1, y1, z);
                ++vbItr;
                
                // copy top for left / right
                vbItr.setPosition(x0, y0, z);
                ++vbItr;
                
                vbItr.setPosition(x1, y0, z);
                ++vbItr;
                
                // copy bottom for left / right
                vbItr.setPosition(x0, y1, z);
                ++vbItr;
                
                vbItr.setPosition(x1, y1, z);
                ++vbItr;
            }

            const Vec3f& start = points[0];
            const Vec3f& end   = points[points.size() - 1];
            
            // start
            x0 = start.x - diverWidth_2;
            x1 = start.x + diverWidth_2;
            y0 = start.y - diverHeight_2;
            y1 = start.y + diverHeight_2;
            z  = start.z;
            
            vbItr.setPosition(x0,y0,z);
            ++vbItr;
            
            vbItr.setPosition(x1,y0,z);
            ++vbItr;
            
            vbItr.setPosition(x0,y1,z);
            ++vbItr;
            
            vbItr.setPosition(x1,y1,z);
            ++vbItr;
            
            // end
            x0 = end.x - diverWidth_2;
            x1 = end.x + diverWidth_2;
            y0 = end.y - diverHeight_2;
            y1 = end.y + diverHeight_2;
            z  = end.z;
            
            vbItr.setPosition(x0,y0,end.z);
            ++vbItr;
            
            vbItr.setPosition(x1,y0,end.z);
            ++vbItr;
            
            vbItr.setPosition(x0,y1,end.z);
            ++vbItr;
            
            vbItr.setPosition(x1,y1,end.z);
            ++vbItr;

            // update if in or out,
            // need to be on same thread, otherwise in update
            diver->updateInOut();
            diverOut     = diver->isOut();
            diverOutPrev = diver->isOutPrev();
            
            // fold / unfold diver
            if(diverOut && !diverOutPrev){
                fold(diverIndex);
            } else if(!diverOut && diverOutPrev){
                unfold(diverIndex);
            }
               
            diverIndex++;
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
