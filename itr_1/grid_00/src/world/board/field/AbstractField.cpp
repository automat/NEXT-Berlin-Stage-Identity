#include "world/board/field/AbstractField.h"
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>


#include "world/board/path/PathSlice.h"

using namespace boost::assign;

/*--------------------------------------------------------------------------------------------*/
//  Constructor
/*--------------------------------------------------------------------------------------------*/

AbstractField::AbstractField(const Vec3f& pos, int size, int numPathSlices) :
    mActive(true),mPos(pos),mSize(size),mSurfaceNumSlices(numPathSlices),mSurfaceOffset(0){
        mTransform.translate(mPos);
}

AbstractField::~AbstractField(){
    freeData();
}

/*--------------------------------------------------------------------------------------------*/
//  Geometry
/*--------------------------------------------------------------------------------------------*/

void AbstractField::fold(int index){
    // copy to target buffer
    copy(mDiverIndicesFolded.begin(),
         mDiverIndicesFolded.end(),
         mMeshIndexBuffer.begin() + index * mDiverIndicesLen);
}

void AbstractField::unfold(int index){
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
//  Internal
/*--------------------------------------------------------------------------------------------*/


void AbstractField::freeData(){
    while(!mDivers.empty()) delete mDivers.back(), mDivers.pop_back();
}

void AbstractField::reset_Internal(){
    freeData();
    mPathSurface.set(mPos,mSurfaceNumSlices,mSize);
    
    float offset,speed,length,height;
    int   numPoints = mDiverUnitNumPoints * mPathSurface.getSize();
    
    int i = -1;
    while(++i < mSurfaceNumSlices){
        offset = Rand::randFloat(mDiverOffsetMin, mDiverOffsetMax);
        speed  = Rand::randFloat(mDiverSpeedMin,  mDiverSpeedMax);
        length = Rand::randFloat(mDiverLengthMin, mDiverLengthMax);
        height = Rand::randFloat(mDiverHeightMin, mDiverHeightMax);

        mDivers += new Diver(mPathSurface.getSlicePtr(i),numPoints,offset,speed,length,height);
    }
}

/*--------------------------------------------------------------------------------------------*/
//  Update
/*--------------------------------------------------------------------------------------------*/

void AbstractField::updatePathSurface(Oscillator* osc, float t){
    mPathSurface.update(osc, mPos + mSurfaceDisplacement, mSurfaceDensity, mSurfaceAmplitude, mSurfaceOffset);
}

void AbstractField::updateDivers(){
    for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
        (*itr)->update();
    }
}

/*--------------------------------------------------------------------------------------------*/
//  Debug Draw
/*--------------------------------------------------------------------------------------------*/

void AbstractField::debugDrawArea_Internal(){
    static const Vec3f zero;
    
    static const float verticesArea[12] = {
        -0.5f, 0,-0.5f,
        0.5f,  0,-0.5f,
        0.5f,  0, 0.5f,
        -0.5f, 0, 0.5f
    };
    
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    int i = -1;
    while (++i < mSize) {
        glPushMatrix();
        glTranslatef(0, 0, i);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &verticesArea[0]);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glVertexPointer(3, GL_FLOAT, 0, &zero.x);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }
    glPopMatrix();
}

void AbstractField::debugDrawPathSurface(){
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    mPathSurface.debugDraw();
    glPopMatrix();
}

void AbstractField::debugDrawDivers(){
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
        (*itr)->debugDraw();
    }
    glPopMatrix();
}