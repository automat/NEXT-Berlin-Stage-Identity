//
//  LightCubeMatrix.cpp
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 03/12/13.
//
//

#include "LightCubeMatrix.h"
#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>

LightCubeMatrix::LightCubeMatrix() :
mDrawBulbsOff(true),
mDrawPoints(true),
mPointSize(0.005f),
mBulbSizeOffLast(-1),
mBulbSizeOnLast(-1){
    this->initDebugFont();
    this->initBatchesOff();
}

LightCubeMatrix::~LightCubeMatrix(){
    delete mBatchBulbsOffX;
    delete mBatchBulbsOffY;
    delete mBatchBulbsOffZ;
    delete mBatchBulbsOffXD;
    delete mBatchBulbsOffYD;
    delete mBatchBulbsOffZD;
    
    this->clear();
}

void LightCubeMatrix::initDebugFont(){
    mDebugFontScale   = 0.0015f;
    mDebugFont        = ci::Font("Arial",12);
    mTextureDebugFont = ci::gl::TextureFont::create(mDebugFont);
}

void LightCubeMatrix::setPointsNum(int numX, int numY, int numZ){
    this->setNumPoints_Internal(numX, numY, numZ);
    mBulbSizeOnLast  = -1;
    mBulbSizeOffLast = -1;
}

void LightCubeMatrix::setPointsNum(int numXYZ){
    this->setNumPoints_Internal(numXYZ, numXYZ, numXYZ);
}

void LightCubeMatrix::setNumPoints_Internal(int numX, int numY, int numZ){
    mNumPointsX = numX;
    mNumPointsY = numY;
    mNumPointsZ = numZ;
    mNumPoints  = mNumPointsX * mNumPointsY * mNumPointsZ;
    
    mNumCubesX = mNumPointsX_1 = mNumPointsX - 1;
    mNumCubesY = mNumPointsY_1 = mNumPointsY - 1;
    mNumCubesZ = mNumPointsZ_1 = mNumPointsZ - 1;
    mNumCubes  = mNumCubesX * mNumCubesY * mNumCubesZ;
    
    this->clear();
    this->build();
}

void LightCubeMatrix::initBatchesOff(){
    mBatchBulbsOffX = new TriMeshBatch();
    mBatchBulbsOffY = new TriMeshBatch();
    mBatchBulbsOffZ = new TriMeshBatch();
    mBatchBulbsOffXD= new TriMeshBatch();
    mBatchBulbsOffYD= new TriMeshBatch();
    mBatchBulbsOffZD= new TriMeshBatch();
}

void LightCubeMatrix::clear(){
    mPoints.clear();
    mBulbsX.clear();
    mBulbsY.clear();
    mBulbsZ.clear();
    mBulbsXD.clear();
    mBulbsYD.clear();
    mBulbsZD.clear();
}

/*--------------------------------------------------------------------------------*/

