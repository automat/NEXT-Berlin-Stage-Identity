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
mDrawGridPoints(true),
mGridPointsSize(0.005f),
mLightBulbSizeOffLast(-1),
mLightBulbSizeOnLast(-1){
    this->initDebugFont();
}

LightCubeMatrix::~LightCubeMatrix(){
    this->clear();
}

void LightCubeMatrix::initDebugFont(){
    mDebugFontScale   = 0.0015f;
    mDebugFont        = ci::Font("Arial",12);
    mTextureDebugFont = ci::gl::TextureFont::create(mDebugFont);
}

void LightCubeMatrix::setPointsNum(int numX, int numY, int numZ){
    this->setNumPoints_Internal(numX, numY, numZ);
    mLightBulbSizeOnLast  = -1;
    mLightBulbSizeOffLast = -1;
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

void LightCubeMatrix::clear(){
    mGridPoints.clear();
    mLightBulbsX.clear();
    mLightBulbsY.clear();
    mLightBulbsZ.clear();
    mLightBulbsXD.clear();
    mLightBulbsYD.clear();
    mLightBulbsZD.clear();
}


void LightCubeMatrix::getLightBulbX(int indexBase, int indexAddX, int indexAddY, int indexAddZ, LightBulb* bulb){
   // int stepX = mNum
    std::cout << indexAddX * mNumPointsYZ_1 + indexAddY * mNumPointsZ_1 + indexAddZ << std::endl;
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
                mGridPoints.push_back(ci::Vec3f((-0.5  + nx),
                                                (-0.5f + ny),
                                                (-0.5f + nz)));
            }
        }
    }
    
    
    /*--------------------------------------------------------------------------------*/
    // Setup Edges aka LightBulbs
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
                    mLightBulbsX.push_back(LightBulb(&mGridPoints[indexCubeVertex0],
                                                     &mGridPoints[indexCubeVertex1]));
                }
                
                //bulbs Z axis
                if (iz < numEdgesZ) {
                    
                    mLightBulbsZ.push_back(LightBulb(&mGridPoints[indexCubeVertex0],
                                                     &mGridPoints[indexCubeVertex2]));
                }
                
                //bulbs Y axis
                if (iy < numEdgesY) {
                    mLightBulbsY.push_back(LightBulb(&mGridPoints[indexCubeVertex0],
                                                     &mGridPoints[indexCubeVertex4]));
                }
                
                //bulbs diagonal X Axis
                if( ix < numEdgesX && iy < numEdgesY){
                    mLightBulbsXD.push_back(LightBulb(&mGridPoints[indexCubeVertex4],
                                                      &mGridPoints[indexCubeVertex1]));
                }
                
                //bulbs diagonal TOP / BOTTOM
                if( ix < numEdgesX && iz < numEdgesZ){
                    mLightBulbsZD.push_back(LightBulb(&mGridPoints[indexCubeVertex0],
                                                      &mGridPoints[indexCubeVertex3]));
                }
                
                //bulbs diagonal Z axes
                if( iz < numEdgesZ && iy < numEdgesY){
                    mLightBulbsYD.push_back(LightBulb(&mGridPoints[indexCubeVertex0],
                                                      &mGridPoints[indexCubeVertex6]));
                }
                
                //std::cout << indexCube << " : " << indexCubeVertex0 << " " << indexCubeVertex1 << " " << indexCubeVertex2 << " " << indexCubeVertex3 <<  std::endl;
                //std::cout << indexCube << " : " << indexCubeVertex4 << " " << indexCubeVertex5 << " " << indexCubeVertex6 << " " << indexCubeVertex7 <<  std::endl;
            }
        }
    }
    
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
    
    mNumLightBulbsXX  = mNumPointsX_1;
    mNumLightBulbsXY  = mNumPointsY;
    mNumLightBulbsXZ  = mNumPointsZ;
    mNumLightBulbsXZY = mNumLightBulbsXZ * mNumLightBulbsXY;
    
    mNumLightBulbsYX  = mNumPointsX;
    mNumLightBulbsYY  = mNumPointsY_1;
    mNumLightBulbsYZ  = mNumPointsZ;
    mNumLightBulbsYZX = mNumLightBulbsYX * mNumLightBulbsYZ;
    
    mNumLightBulbsZX  = mNumPointsX_1;
    mNumLightBulbsZY  = mNumPointsY;
    mNumLightBulbsZZ  = mNumPointsZ;
    mNumLightBulbsZXY = mNumLightBulbsZX * mNumLightBulbsZY;
    
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
                
                
                
                mLightCubes.push_back(LightCube());
                mLightCubes.back().set(&( mLightBulbsX[indexBulb00]), &( mLightBulbsZ[indexBulb01]), &( mLightBulbsX[indexBulb02]), &( mLightBulbsZ[indexBulb03]),
                                       &( mLightBulbsX[indexBulb04]), &( mLightBulbsZ[indexBulb05]), &( mLightBulbsX[indexBulb06]), &( mLightBulbsZ[indexBulb07]),
                                       &( mLightBulbsY[indexBulb08]), &( mLightBulbsY[indexBulb09]), &( mLightBulbsY[indexBulb10]), &( mLightBulbsY[indexBulb11]),
                                       &(mLightBulbsZD[indexBulb12]), &(mLightBulbsZD[indexBulb13]),
                                       &(mLightBulbsXD[indexBulb14]), &(mLightBulbsYD[indexBulb15]), &(mLightBulbsXD[indexBulb16]), &(mLightBulbsYD[indexBulb17]));
            }
        }
    }
}

