//
//  EdgeCubeMatrix.cpp
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 03/12/13.
//
//

#include "EdgeCubeMatrix.h"
#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>

EdgeCubeMatrix::EdgeCubeMatrix() :
mDrawEdgesOff(true),
mDrawPoints(true),
mPointSize(0.005f){
    this->initDebugFont();
    this->initBatchesOff();
}

EdgeCubeMatrix::~EdgeCubeMatrix(){
    delete mBatchEdgesOffX;
    delete mBatchEdgesOffY;
    delete mBatchEdgesOffZ;
    delete mBatchEdgesOffXD;
    delete mBatchEdgesOffYD;
    delete mBatchEdgesOffZD;
    
    this->clear();
}

void EdgeCubeMatrix::initDebugFont(){
    mDebugFontScale   = 0.0015f;
    mDebugFont        = ci::Font("Arial",12);
    mTextureDebugFont = ci::gl::TextureFont::create(mDebugFont);
}

void EdgeCubeMatrix::setSize(int sizeX, int sizeY, int sizeZ){
    this->setSize_Internal(sizeX, sizeY, sizeZ);
}

void EdgeCubeMatrix::setSize(int sizeXYZ){
    this->setSize_Internal(sizeXYZ, sizeXYZ, sizeXYZ);
}

void EdgeCubeMatrix::setSize_Internal(int sizeX, int sizeY, int sizeZ){
    mEdgesSizeOnLast.set(-1, -1);
    mEdgesSizeOffLast.set(-1, -1);
    
    mNumPointsX = std::max(2,sizeX);
    mNumPointsY = std::max(2,sizeY);
    mNumPointsZ = std::max(2,sizeZ);
    mNumPoints  = mNumPointsX * mNumPointsY * mNumPointsZ;
    
    mNumCubesX = mNumPointsX_1 = mNumPointsX - 1;
    mNumCubesY = mNumPointsY_1 = mNumPointsY - 1;
    mNumCubesZ = mNumPointsZ_1 = mNumPointsZ - 1;
    mNumCubes  = mNumCubesX * mNumCubesY * mNumCubesZ;
    
    this->clear();
    this->build();
}

void EdgeCubeMatrix::initBatchesOff(){
    mBatchEdgesOffX = new TriMeshBatch();
    mBatchEdgesOffY = new TriMeshBatch();
    mBatchEdgesOffZ = new TriMeshBatch();
    mBatchEdgesOffXD= new TriMeshBatch();
    mBatchEdgesOffYD= new TriMeshBatch();
    mBatchEdgesOffZD= new TriMeshBatch();
}

void EdgeCubeMatrix::clear(){
    mPoints.clear();
    mEdgesX.clear();
    mEdgesY.clear();
    mEdgesZ.clear();
    mEdgesXD.clear();
    mEdgesYD.clear();
    mEdgesZD.clear();
}

/*--------------------------------------------------------------------------------*/

