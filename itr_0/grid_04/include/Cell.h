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
       
     
        mDiverVerticesCapLen  = 4;//4 * 2;
        mDiverVerticesTubeLen = CELL_DIVER_NUM_POINTS * 4 * 2; //(top , bottom , left, right ) * 2 ,
        mDiverIndicesLen      = (CELL_DIVER_NUM_POINTS - 1) * 6  * 2 * 2 + (mDiverVerticesCapLen/2 * 3);// + ((mDiverVerticesCapLen / 2 - 1) * 3);
        mVboMeshVerticesLen   = (mDiverVerticesTubeLen + mDiverVerticesCapLen) * mNumDivers;
        mVboMeshIndicesLen    = mDiverIndicesLen  * mNumDivers;
        
        mVboMesh.reset();
        mVboMesh = gl::VboMesh(mVboMeshVerticesLen,mVboMeshIndicesLen,mVboMeshLayout,GL_TRIANGLES);

        cout<< "TUBE LEN " << mDiverVerticesTubeLen << endl;
        
        vector<uint32_t> indices;
     
        int j;
        int v0,v1,v2,v3;
        int v4,v5,v6,v7;
        int index;
        int uniqueVerticesLen = mDiverVerticesTubeLen / 2  ;
        int cap   = 4;
        i = -1;
        while (++i < mNumDivers) {
            j = 0;
            while (j < uniqueVerticesLen - 4) {
                index = (i * uniqueVerticesLen + j) * 2;
                // 2 side
                // bottom -> j / top ->j(next step)
                // bottom
                v0 = index; //0
                v1 = v0+1;  //1
                v3 = v1+7;  //3
                v2 = v3+1;  //2
                
                // top
                v4 = v0 + 2;
                v5 = v1 + 2;
                v6 = v2 + 2;
                v7 = v3 + 2;
                
                // bottom
                indices.push_back(v0);
                indices.push_back(v1);
                indices.push_back(v3);
                
                indices.push_back(v1);
                indices.push_back(v2);
                indices.push_back(v3);
                
                
                // top
                indices.push_back(v4);
                indices.push_back(v5);
                indices.push_back(v7);
                
                indices.push_back(v5);
                indices.push_back(v6);
                indices.push_back(v7);
                
                
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
                 
                
                

#ifdef CELL_DEBUG_DRAW_MESH_INDICES
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
#endif
                j+=4;
                cout << index << endl;
                //cout << index << " / " << (mVboMeshVerticesLen - 1) << endl;
            }
            cout << index << endl;
            index = (mDiverVerticesTubeLen * (i+1)) + (mDiverVerticesCapLen * i);
            //index += (mDiverVerticesCapLen * (i+1));
            cout << "CAP :" << index << endl;
            
            //continue;
            
            
            
            v0 = index;
            v1 = v0 + 1;
            v2 = v0 + 2;
            v3 = v2 + 1;
            
            v4 = v0 + 4;
            v5 = v4 + 1;
            v6 = v4 + 2;
            v7 = v6 + 1;
            //continue;
            
            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);
            
            indices.push_back(v2);
            indices.push_back(v3);
            indices.push_back(v1);
            
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
            
            /*
            indices.push_back(v4);
            indices.push_back(v5);
            indices.push_back(v6);
            
            indices.push_back(v6);
            indices.push_back(v7);
            indices.push_back(v5);
             */
            
            /*
            cout << "   " << v0 << endl;
            cout << "   " << v1 << endl;
            cout << "   " << v2 << endl;
            cout << "   " << v3 << endl;
            
            
            cout << "   " << v4 << endl;
            cout << "   " << v5 << endl;
            cout << "   " << v6 << endl;
            cout << "   " << v7 << endl;
            */
            
            
            

            
            
            // push caps
            //v0 =
        }
        
        //cout << indices.size() << " / " << mVboMeshIndicesLen << endl;
        
        mVboMesh.bufferIndices(indices);