void LightCubeMatrix::switchOn(){
    for (std::vector<LightBulb>::iterator itr = mLightBulbsX.begin(); itr != mLightBulbsX.end(); ++itr) {
        itr->switchOn();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsY.begin(); itr != mLightBulbsY.end(); ++itr) {
        itr->switchOn();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZ.begin(); itr != mLightBulbsZ.end(); ++itr) {
        itr->switchOn();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsXD.begin(); itr != mLightBulbsXD.end(); ++itr) {
        itr->switchOn();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsYD.begin(); itr != mLightBulbsYD.end(); ++itr) {
        itr->switchOn();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZD.begin(); itr != mLightBulbsZD.end(); ++itr) {
        itr->switchOn();
    }
}

void LightCubeMatrix::switchOff(){
    for (std::vector<LightBulb>::iterator itr = mLightBulbsX.begin(); itr != mLightBulbsX.end(); ++itr) {
        itr->switchOff();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsY.begin(); itr != mLightBulbsY.end(); ++itr) {
        itr->switchOff();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZ.begin(); itr != mLightBulbsZ.end(); ++itr) {
        itr->switchOff();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsXD.begin(); itr != mLightBulbsXD.end(); ++itr) {
        itr->switchOff();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsYD.begin(); itr != mLightBulbsYD.end(); ++itr) {
        itr->switchOff();
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZD.begin(); itr != mLightBulbsZD.end(); ++itr) {
        itr->switchOff();
    }
}

void LightCubeMatrix::switchRandom(float triggerTolerance){
    for (std::vector<LightBulb>::iterator itr = mLightBulbsX.begin(); itr != mLightBulbsX.end(); ++itr) {
        itr->switchRandom(triggerTolerance);
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsY.begin(); itr != mLightBulbsY.end(); ++itr) {
        itr->switchRandom(triggerTolerance);
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZ.begin(); itr != mLightBulbsZ.end(); ++itr) {
        itr->switchRandom(triggerTolerance);
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsXD.begin(); itr != mLightBulbsXD.end(); ++itr) {
        itr->switchRandom(triggerTolerance);
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsYD.begin(); itr != mLightBulbsYD.end(); ++itr) {
        itr->switchRandom(triggerTolerance);
    }
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZD.begin(); itr != mLightBulbsZD.end(); ++itr) {
        itr->switchRandom(triggerTolerance);
    }
}

/*--------------------------------------------------------------------------------*/