void EdgeCubeMatrix::build(){
    
    /*--------------------------------------------------------------------------------*/
    // Setup Points
    /*--------------------------------------------------------------------------------*/
    
    int ix,iy,iz;
    float nx,ny,nz;
    float cubeSize = 0.25f;
    float rangeX = cubeSize * (float)mNumCubesX;
    float rangeY = cubeSize * (float)mNumCubesY;
    float rangeZ = cubeSize * (float)mNumCubesZ;
    
    ix = -1;
    while (++ix < mNumPointsX) {
        nx = (float)ix / (float)(mNumPointsX_1);
        iy = -1;
        while (++iy < mNumPointsY) {
            ny = (float)iy / (float)(mNumPointsY_1);
            iz = -1;
            while (++iz < mNumPointsZ) {
                nz = (float)iz / (float)(mNumPointsZ_1);
                mPoints.push_back(ci::Vec3f((-0.5  + nx) * rangeX,
                                            (-0.5f + ny) * rangeY,
                                            (-0.5f + nz) * rangeZ));
            }
        }
    }
    
    
    /*--------------------------------------------------------------------------------*/
    // Setup Edges aka Bulbs
    /*--------------------------------------------------------------------------------*/
    
    //int index;
    //int indexCube;
    
    //bottom
    int indexCubeVertex0;
    int indexCubeVertex1;
    int indexCubeVertex2;
    int indexCubeVertex3;
    
    //top
    int indexCubeVertex4;
    int indexCubeVertex5;
    int indexCubeVertex6;
    int indexCubeVertex7;
    
    int numEdgesZ  = mNumPointsZ_1;
    int numEdgesY  = mNumPointsY_1;
    int numEdgesX  = mNumPointsX_1;
    
    int ix1;
    int iy1;
    
    int numEdgesXVertices = 0;
    int numEdgesYVertices = 0;
    int numEdgesZVertices = 0;
    int numEdgesXDVertices = 0;
    int numEdgesYDVertices = 0;
    int numEdgesZDVertices = 0;
    
    int numEdgesXIndices  = 0;
    int numEdgesYIndices  = 0;
    int numEdgesZIndices  = 0;
    int numEdgesXDIndices  = 0;
    int numEdgesYDIndices  = 0;
    int numEdgesZDIndices  = 0;
    
    
    
    ix = -1;
    while (++ix < mNumPointsX) {
        ix1 = ix + 1;
        iy  = -1;
        while (++iy < mNumPointsY) {
            iy1 = iy + 1;
            iz  = -1;
            while (++iz < mNumPointsZ) {
                
                //index     = ix * mNumPointsY * mNumPointsZ + iy * mNumPointsZ + iz;
                //indexCube = ix * numEdgesY * numEdgesZ + iy * numEdgesZ + iz;
                
                //bottom
                indexCubeVertex0 = ix  * mNumPointsY * mNumPointsZ + iy * mNumPointsZ + iz;
                indexCubeVertex1 = ix1 * mNumPointsY * mNumPointsZ + iy * mNumPointsZ + iz;
                indexCubeVertex2 = indexCubeVertex0 + 1;
                indexCubeVertex3 = indexCubeVertex1 + 1;
                
                //top
                indexCubeVertex4 = ix  * mNumPointsY * mNumPointsZ + iy1 * mNumPointsZ + iz;
                indexCubeVertex5 = ix1 * mNumPointsY * mNumPointsZ + iy1 * mNumPointsZ + iz;
                indexCubeVertex6 = indexCubeVertex4 + 1;
                indexCubeVertex7 = indexCubeVertex5 + 1;
                
                
                //bulbs X axis
                if (ix < numEdgesX){
                    mEdgesX.push_back(Edge(&mPoints[indexCubeVertex0],
                                                &mPoints[indexCubeVertex1]));
                    numEdgesXVertices += mEdgesX.back().getNumVertices();
                    numEdgesXIndices  += mEdgesX.back().getNumIndices();
                }
                
                //bulbs Z axis
                if (iz < numEdgesZ) {
                    
                    mEdgesZ.push_back(Edge(&mPoints[indexCubeVertex0],
                                                &mPoints[indexCubeVertex2]));
                    numEdgesZVertices += mEdgesZ.back().getNumVertices();
                    numEdgesZIndices  += mEdgesZ.back().getNumIndices();
                }
                
                //bulbs Y axis
                if (iy < numEdgesY) {
                    mEdgesY.push_back(Edge(&mPoints[indexCubeVertex0],
                                                &mPoints[indexCubeVertex4]));
                    numEdgesYVertices += mEdgesY.back().getNumVertices();
                    numEdgesYIndices  += mEdgesY.back().getNumIndices();
                }
                
                //bulbs diagonal X Axis
                if( ix < numEdgesX && iy < numEdgesY){
                    mEdgesXD.push_back(Edge(&mPoints[indexCubeVertex0],  //4
                                                 &mPoints[indexCubeVertex5]));//1
                    numEdgesXDVertices += mEdgesXD.back().getNumVertices();
                    numEdgesXDIndices  += mEdgesXD.back().getNumIndices();
                }
                
                //bulbs diagonal TOP / BOTTOM
                if( ix < numEdgesX && iz < numEdgesZ){
                    mEdgesZD.push_back(Edge(&mPoints[indexCubeVertex1],  //0
                                                 &mPoints[indexCubeVertex2]));//3
                    numEdgesZDVertices += mEdgesZD.back().getNumVertices();
                    numEdgesZDIndices  += mEdgesZD.back().getNumIndices();
                }
                
                //bulbs diagonal Z axes
                if( iz < numEdgesZ && iy < numEdgesY){
                    mEdgesYD.push_back(Edge(&mPoints[indexCubeVertex0],
                                                 &mPoints[indexCubeVertex6]));
                    numEdgesYDVertices += mEdgesYD.back().getNumVertices();
                    numEdgesYDIndices  += mEdgesYD.back().getNumIndices();
                }
                
                //std::cout << indexCube << " : " << indexCubeVertex0 << " " << indexCubeVertex1 << " " << indexCubeVertex2 << " " << indexCubeVertex3 <<  std::endl;
                //std::cout << indexCube << " : " << indexCubeVertex4 << " " << indexCubeVertex5 << " " << indexCubeVertex6 << " " << indexCubeVertex7 <<  std::endl;
            }
        }
    }
    
    mBatchEdgesOffX->resize(numEdgesXVertices, numEdgesXIndices);
    mBatchEdgesOffY->resize(numEdgesYVertices, numEdgesYIndices);
    mBatchEdgesOffZ->resize(numEdgesZVertices, numEdgesZIndices);
    
    mBatchEdgesOffXD->resize(numEdgesXDVertices, numEdgesXDIndices);
    mBatchEdgesOffYD->resize(numEdgesYDVertices, numEdgesYDIndices);
    mBatchEdgesOffZD->resize(numEdgesZDVertices, numEdgesZDIndices);
    
    /*--------------------------------------------------------------------------------*/
    // Setup Light Cubes
    /*--------------------------------------------------------------------------------*/
    
    //bottom
    int indexBulb00;
    int indexBulb01;
    int indexBulb02;
    int indexBulb03;
    
    //top
    int indexBulb04;
    int indexBulb05;
    int indexBulb06;
    int indexBulb07;
    
    //sides
    int indexBulb08;
    int indexBulb09;
    int indexBulb10;
    int indexBulb11;
    
    //bottom/top diagonal
    int indexBulb12;
    int indexBulb13;
    
    //sides diagonal
    int indexBulb14;
    int indexBulb15;
    int indexBulb16;
    int indexBulb17;
    
    mNumPointsY_1Z_1 = mNumPointsY_1 * mNumPointsZ_1;
    mNumPointsYZ     = mNumPointsY   * mNumPointsZ;
    mNumPointsYZ_1   = mNumPointsY   * mNumPointsZ_1;
    mNumPointsY_1Z   = mNumPointsY_1 * mNumPointsZ;

    ix = -1;
    while (++ix < numEdgesX) {
        ix1 = ix + 1;
        iy  = -1;
        while (++iy < numEdgesY) {
            iy1 = iy + 1;
            iz  = -1;
            while (++iz < numEdgesZ) {
                //bottom trbl
                indexBulb00 = ix * mNumPointsYZ   + iy * mNumPointsZ  + iz;
                indexBulb03 = ix * mNumPointsYZ_1 + iy * mNumPointsZ_1 + iz;
                indexBulb02 = indexBulb00 + 1;
                indexBulb01 = indexBulb03 + mNumPointsYZ_1;
                
                //top trbl
                indexBulb04 = ix * mNumPointsYZ   + iy1 * mNumPointsZ  + iz;
                indexBulb07 = ix * mNumPointsYZ_1 + iy1 * mNumPointsZ_1 + iz;
                indexBulb06 = indexBulb04 + 1;
                indexBulb05 = indexBulb07 + mNumPointsYZ_1;
                
                //sides cw
                indexBulb08 = ix  * mNumPointsY_1Z + iy * mNumPointsZ + iz;
                indexBulb09 = ix1 * mNumPointsY_1Z + iy * mNumPointsZ + iz;
                indexBulb10 = indexBulb09 + 1;
                indexBulb11 = indexBulb08 + 1;
                
                //bottom/top diagonal
                indexBulb12 = ix * mNumPointsYZ_1 + iy  * mNumPointsZ_1 + iz;
                indexBulb13 = ix * mNumPointsYZ_1 + iy1 * mNumPointsZ_1 + iz;
                
                //sides diagonal;
                indexBulb14 = indexBulb08;
                indexBulb17 = ix * mNumPointsY_1Z_1 + iy * mNumPointsZ_1 + iz;
                indexBulb16 = indexBulb14 + 1;
                indexBulb15 = indexBulb17 + mNumPointsY_1Z_1;
                
                
                /*
                 std::cout << "---" << std::endl;
                 std::cout << "- " << indexCube << std::endl;
                 std::cout << indexBulb00 << std::endl;
                 std::cout << indexBulb01 << std::endl;
                 std::cout << indexBulb02 << std::endl;
                 std::cout << indexBulb03 << std::endl;
                 std::cout << "- " << std::endl;
                 std::cout << indexBulb04 << std::endl;
                 std::cout << indexBulb05 << std::endl;
                 std::cout << indexBulb06 << std::endl;
                 std::cout << indexBulb07 << std::endl;
                 std::cout << "- " << std::endl;
                 std::cout << indexBulb08 << std::endl;
                 std::cout << indexBulb09 << std::endl;
                 std::cout << indexBulb10 << std::endl;
                 std::cout << indexBulb11 << std::endl;
                 std::cout << "- " << std::endl;
                 std::cout << indexBulb12 << std::endl;
                 std::cout << indexBulb13 << std::endl;
                 std::cout << "- " << std::endl;
                 std::cout << indexBulb14 << std::endl;
                 std::cout << indexBulb15 << std::endl;
                 std::cout << indexBulb16 << std::endl;
                 std::cout << indexBulb17 << std::endl;
                 std::cout << "---" << std::endl;
                 */
                
                
                
                mCubes.push_back(EdgeCube(ix,iy,iz));
                mCubes.back().set(&( mEdgesX[indexBulb00]), &( mEdgesZ[indexBulb01]), &( mEdgesX[indexBulb02]), &( mEdgesZ[indexBulb03]),
                                  &( mEdgesX[indexBulb04]), &( mEdgesZ[indexBulb05]), &( mEdgesX[indexBulb06]), &( mEdgesZ[indexBulb07]),
                                  &( mEdgesY[indexBulb08]), &( mEdgesY[indexBulb09]), &( mEdgesY[indexBulb10]), &( mEdgesY[indexBulb11]),
                                  &(mEdgesZD[indexBulb12]), &(mEdgesZD[indexBulb13]),
                                  &(mEdgesXD[indexBulb14]), &(mEdgesYD[indexBulb15]), &(mEdgesXD[indexBulb16]), &(mEdgesYD[indexBulb17]));
            }
        }
    }
    
}