void LightCubeMatrix::build(){
    
    /*--------------------------------------------------------------------------------*/
    // Setup Points
    /*--------------------------------------------------------------------------------*/
    
    int ix,iy,iz;
    float nx,ny,nz;
    
    ix = -1;
    while (++ix < mNumPointsX) {
        nx = (float)ix / (float)(mNumPointsX_1);
        iy = -1;
        while (++iy < mNumPointsY) {
            ny = (float)iy / (float)(mNumPointsY_1);
            iz = -1;
            while (++iz < mNumPointsZ) {
                nz = (float)iz / (float)(mNumPointsZ_1);
                mPoints.push_back(ci::Vec3f((-0.5  + nx),
                                                (-0.5f + ny),
                                                (-0.5f + nz)));
            }
        }
    }
    
    
    /*--------------------------------------------------------------------------------*/
    // Setup Edges aka Bulbs
    /*--------------------------------------------------------------------------------*/
    
    int index;
    int indexCube;
    
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
    
    int numBulbsXVertices = 0;
    int numBulbsYVertices = 0;
    int numBulbsZVertices = 0;
    int numBulbsXDVertices = 0;
    int numBulbsYDVertices = 0;
    int numBulbsZDVertices = 0;
    
    int numBulbsXIndices  = 0;
    int numBulbsYIndices  = 0;
    int numBulbsZIndices  = 0;
    int numBulbsXDIndices  = 0;
    int numBulbsYDIndices  = 0;
    int numBulbsZDIndices  = 0;
    
    
    
    ix = -1;
    while (++ix < mNumPointsX) {
        ix1 = ix + 1;
        iy  = -1;
        while (++iy < mNumPointsY) {
            iy1 = iy + 1;
            iz  = -1;
            while (++iz < mNumPointsZ) {
                
                index     = ix * mNumPointsY * mNumPointsZ + iy * mNumPointsZ + iz;
                indexCube = ix * numEdgesY * numEdgesZ + iy * numEdgesZ + iz;
                
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
                    mBulbsX.push_back(LightBulb(&mPoints[indexCubeVertex0],
                                                &mPoints[indexCubeVertex1]));
                    numBulbsXVertices += mBulbsX.back().getNumVertices();
                    numBulbsXIndices  += mBulbsX.back().getNumIndices();
                }
                
                //bulbs Z axis
                if (iz < numEdgesZ) {
                    
                    mBulbsZ.push_back(LightBulb(&mPoints[indexCubeVertex0],
                                                &mPoints[indexCubeVertex2]));
                    numBulbsZVertices += mBulbsZ.back().getNumVertices();
                    numBulbsZIndices  += mBulbsZ.back().getNumIndices();
                }
                
                //bulbs Y axis
                if (iy < numEdgesY) {
                    mBulbsY.push_back(LightBulb(&mPoints[indexCubeVertex0],
                                                &mPoints[indexCubeVertex4]));
                    numBulbsYVertices += mBulbsY.back().getNumVertices();
                    numBulbsYIndices  += mBulbsY.back().getNumIndices();
                }
                
                //bulbs diagonal X Axis
                if( ix < numEdgesX && iy < numEdgesY){
                    mBulbsXD.push_back(LightBulb(&mPoints[indexCubeVertex4],
                                                 &mPoints[indexCubeVertex1]));
                    numBulbsXDVertices += mBulbsXD.back().getNumVertices();
                    numBulbsXDIndices  += mBulbsXD.back().getNumIndices();
                }
                
                //bulbs diagonal TOP / BOTTOM
                if( ix < numEdgesX && iz < numEdgesZ){
                    mBulbsZD.push_back(LightBulb(&mPoints[indexCubeVertex0],
                                                 &mPoints[indexCubeVertex3]));
                    numBulbsZDVertices += mBulbsZD.back().getNumVertices();
                    numBulbsZDIndices  += mBulbsZD.back().getNumIndices();
                }
                
                //bulbs diagonal Z axes
                if( iz < numEdgesZ && iy < numEdgesY){
                    mBulbsYD.push_back(LightBulb(&mPoints[indexCubeVertex0],
                                                 &mPoints[indexCubeVertex6]));
                    numBulbsYDVertices += mBulbsYD.back().getNumVertices();
                    numBulbsYDIndices  += mBulbsYD.back().getNumIndices();
                }
                
                //std::cout << indexCube << " : " << indexCubeVertex0 << " " << indexCubeVertex1 << " " << indexCubeVertex2 << " " << indexCubeVertex3 <<  std::endl;
                //std::cout << indexCube << " : " << indexCubeVertex4 << " " << indexCubeVertex5 << " " << indexCubeVertex6 << " " << indexCubeVertex7 <<  std::endl;
            }
        }
    }
    
    mBatchBulbsOffX->resize(numBulbsXVertices, numBulbsXIndices);
    mBatchBulbsOffY->resize(numBulbsYVertices, numBulbsYIndices);
    mBatchBulbsOffZ->resize(numBulbsZVertices, numBulbsZIndices);
    
    mBatchBulbsOffXD->resize(numBulbsXDVertices, numBulbsXDIndices);
    mBatchBulbsOffYD->resize(numBulbsYDVertices, numBulbsYDIndices);
    mBatchBulbsOffZD->resize(numBulbsZDVertices, numBulbsZDIndices);
    
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
                
                
                
                mCubes.push_back(LightCube(ix,iy,iz));
                mCubes.back().set(&( mBulbsX[indexBulb00]), &( mBulbsZ[indexBulb01]), &( mBulbsX[indexBulb02]), &( mBulbsZ[indexBulb03]),
                                  &( mBulbsX[indexBulb04]), &( mBulbsZ[indexBulb05]), &( mBulbsX[indexBulb06]), &( mBulbsZ[indexBulb07]),
                                  &( mBulbsY[indexBulb08]), &( mBulbsY[indexBulb09]), &( mBulbsY[indexBulb10]), &( mBulbsY[indexBulb11]),
                                  &(mBulbsZD[indexBulb12]), &(mBulbsZD[indexBulb13]),
                                  &(mBulbsXD[indexBulb14]), &(mBulbsYD[indexBulb15]), &(mBulbsXD[indexBulb16]), &(mBulbsYD[indexBulb17]));
            }
        }
    }
    
}