void LightCubeMatrix::setLightBulbSizeOn(float size){
    if(mLightBulbSizeOnLast == size)return;
    for (std::vector<LightBulb>::iterator itr = mLightBulbsX.begin(); itr!=mLightBulbsX.end(); ++itr) {
        itr->setSizeOn(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsY.begin(); itr!=mLightBulbsY.end(); ++itr) {
        itr->setSizeOn(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZ.begin(); itr!=mLightBulbsZ.end(); ++itr) {
        itr->setSizeOn(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsXD.begin(); itr!=mLightBulbsXD.end(); ++itr) {
        itr->setSizeOn(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsYD.begin(); itr!=mLightBulbsYD.end(); ++itr) {
        itr->setSizeOn(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZD.begin(); itr!=mLightBulbsZD.end(); ++itr) {
        itr->setSizeOn(size);
    }
    
    mLightBulbSizeOnLast = size;

}

void LightCubeMatrix::setLightBulbSizeOff(float size){
    if(mLightBulbSizeOffLast == size)return;
    for (std::vector<LightBulb>::iterator itr = mLightBulbsX.begin(); itr!=mLightBulbsX.end(); ++itr) {
        itr->setSizeOff(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsY.begin(); itr!=mLightBulbsY.end(); ++itr) {
        itr->setSizeOff(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZ.begin(); itr!=mLightBulbsZ.end(); ++itr) {
        itr->setSizeOff(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsXD.begin(); itr!=mLightBulbsXD.end(); ++itr) {
        itr->setSizeOff(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsYD.begin(); itr!=mLightBulbsYD.end(); ++itr) {
        itr->setSizeOff(size);
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZD.begin(); itr!=mLightBulbsZD.end(); ++itr) {
        itr->setSizeOff(size);
    }
    
    mLightBulbSizeOffLast = size;
    
}

void LightCubeMatrix::drawOcclusive(){
    // Draw Points
    
    if(mDrawGridPoints){
        ci::Vec3f size(mGridPointsSize,mGridPointsSize,mGridPointsSize);
        for (std::vector<ci::Vec3f>::iterator itr = mGridPoints.begin(); itr != mGridPoints.end(); ++itr) {
            ci::gl::drawSphere(*itr, mGridPointsSize, 15);
            //ci::gl::drawCube(*itr, size);
            //ci::gl::drawColorCube(*itr,size);
        }
    }
    
    
    if(!mDrawBulbsOff)return;
    
    // Draw Bulbs
    for (std::vector<LightBulb>::iterator itr = mLightBulbsX.begin(); itr!=mLightBulbsX.end(); ++itr) {
        itr->drawOcclusive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsY.begin(); itr!=mLightBulbsY.end(); ++itr) {
        itr->drawOcclusive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZ.begin(); itr!=mLightBulbsZ.end(); ++itr) {
        itr->drawOcclusive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsXD.begin(); itr!=mLightBulbsXD.end(); ++itr) {
        itr->drawOcclusive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsYD.begin(); itr!=mLightBulbsYD.end(); ++itr) {
        itr->drawOcclusive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZD.begin(); itr!=mLightBulbsZD.end(); ++itr) {
        itr->drawOcclusive();
    }
}


void LightCubeMatrix::drawEmissive(){
    for (std::vector<LightBulb>::iterator itr = mLightBulbsX.begin(); itr!=mLightBulbsX.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsY.begin(); itr!=mLightBulbsY.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZ.begin(); itr!=mLightBulbsZ.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsXD.begin(); itr!=mLightBulbsXD.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsYD.begin(); itr!=mLightBulbsYD.end(); ++itr) {
        itr->drawEmissive();
    }
    
    for (std::vector<LightBulb>::iterator itr = mLightBulbsZD.begin(); itr!=mLightBulbsZD.end(); ++itr) {
        itr->drawEmissive();
    }
}

/*--------------------------------------------------------------------------------*/


void LightCubeMatrix::drawDebugPoints(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::gl::color(1, 1, 1);
    int i = -1;
    while (++i < mGridPoints.size()) {
       glPushMatrix();
        ci::Vec3f& point = mGridPoints[i];
        glTranslatef(point.x, point.y, point.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}

void LightCubeMatrix::drawDebugLightBulbsX(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::Vec3f mid;
    
    ci::gl::color(1,0,0);
    int i = -1;
    while (++i < mLightBulbsX.size()) {
        mLightBulbsX[i].getMid(&mid);
        glPushMatrix();
        glTranslatef(mid.x,mid.y,mid.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}

void LightCubeMatrix::drawDebugLightBulbsY(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::Vec3f mid;
    
    ci::gl::color(0,1,0);
    int i = -1;
    while (++i < mLightBulbsY.size()) {
        mLightBulbsY[i].getMid(&mid);
        glPushMatrix();
        glTranslatef(mid.x,mid.y,mid.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}

void LightCubeMatrix::drawDebugLightBulbsZ(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::Vec3f mid;
    
    ci::gl::color(0,0,1);
    int i = -1;
    while (++i < mLightBulbsZ.size()) {
        mLightBulbsZ[i].getMid(&mid);
        glPushMatrix();
        glTranslatef(mid.x,mid.y,mid.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}

void LightCubeMatrix::drawDebugLightBulbsXD(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::Vec3f mid;
    
    ci::gl::color(0.5f,0,0);
    int i = -1;
    while (++i < mLightBulbsXD.size()) {
        mLightBulbsXD[i].getMid(&mid);
        glPushMatrix();
        glTranslatef(mid.x,mid.y,mid.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}

void LightCubeMatrix::drawDebugLightBulbsYD(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::Vec3f mid;
    
    ci::gl::color(0,0.5f,0);
    int i = -1;
    while (++i < mLightBulbsYD.size()) {
        mLightBulbsYD[i].getMid(&mid);
        glPushMatrix();
        glTranslatef(mid.x,mid.y,mid.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}

void LightCubeMatrix::drawDebugLightBulbsZD(){
    ci::Vec2f zero = ci::Vec2f::zero();
    ci::Vec3f mid;
    
    ci::gl::color(0,0,0.5f);
    int i = -1;
    while (++i < mLightBulbsZD.size()) {
        mLightBulbsZD[i].getMid(&mid);
        glPushMatrix();
        glTranslatef(mid.x,mid.y,mid.z);
        glScalef(mDebugFontScale, -mDebugFontScale, mDebugFontScale);
        mTextureDebugFont->drawString( ci::toString(i), zero );
        glPopMatrix();
    }
}