/*--------------------------------------------------------------------------------*/

void EdgeCubeMatrix::switchOn(){
    this->switchOnEdges(mEdgesX);
    this->switchOnEdges(mEdgesY);
    this->switchOnEdges(mEdgesZ);
    this->switchOnEdges(mEdgesXD);
    this->switchOnEdges(mEdgesYD);
    this->switchOnEdges(mEdgesZD);
}

void EdgeCubeMatrix::switchOff(){
    this->switchOffEdges(mEdgesX);
    this->switchOffEdges(mEdgesY);
    this->switchOffEdges(mEdgesZ);
    this->switchOffEdges(mEdgesXD);
    this->switchOffEdges(mEdgesYD);
    this->switchOffEdges(mEdgesZD);
}

void EdgeCubeMatrix::switchRandom(float triggerTolerance){
    this->switchRandomEdges(mEdgesX,  triggerTolerance);
    this->switchRandomEdges(mEdgesY,  triggerTolerance);
    this->switchRandomEdges(mEdgesZ,  triggerTolerance);
    this->switchRandomEdges(mEdgesXD, triggerTolerance);
    this->switchRandomEdges(mEdgesYD, triggerTolerance);
    this->switchRandomEdges(mEdgesZD, triggerTolerance);
}

void EdgeCubeMatrix::switchRandomEdges(std::vector<Edge>& bulbs, float triggerTolerance){
    for (std::vector<Edge>::iterator itr = bulbs.begin(); itr != bulbs.end(); ++itr) {
        itr->switchRandom(triggerTolerance);
    }
}

