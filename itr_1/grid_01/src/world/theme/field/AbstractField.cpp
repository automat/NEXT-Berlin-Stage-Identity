#include "world/theme/field/AbstractField.h"
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "Config.h"

#include "world/theme/path/PathSlice.h"
#include "util/ColorUtil.h"

using namespace boost::assign;

/*--------------------------------------------------------------------------------------------*/
//  Constructor
/*--------------------------------------------------------------------------------------------*/

AbstractField::AbstractField(const Vec3f& pos, int size, int numPathSlices) :
    mActive(true),mPos(pos),mSize(size),mSurfaceNumSlices(numPathSlices),mSurfaceOffset(0){
        mTransform.translate(mPos);
        
        //
        //  Setup shared layout
        //
        mMeshLayout.setDynamicPositions();
        mMeshLayout.setStaticNormals();
        mMeshLayout.setDynamicIndices();
        
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

void AbstractField::updateMesh(){
    if(!mActive){
        return;
    }
    
    
    static const Vec3f up(0,1,0),down(0,-1,0);
    static const Vec3f left(-1,0,0),right(1,0,0);
    static const Vec3f front(0,0,-1),back(0,0,1);
    Vec3f tempNormal;
    
    // update geometry
    float diverWidth_2 = mSurfaceSliceWidth * 0.5f;
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
    
    
    updateMeshNormals();
    
    mMesh.bufferNormals(mMeshNormalBuffer);
    
    
}

void AbstractField::updateMeshNormals(){
    static const Vec3f zero(0,0,0);
    
    size_t size = mMeshIndexScheme.size();
    int i = -1;
    while (++i < size) {
        mMeshNormalBuffer[mMeshIndexScheme[i]].set(zero);
    }
    
    size_t numTriangles = size / 3;
    
    Vec3f e0,e1,normal;
    int index0,index1,index2;
    int i3;
    
    i = -1;
    while(++i < numTriangles){
        i3 = i * 3;
        index0 = mMeshIndexScheme[i3 + 0];
        index1 = mMeshIndexScheme[i3 + 1];
        index2 = mMeshIndexScheme[i3 + 2];
        e0.set(mMeshVertexBuffer[ index2 ] - mMeshVertexBuffer[ index0 ]);
        e1.set(mMeshVertexBuffer[ index1 ] - mMeshVertexBuffer[ index0 ]);
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

void AbstractField::drawMesh(){
    if(!mActive){
        return;
    }
    
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    gl::draw(mMesh);
    glPopMatrix();
}

void AbstractField::drawSurface(){
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    mPathSurface.drawSurface();
    glPopMatrix();
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
    mSurfaceSliceWidth = mPathSurface.getSliceWidth();
    
    float offset,speed,length,height;
    mDiverNumPoints = mDiverUnitNumPoints * mPathSurface.getSize();
    
    int i = -1;
    while(++i < mSurfaceNumSlices){
        offset = Rand::randFloat(mDiverOffsetMin, mDiverOffsetMax);
        speed  = Rand::randFloat(mDiverSpeedMin,  mDiverSpeedMax);
        length = Rand::randFloat(mDiverLengthMin, mDiverLengthMax);
        height = Rand::randFloat(mDiverHeightMin, mDiverHeightMax);

        mDivers += new Diver(mPathSurface.getSlicePtr(i),mDiverNumPoints,offset,speed,length,height);
    }
    
    mNumDivers            = mSurfaceNumSlices;
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
            /*
            // first quad
            v00 = index + 0; v01 = index + 2;
            v02 = index + 3; v03 = index + 1;
            // next quad
            v04 = index + 8; v05 = index + 10;
            v06 = index + 11;v07 = index + 9;
             */
            // first quad
            v01 = index + 2;
            v02 = index + 3;
            // next quad
            v05 = index + 10;
            v06 = index + 11;
            // top lower & upper triangle
            mMeshIndexScheme += v01,v02,v05;
            mMeshIndexScheme += v02,v06,v05;
        }
    }
    
    //
    //  Setup vbo mesh
    //
    
    mMesh.reset();
    mMesh = gl::VboMesh(mMeshVerticesLen,mMeshIndicesLen,mMeshLayout,GL_TRIANGLES);
    
    // unfold mesh
    i = -1;
    while (++i < mNumDivers) {
        unfold(i);
    }
    
    // put normals
    const Vec3f up(0,1,0),down(0,-1,0);
    const Vec3f left(-1,0,0),right(1,0,0);
    const Vec3f front(0,0,-1),back(0,0,1);
    
    i = -1;
    while (++i < mNumDivers) {
        j = 0;
        while (j < mDiverNumPoints) {
            mMeshNormalBuffer += down,down;
            mMeshNormalBuffer += up,up;
            mMeshNormalBuffer += left,right;
            mMeshNormalBuffer += left,right;
            
            ++j;
        }
        mMeshNormalBuffer += front,front,front,front;
        mMeshNormalBuffer += back,back,back,back;
        
    }

    mMesh.bufferNormals(mMeshNormalBuffer);
    addMeshColors();
    mMesh.bufferIndices(mMeshIndexBuffer);
    mMesh.unbindBuffers();
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
//  State
/*--------------------------------------------------------------------------------------------*/

void AbstractField::activate(){
    mActive = true;
    for (vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
        (*itr)->show();
    }
}

void AbstractField::deactivate(){
    mActive = false;
    for (vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
        (*itr)->hide();
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