#ifdef CELL_DEBUG_DRAW_MESH_INDICES
        mVboMeshVerticesCopy.resize(mVboMesh.getNumVertices());
#endif
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
#ifdef CELL_DEBUG_DRAW_MESH_INDICES
        static const Vec2f meshIndexStringPos(0,0);
        static const float meshIndexStringScale(0.00625f);
        
        gl::enableAlphaBlending();
        gl::enableAlphaTest();
        glColor3f(1, 1, 1);
        
        int i = -1;
        while(++i < mVboMeshVerticesCopy.size()){
            const Vec3f& pos = mVboMeshVerticesCopy[i];
            glPushMatrix();
    
            glTranslatef(pos.x + ((i%2==0) ? 0.125f : 0.0f), pos.y + 0.125f, pos.z);
            glScalef(-meshIndexStringScale,-meshIndexStringScale,meshIndexStringScale);
            gl::drawString(toString(i),meshIndexStringPos);
            glPopMatrix();
        }
        
        gl::disableAlphaBlending();
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
        
#ifdef CELL_DEBUG_DRAW_MESH_INDICES
        vector<Vec3f>::iterator vbCopyItr = mVboMeshVerticesCopy.begin();
#endif
        int k = 0;
        cout << "BEGIN" << endl;
        cout << vbItr.getIndex() << endl;
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

#ifdef CELL_DEBUG_DRAW_MESH_INDICES
                /*------START------*/
                
                // top
                vbItr.setPosition(x0, y0, point.z);
                vbCopyItr->set(*(vbItr.getPositionPointer()));
                ++vbCopyItr;
                ++vbItr;
                
                vbItr.setPosition(x1, y0, point.z);
                vbCopyItr->set(*(vbItr.getPositionPointer()));
                ++vbCopyItr;
                ++vbItr;
                
                // bottom
                vbItr.setPosition(x0, y1, point.z);
                vbCopyItr->set(*(vbItr.getPositionPointer()));
                ++vbCopyItr;
                ++vbItr;
                
                vbItr.setPosition(x1, y1, point.z);
                vbCopyItr->set(*(vbItr.getPositionPointer()));
                ++vbCopyItr;
                ++vbItr;
                
                
                // copy top for left / right
                vbItr.setPosition(x0, y0, point.z);
                vbCopyItr->set(*(vbItr.getPositionPointer()));
                ++vbCopyItr;
                ++vbItr;
                
                vbItr.setPosition(x1, y0, point.z);
                vbCopyItr->set(*(vbItr.getPositionPointer()));
                ++vbCopyItr;
                ++vbItr;
                
                // copy bottom for left / right
                vbItr.setPosition(x0, y1, point.z);
                vbCopyItr->set(*(vbItr.getPositionPointer()));
                ++vbCopyItr;
                ++vbItr;
                
                vbItr.setPosition(x1, y1, point.z);
                vbCopyItr->set(*(vbItr.getPositionPointer()));
                ++vbCopyItr;
                ++vbItr;
                
                /*-------END-------*/
#else
                /*------START------*/
                
                // top
                vbItr.setPosition(x0, y0, point.z);
                ++vbItr;
                
                vbItr.setPosition(x1, y0, point.z);
                ++vbItr;
                
                // bottom
                vbItr.setPosition(x0, y1, point.z);
                ++vbItr;
                
                vbItr.setPosition(x1, y1, point.z);
                ++vbItr;
                
                //continue;
                
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
                
                /*-------END-------*/
                
                k+=8;
                cout << vbItr.getIndex() << endl;
                //cout << k << endl;
#endif
            }
            
            
            
            cout << "CAP" << endl;
            
            //cout << "END" << endl;
            //cout << "CAP INDEX: " << k << endl;
            
            const Vec3f& end   = points[points.size() - 1];
            //cout << k << endl;
            
            // start
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
            
            k+=4;
            
            cout << vbItr.getIndex() << endl;
            
            continue;
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