void EdgeCubeMatrix::switchOffEdges(std::vector<Edge>& edges){
    for (std::vector<Edge>::iterator itr = edges.begin(); itr != edges.end(); ++itr) {
        itr->switchOff();
    }
}

void EdgeCubeMatrix::switchOnEdges(std::vector<Edge>& edges){
    for (std::vector<Edge>::iterator itr = edges.begin(); itr != edges.end(); ++itr) {
        itr->switchOn();
    }
}

/*--------------------------------------------------------------------------------*/

void EdgeCubeMatrix::setEdgeSizeOn(const ci::Vec2f& size){
    if(mEdgesSizeOnLast == size)return;
    
    this->setSizeOnEdges(mEdgesX, size);
    this->setSizeOnEdges(mEdgesY, size);
    this->setSizeOnEdges(mEdgesZ, size);
    this->setSizeOnEdges(mEdgesXD,size);
    this->setSizeOnEdges(mEdgesYD,size);
    this->setSizeOnEdges(mEdgesZD,size);
    
    mEdgesSizeOnLast = size;
}

void EdgeCubeMatrix::setEdgeSizeOff(const ci::Vec2f& size){
    if(mEdgesSizeOffLast == size)return;
    
    this->setSizeOffEdges(mEdgesX, size);
    this->setSizeOffEdges(mEdgesY, size);
    this->setSizeOffEdges(mEdgesZ, size);
    this->setSizeOffEdges(mEdgesXD,size);
    this->setSizeOffEdges(mEdgesYD,size);
    this->setSizeOffEdges(mEdgesZD,size);
    
    mEdgesSizeOffLast = size;
}