/*--------------------------------------------------------------------------------*/

void LightCubeMatrix::switchOn(){
    this->switchOnBulb(mBulbsX);
    this->switchOnBulb(mBulbsY);
    this->switchOnBulb(mBulbsZ);
    this->switchOnBulb(mBulbsXD);
    this->switchOnBulb(mBulbsYD);
    this->switchOnBulb(mBulbsZD);
}

void LightCubeMatrix::switchOff(){
    this->switchOffBulbs(mBulbsX);
    this->switchOffBulbs(mBulbsY);
    this->switchOffBulbs(mBulbsZ);
    this->switchOffBulbs(mBulbsXD);
    this->switchOffBulbs(mBulbsYD);
    this->switchOffBulbs(mBulbsZD);
}

void LightCubeMatrix::switchRandom(float triggerTolerance){
    this->switchRandomBulbs(mBulbsX,  triggerTolerance);
    this->switchRandomBulbs(mBulbsY,  triggerTolerance);
    this->switchRandomBulbs(mBulbsZ,  triggerTolerance);
    this->switchRandomBulbs(mBulbsXD, triggerTolerance);
    this->switchRandomBulbs(mBulbsYD, triggerTolerance);
    this->switchRandomBulbs(mBulbsZD, triggerTolerance);
}

void LightCubeMatrix::switchRandomBulbs(std::vector<LightBulb>& bulbs, float triggerTolerance){
    for (std::vector<LightBulb>::iterator itr = bulbs.begin(); itr != bulbs.end(); ++itr) {
        itr->switchRandom(triggerTolerance);
    }
}

void LightCubeMatrix::switchOffBulbs(std::vector<LightBulb>& bulbs){
    for (std::vector<LightBulb>::iterator itr = bulbs.begin(); itr != bulbs.end(); ++itr) {
        itr->switchOn();
    }
}

void LightCubeMatrix::switchOnBulb(std::vector<LightBulb>& bulbs){
    for (std::vector<LightBulb>::iterator itr = bulbs.begin(); itr != bulbs.end(); ++itr) {
        itr->switchOff();
    }
}

/*--------------------------------------------------------------------------------*/

void LightCubeMatrix::setBulbSizeOn(float size){
    if(mBulbSizeOnLast == size)return;
    
    this->setSizeOnBulbs(mBulbsX, size);
    this->setSizeOnBulbs(mBulbsY, size);
    this->setSizeOnBulbs(mBulbsZ, size);
    this->setSizeOnBulbs(mBulbsXD,size);
    this->setSizeOnBulbs(mBulbsYD,size);
    this->setSizeOnBulbs(mBulbsZD,size);
    
    mBulbSizeOnLast = size;
}

void LightCubeMatrix::setBulbSizeOff(float size){
    if(mBulbSizeOffLast == size)return;
    
    this->setSizeOffBulbs(mBulbsX, size);
    this->setSizeOffBulbs(mBulbsY, size);
    this->setSizeOffBulbs(mBulbsZ, size);
    this->setSizeOffBulbs(mBulbsXD,size);
    this->setSizeOffBulbs(mBulbsYD,size);
    this->setSizeOffBulbs(mBulbsZD,size);
    
    mBulbSizeOffLast = size;
}

void LightCubeMatrix::setSizeOffBulbs(std::vector<LightBulb> &bulbs, float size){
    for (std::vector<LightBulb>::iterator itr = bulbs.begin(); itr!=bulbs.end(); ++itr) {
        itr->setSizeOff(size);
        itr->updateGeometry();
    }
}

void LightCubeMatrix::setSizeOnBulbs(std::vector<LightBulb> &bulbs, float size){
    for (std::vector<LightBulb>::iterator itr = bulbs.begin(); itr!=bulbs.end(); ++itr) {
        itr->setSizeOff(size);
        itr->updateGeometry();
    }
}

/*--------------------------------------------------------------------------------*/

void LightCubeMatrix::drawOcclusive(){
    // Draw Points
    
    if(mDrawPoints){
        ci::Vec3f size(mPointSize,mPointSize,mPointSize);
        for (std::vector<ci::Vec3f>::iterator itr = mPoints.begin(); itr != mPoints.end(); ++itr) {
            ci::gl::drawSphere(*itr, mPointSize, 15);
            //ci::gl::drawCube(*itr, size);
            //ci::gl::drawColorCube(*itr,size);
        }
    }
    
    if(!mDrawBulbsOff)return;
    
    this->drawBatch(mBatchBulbsOffX);
    this->drawBatch(mBatchBulbsOffY);
    this->drawBatch(mBatchBulbsOffZ);
    this->drawBatch(mBatchBulbsOffXD);
    this->drawBatch(mBatchBulbsOffYD);
    this->drawBatch(mBatchBulbsOffZD);
}