void EdgeCubeMatrix::setSizeOffEdges(std::vector<Edge> &bulbs, const ci::Vec2f& size){
    for (std::vector<Edge>::iterator itr = bulbs.begin(); itr!=bulbs.end(); ++itr) {
        itr->setSizeOff(size);
        itr->updateGeometry();
    }
}

void EdgeCubeMatrix::setSizeOnEdges(std::vector<Edge> &bulbs, const ci::Vec2f& size){
    for (std::vector<Edge>::iterator itr = bulbs.begin(); itr!=bulbs.end(); ++itr) {
        itr->setSizeOn(size);
        itr->updateGeometry();
    }
}

/*--------------------------------------------------------------------------------*/

void EdgeCubeMatrix::drawOcclusive(){
    // Draw Points
    
    if(mDrawPoints){
        ci::Vec3f size(mPointSize,mPointSize,mPointSize);
        for (std::vector<ci::Vec3f>::iterator itr = mPoints.begin(); itr != mPoints.end(); ++itr) {
            ci::gl::drawSphere(*itr, mPointSize, 15);
            //ci::gl::drawCube(*itr, size);
            //ci::gl::drawColorCube(*itr,size);
        }
    }
    
    if(!mDrawEdgesOff)return;
    
    this->drawBatch(mBatchEdgesOffX);
    this->drawBatch(mBatchEdgesOffY);
    this->drawBatch(mBatchEdgesOffZ);
    this->drawBatch(mBatchEdgesOffXD);
    this->drawBatch(mBatchEdgesOffYD);
    this->drawBatch(mBatchEdgesOffZD);
}

void EdgeCubeMatrix::update(){
    
    if(!mDrawEdgesOff)return;
    this->putBatch(mEdgesX,  mBatchEdgesOffX);
    this->putBatch(mEdgesY,  mBatchEdgesOffY);
    this->putBatch(mEdgesZ,  mBatchEdgesOffZ);
    this->putBatch(mEdgesXD, mBatchEdgesOffXD);
    this->putBatch(mEdgesYD, mBatchEdgesOffYD);
    this->putBatch(mEdgesZD, mBatchEdgesOffZD);
}

void EdgeCubeMatrix::putBatch(std::vector<Edge> &edge, TriMeshBatch *batch){
    batch->reset();
    for (std::vector<Edge>::iterator itr = edge.begin(); itr!=edge.end(); ++itr) {
        batch->put(*itr);
    }
}

void EdgeCubeMatrix::drawBatch(TriMeshBatch *batch){
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &batch->getVertices()[0].x);
    glNormalPointer(GL_FLOAT, 0, &batch->getNormals()[0].x);
    glDrawElements(GL_TRIANGLES, batch->getNumIndices(), GL_UNSIGNED_INT, &batch->getIndices()[0]);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}


void EdgeCubeMatrix::drawEmissive(){
    for (std::vector<Edge>::iterator itr = mEdgesX.begin(); itr!=mEdgesX.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<Edge>::iterator itr = mEdgesY.begin(); itr!=mEdgesY.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<Edge>::iterator itr = mEdgesZ.begin(); itr!=mEdgesZ.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<Edge>::iterator itr = mEdgesXD.begin(); itr!=mEdgesXD.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<Edge>::iterator itr = mEdgesYD.begin(); itr!=mEdgesYD.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<Edge>::iterator itr = mEdgesZD.begin(); itr!=mEdgesZD.end(); ++itr) {
        itr->drawEmissive();
    }
}

/*--------------------------------------------------------------------------------*/

void EdgeCubeMatrix::applyPattern(const EdgePattern &pattern){
    EdgeCube* cube = this->getEdgeCube(pattern.pos.x,
                                       pattern.pos.y,
                                       pattern.pos.z);
    if(!cube)return;
    int i = -1;
    while (++i < EdgeCube::NUM_EDGES) {
        if(bool(pattern[i]))(*cube)[i]->switchOn(); // just switch on
    }
}