void LightCubeMatrix::update(){
    
    if(!mDrawBulbsOff)return;
    this->putBatch(mBulbsX,  mBatchBulbsOffX);
    this->putBatch(mBulbsY,  mBatchBulbsOffY);
    this->putBatch(mBulbsZ,  mBatchBulbsOffZ);
    this->putBatch(mBulbsXD, mBatchBulbsOffXD);
    this->putBatch(mBulbsYD, mBatchBulbsOffYD);
    this->putBatch(mBulbsZD, mBatchBulbsOffZD);
}

void LightCubeMatrix::putBatch(std::vector<LightBulb> &bulbs, TriMeshBatch *batch){
    batch->reset();
    for (std::vector<LightBulb>::iterator itr = bulbs.begin(); itr!=bulbs.end(); ++itr) {
        batch->put(*itr);
    }
}

void LightCubeMatrix::drawBatch(TriMeshBatch *batch){
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &batch->getVertices()[0].x);
    glNormalPointer(GL_FLOAT, 0, &batch->getNormals()[0].x);
    glDrawElements(GL_TRIANGLES, batch->getNumIndices(), GL_UNSIGNED_INT, &batch->getIndices()[0]);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}


void LightCubeMatrix::drawEmissive(){
    for (std::vector<LightBulb>::iterator itr = mBulbsX.begin(); itr!=mBulbsX.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mBulbsY.begin(); itr!=mBulbsY.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mBulbsZ.begin(); itr!=mBulbsZ.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mBulbsXD.begin(); itr!=mBulbsXD.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mBulbsYD.begin(); itr!=mBulbsYD.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mBulbsZD.begin(); itr!=mBulbsZD.end(); ++itr) {
        itr->drawEmissive();
    }
}

/*--------------------------------------------------------------------------------*/

LightCube* LightCubeMatrix::getLightCube(int x, int y, int z){
    int numCubesX_1 = mNumCubesX - 1;
    int numCubesY_1 = mNumCubesY - 1;
    int numCubesZ_1 = mNumCubesZ - 1;
    
    if (x > numCubesX_1   || y > numCubesY_1  || z > numCubesZ_1 ) {
        return NULL;
    }
    
    return &mCubes[x * mNumCubesY * mNumCubesZ + y * mNumCubesZ + z];
}

LightCube* LightCubeMatrix::getLightCube(int x, int y, int z, int ax, int ay, int az){
    int numCubesX_1 = mNumCubesX - 1;
    int numCubesY_1 = mNumCubesY - 1;
    int numCubesZ_1 = mNumCubesZ - 1;
    
    int indexXAdded = x + ax;
    int indexYAdded = y + ay;
    int indexZAdded = z + az;
    
    if (ax > numCubesX_1   || ay > numCubesY_1   || az > numCubesZ_1 ||
        indexXAdded > numCubesX_1 || indexYAdded > numCubesY_1 || indexZAdded > numCubesZ_1) {
        return NULL;
    }
    
    return &mCubes[indexXAdded * mNumCubesY * mNumCubesZ + indexYAdded * mNumCubesZ + indexZAdded];
}

LightCube* LightCubeMatrix::getLightCube(LightCube &baseCube, int ax, int ay, int az){
    return this->getLightCube(baseCube.getIX(), baseCube.getIY(), baseCube.getIZ(),
                              ax, ay, az);
}

/*--------------------------------------------------------------------------------*/

void LightCubeMatrix::drawDebugBulb(std::vector<LightBulb> &bulbs){
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

void LightCubeMatrix::drawDebugPoints(){
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


void LightCubeMatrix::drawDebugCubes(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::Vec3f sum;
    ci::Vec3f mid;
    
    int numBulbs = 18;
    
    ci::gl::color(1, 0, 0);
    int i,j;
    i = -1;
    while (++i < mCubes.size()) {
        sum.x = 0;
        sum.y = 0;
        sum.z = 0;
        
        j = -1;
        while (++j < numBulbs) {
            mCubes[i][j]->getMid(&mid);
            sum += mid;
        }
        
        sum/=numBulbs;
        
        glPushMatrix();
        glTranslatef(sum.x,sum.y,sum.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}