void EdgeCubeMatrix::applyPatternSeq(const EdgePatternSequence& seq){
    const std::vector<EdgePattern>& patterns = seq.getPatterns();
    int i;
    EdgeCube* cube;
    ci::Vec3f index;
    for(std::vector<EdgePattern>::const_iterator itr = patterns.begin();
        itr != patterns.end();
        ++itr){
        index = seq.pos + itr->pos;
        cube  = this->getEdgeCube(index.x,index.y,index.z);
        if(!cube)continue;
        i = -1;
        while (++i < EdgeCube::NUM_EDGES) {
            if(bool((*itr)[i]))(*cube)[i]->switchOn();
        }
    }
}

void EdgeCubeMatrix::applyPatternSeqs(const std::vector<EdgePatternSequence>& seqs){
    int i;
    EdgeCube* cube;
    ci::Vec3f index;
    for (std::vector<EdgePatternSequence>::const_iterator itrSeq = seqs.begin();
         itrSeq != seqs.end();
         ++itrSeq) {
        const std::vector<EdgePattern>& patterns = itrSeq->getPatterns();
        for (std::vector<EdgePattern>::const_iterator itr = patterns.begin();
             itr != patterns.end();
             ++itr) {
            index = itrSeq->pos + itr->pos;
            cube  = this->getEdgeCube(index.x, index.y, index.z);
            if(!cube)continue;
            i = -1;
            while (++i < EdgeCube::NUM_EDGES) {
                if(bool((*itr)[i]))(*cube)[i]->switchOn();
            }
        }
    }
}

/*--------------------------------------------------------------------------------*/

EdgeCube* EdgeCubeMatrix::getEdgeCube(int x, int y, int z){
    int numCubesX_1 = mNumCubesX - 1;
    int numCubesY_1 = mNumCubesY - 1;
    int numCubesZ_1 = mNumCubesZ - 1;
    
    if ( x < 0 || x > numCubesX_1 ||
         y < 0 || y > numCubesY_1 ||
         z < 0 || z > numCubesZ_1 ) {
        return NULL;
    }
    
    return &mCubes[x * mNumCubesY * mNumCubesZ + y * mNumCubesZ + z];
}

EdgeCube* EdgeCubeMatrix::getEdgeCube(int x, int y, int z, int ax, int ay, int az){
    int numCubesX_1 = mNumCubesX - 1;
    int numCubesY_1 = mNumCubesY - 1;
    int numCubesZ_1 = mNumCubesZ - 1;
    
    int indexXAdded = x + ax;
    int indexYAdded = y + ay;
    int indexZAdded = z + az;
    
    if (ax > numCubesX_1 ||
        ay > numCubesY_1 ||
        az > numCubesZ_1 ||
        indexXAdded < 0 || indexXAdded > numCubesX_1 ||
        indexYAdded < 0 || indexYAdded > numCubesY_1 ||
        indexZAdded < 0 || indexZAdded > numCubesZ_1) {
        return NULL;
    }
    
    return &mCubes[indexXAdded * mNumCubesY * mNumCubesZ + indexYAdded * mNumCubesZ + indexZAdded];
}

EdgeCube* EdgeCubeMatrix::getEdgeCube(EdgeCube &baseCube, int ax, int ay, int az){
    return this->getEdgeCube(baseCube.getIX(), baseCube.getIY(), baseCube.getIZ(),
                              ax, ay, az);
}

/*--------------------------------------------------------------------------------*/

void EdgeCubeMatrix::drawDebugBulb(std::vector<Edge> &bulbs){
    const static ci::Vec2f zero = ci::Vec2f::zero();
    ci::Vec3f mid;
    
    int i = -1;
    while (++i < bulbs.size()) {
        bulbs[i].getMid(&mid);
        glPushMatrix();
        glTranslatef(mid.x,mid.y,mid.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}

void EdgeCubeMatrix::drawDebugPoints(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::gl::color(1, 1, 1);
    int i = -1;
    while (++i < mPoints.size()) {
       glPushMatrix();
        ci::Vec3f& point = mPoints[i];
        glTranslatef(point.x, point.y, point.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}


void EdgeCubeMatrix::drawDebugCubes(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::Vec3f sum;
    ci::Vec3f mid;
    
    int numEdges = 18;
    
    ci::gl::color(1, 0, 0);
    int i,j;
    i = -1;
    while (++i < mCubes.size()) {
        sum.x = 0;
        sum.y = 0;
        sum.z = 0;
        
        j = -1;
        while (++j < numEdges) {
            mCubes[i][j]->getMid(&mid);
            sum += mid;
        }
        
        sum/=numEdges;
        
        glPushMatrix();
        glTranslatef(sum.x,sum.y,sum.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}
