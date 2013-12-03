#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Camera.h"
#include "cinder/Utilities.h"
#include <vector>
#include "LightCube.h"
#include "LightBulb.h"
#include "cinder/Rand.h"
#include "Utils.h"

using namespace ci;
using namespace ci::app;
using namespace std;

/*----------------------------------------------------------------------------------*/

static const bool DRAW_POINT_INDICES(true),
                  DRAW_GRID_POINTS(true),
                  DRAW_LIGHT_BULBS(true);

/*----------------------------------------------------------------------------------*/

class IsoGridMarcherApp : public AppNative {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseWheel(MouseEvent event );
    void resize();
    void updateCamRot(const ci::Vec2f& pos);
    void setupGridCube();
    
	void update();
	void draw();
    void drawAxes();
    
    /*----------------------------------------------------------------------------------*/
    
    //Camera
    
    ci::CameraPersp  mCamera;
    ci::Vec3f        mCameraEye;
    ci::Vec3f        mCameraEyeTarget;
    float            mCameraZoom;
    float            mCameraRotX;
    float            mCameraRotY;
    float            mMouseWheelOffset;
    
    /*----------------------------------------------------------------------------------*/
    
    ci::Timer mTimer;
    float mTimerDiff;
    
    float mTime;
    float mTimeLast;
    float mTimeDelta;
    int   mTick;
    
    /*----------------------------------------------------------------------------------*/
    
    ci::Font               mFont;
    ci::gl::TextureFontRef mTextureFont;
    
    /*----------------------------------------------------------------------------------*/
    
    
    int mNumPointsX;
    int mNumPointsY;
    int mNumPointsZ;
    int mNumPoints;
    
    int mNumCubesX;
    int mNumCubesY;
    int mNumCubesZ;
    
    std::vector<ci::Vec3f> mGridPoints;
    std::vector<LightBulb> mLightBulbsX; // X axis
    std::vector<LightBulb> mLightBulbsY; // Y axis
    std::vector<LightBulb> mLightBulbsZ; // Z axis
    std::vector<LightBulb> mLightBulbsXD; // diagonal
    std::vector<LightBulb> mLightBulbsYD; // diagonal
    std::vector<LightBulb> mLightBulbsZD; // diagonal
    std::vector<LightCube> mLightCubes;
};

void IsoGridMarcherApp::prepareSettings(Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}

void IsoGridMarcherApp::setup(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mTick = 0;
    
    mCameraEye.set(1, 1, 1);
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mMouseWheelOffset = 0.0f;
    
    mFont = Font("Arial",12);
    mTextureFont = gl::TextureFont::create(mFont);
    
    setupGridCube();
}

void IsoGridMarcherApp::setupGridCube(){
    
    /*--------------------------------------------------------------------------------*/
    // Setup Points
    /*--------------------------------------------------------------------------------*/
    int numPointsAxes = 8;
    
    mNumPointsX = numPointsAxes;
    mNumPointsY = numPointsAxes;
    mNumPointsZ = numPointsAxes;
  
    int numPointsX_1 = mNumPointsX - 1;
    int numPointsY_1 = mNumPointsY - 1;
    int numPointsZ_1 = mNumPointsZ - 1;
    
    mNumCubesX = numPointsX_1;
    mNumCubesY = numPointsY_1;
    mNumCubesZ = numPointsZ_1;
    
    float scale = 0.5;
    
    
    int ix,iy,iz;
    float nx,ny,nz;
    
    ix = -1;
    while (++ix < mNumPointsX) {
        nx = (float)ix / (float)(numPointsX_1);
        iy = -1;
        while (++iy < mNumPointsY) {
            ny = (float)iy / (float)(numPointsY_1);
            iz = -1;
            while (++iz < mNumPointsZ) {
                nz = (float)iz / (float)(numPointsZ_1);
                mGridPoints.push_back(ci::Vec3f((-0.5  + nx) * scale,
                                                (-0.5f + ny) * scale,
                                                (-0.5f + nz) * scale));
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
    
    int numEdgesZ  = mNumPointsZ - 1;
    int numEdgesY  = mNumPointsY - 1;
    int numEdgesX  = mNumPointsX - 1;
    
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
    
    
    int numPointsY_1Z_1 = numPointsY_1 * numPointsZ_1;
    int numPointsYZ     = mNumPointsY  * mNumPointsZ;
    int numPointsYZ_1   = mNumPointsY  * numPointsZ_1;
    int numPointsY_1Z   = numPointsY_1 * mNumPointsZ;
    
    
    
    ix = -1;
    while (++ix < numEdgesX) {
        ix1 = ix + 1;
        iy  = -1;
        while (++iy < numEdgesY) {
            iy1 = iy + 1;
            iz  = -1;
            while (++iz < numEdgesZ) {
                //bottom trbl
                indexBulb00 = ix * numPointsYZ   + iy * mNumPointsZ  + iz;
                indexBulb03 = ix * numPointsYZ_1 + iy * numPointsZ_1 + iz;
                indexBulb02 = indexBulb00 + 1;
                indexBulb01 = indexBulb03 + numPointsYZ_1;
                
                //top trbl
                indexBulb04 = ix * numPointsYZ   + iy1 * mNumPointsZ  + iz;
                indexBulb07 = ix * numPointsYZ_1 + iy1 * numPointsZ_1 + iz;
                indexBulb06 = indexBulb04 + 1;
                indexBulb05 = indexBulb07 + numPointsYZ_1;
                
                //sides cw
                indexBulb08 = ix  * numPointsY_1Z + iy * mNumPointsZ + iz;
                indexBulb09 = ix1 * numPointsY_1Z + iy * mNumPointsZ + iz;
                indexBulb10 = indexBulb09 + 1;
                indexBulb11 = indexBulb08 + 1;
                
                //bottom/zop diagonal
                indexBulb12 = ix * numPointsYZ_1 + iy  * numPointsZ_1 + iz;
                indexBulb13 = ix * numPointsYZ_1 + iy1 * numPointsZ_1 + iz;
                
                //sides diagonal;
                indexBulb14 = indexBulb08;
                indexBulb17 = ix * numPointsY_1Z_1 + iy * numPointsZ_1 + iz;
                indexBulb16 = indexBulb14 + 1;
                indexBulb15 = indexBulb17 + numPointsY_1Z_1;
                
   
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


void IsoGridMarcherApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
    
    
    /*--------------------------------------------------------------------------------*/
    // Manage Light Cubes
    /*--------------------------------------------------------------------------------*/

    int cubeIndex = floorf(abs(sinf(mTime*0.5f))*mLightCubes.size());
    
    
    int i = -1;
    while (++i < mLightCubes.size()) {
        mLightCubes[i].switchOff();
    }
   
    /*
    i = -1;
    while (++i < mLightBulbsX.size()) {
        if(ci::randFloat() < 00.0125f)
            mLightBulbsX[i].switchOn();
    }
    
    i = -1;
    while (++i < mLightBulbsY.size()) {
        if(ci::randFloat() < 0.00125f)
            mLightBulbsY[i].switchOn();
    }
    
    i = -1;
    while (++i < mLightBulbsZ.size()) {
        if(ci::randFloat() < 0.00125f)
            mLightBulbsZ[i].switchOn();
    }
    */
    
    
    /*
    i = -1;
    while (++i < mLightBulbsXD.size()) {
        if(ci::randFloat() < 0.00125f)
            mLightBulbsXD[i].switchOn();
    }
    
    i = -1;
    while (++i < mLightBulbsYD.size()) {
        if(ci::randFloat() < 0.00125f)
            mLightBulbsYD[i].switchOn();
    }
    
    i = -1;
    while (++i < mLightBulbsZD.size()) {
        if(ci::randFloat() < 0.00125f)
            mLightBulbsZD[i].switchOn();
    }
    */
    int ix,iy,iz;
    int index;
    ix = floor(abs(sinf(mTime*0.85f))*mNumCubesX);

    
    iy = -1;
    while (++iy < mNumCubesY) {
        iz = -1;
        while (++iz < mNumCubesZ) {
            index = ix * mNumCubesX * mNumCubesZ + iy * mNumCubesZ + iz;
            mLightCubes[index].switchRandom();
            
        }
    }
    
    /*
    mLightCubes[cubeIndex].switchOn();
    
    cubeIndex = floorf(abs(sinf(mTime*0.1f))*mLightCubes.size());
    mLightCubes[cubeIndex].switchOn();
    
    cubeIndex = floorf(abs(sinf(mTime*0.35f))*mLightCubes.size());
    mLightCubes[cubeIndex].switchOn();
     */

    
}

void IsoGridMarcherApp::draw(){
	mCameraZoom += mMouseWheelOffset;
    mCameraZoom = std::max(-5.0f,std::min(mCameraZoom, 5.0f));
    
    mCameraEyeTarget.set(cosf(mCameraRotX) * mCameraZoom,
                         sinf(mCameraRotY) * mCameraZoom,
                         sinf(mCameraRotX) * mCameraZoom);
    
    mCameraEye = mCameraEye.lerp(mTimeDelta*1.5f, mCameraEyeTarget);
    
    
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    gl::setMatrices(mCamera);
    gl::enableDepthRead();
    gl::clear( Color( 0.085f, 0, 0.085f ) );
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glPushMatrix();
    drawAxes();
    
    
    glColor3f(1,1,1);
    int i = -1;
    
    /*
    while (++i < mCubes.size()) {
        ci::gl::drawStrokedCube(mCubes[i].getPosition(), mCubes[i].getSize());
    }
     */
    
    /*--------------------------------------------------------------------------------*/
    // Draw Grid Points
    /*--------------------------------------------------------------------------------*/

    if(DRAW_GRID_POINTS){
        i = -1;
        ci::gl::color(0.01, 0.01, 0.01);
        while (++i < mGridPoints.size()) {
            ci::gl::drawSphere(mGridPoints[i],0.005f);
        }
    }
    
    /*--------------------------------------------------------------------------------*/
    // Draw Debug Point Indices
    /*--------------------------------------------------------------------------------*/

    float fontScale = 0.0015;
     ci::Vec2f zero2(0,0);
    
    /*
    if(DRAW_POINT_INDICES){
        ci::gl::disableDepthRead();
                ci::gl::color(Color::white());
        ci::gl::enableAlphaBlending();
       
        i = -1;
        while (++i < mGridPoints.size()) {
            glPushMatrix();
            glTranslatef(mGridPoints[i].x,mGridPoints[i].y,mGridPoints[i].z);
            glScalef(fontScale, -fontScale, fontScale);
            mTextureFont->drawString( ci::toString(i), zero2 );
            glPopMatrix();
        }
        ci::gl::disableAlphaBlending();
        ci::gl::enableDepthRead();
    }
     */
    
    /*--------------------------------------------------------------------------------*/
    // Draw Light Bulbs
    /*--------------------------------------------------------------------------------*/
    
    
    
    if (DRAW_LIGHT_BULBS) {
        i = -1;
        ci::Vec3f mid;
        std::vector<LightBulb>::iterator itrBulbs;
        
        
        itrBulbs = mLightBulbsX.begin();
        while (itrBulbs != mLightBulbsX.end()) {
            itrBulbs->draw();
            itrBulbs++;
        }
        
        
        itrBulbs = mLightBulbsY.begin();
        while (itrBulbs != mLightBulbsY.end()) {
            itrBulbs->draw();
            itrBulbs++;
        }
        
        itrBulbs = mLightBulbsZ.begin();
        while (itrBulbs != mLightBulbsZ.end()) {
            itrBulbs->draw();
            itrBulbs++;
        }
        
        itrBulbs = mLightBulbsXD.begin();
        while (itrBulbs != mLightBulbsXD.end()) {
            itrBulbs->draw();
            itrBulbs++;
        }
        
        itrBulbs = mLightBulbsYD.begin();
        while (itrBulbs != mLightBulbsYD.end()) {
            itrBulbs->draw();
            itrBulbs++;
        }
        
        itrBulbs = mLightBulbsZD.begin();
        while (itrBulbs != mLightBulbsZD.end()) {
            itrBulbs->draw();
            itrBulbs++;
        }
        
     
        
        
        ci::gl::disableDepthRead();
        ci::gl::enableAlphaBlending();

        /*
        ci::gl::color(1,0,0);
        i = -1;
        while (++i < mLightBulbsX.size()) {
            mLightBulbsX[i].getMid(&mid);
            glPushMatrix();
            glTranslatef(mid.x,mid.y,mid.z);
            glScalef(fontScale, -fontScale, fontScale);
            mTextureFont->drawString( ci::toString(i), zero2 );
            glPopMatrix();
        }
        */
        /*
        ci::gl::color(0,1,0);
        i = -1;
        while (++i < mLightBulbsY.size()) {
            mLightBulbsY[i].getMid(&mid);
            glPushMatrix();
            glTranslatef(mid.x,mid.y,mid.z);
            glScalef(fontScale, -fontScale, fontScale);
            mTextureFont->drawString( ci::toString(i), zero2 );
            glPopMatrix();
        }
         */
        /*
        ci::gl::color(0,0,1);
        i = -1;
        while (++i < mLightBulbsZ.size()) {
            mLightBulbsZ[i].getMid(&mid);
            glPushMatrix();
            glTranslatef(mid.x,mid.y,mid.z);
            glScalef(fontScale, -fontScale, fontScale);
            mTextureFont->drawString( ci::toString(i), zero2 );
            glPopMatrix();
        }
         */
        /*
        ci::gl::color(0.5,0.5,0.5);
        
        i = -1;
        while (++i < mLightBulbsYD.size()) {
            mLightBulbsYD[i].getMid(&mid);
            glPushMatrix();
            glTranslatef(mid.x,mid.y,mid.z);
            glScalef(fontScale, -fontScale, fontScale);
            mTextureFont->drawString( ci::toString(i), zero2 );
            glPopMatrix();
        }
        
      
        i = -1;
        while (++i < mLightBulbsXD.size()) {
            mLightBulbsXD[i].getMid(&mid);
            glPushMatrix();
            glTranslatef(mid.x,mid.y,mid.z);
            glScalef(fontScale, -fontScale, fontScale);
            mTextureFont->drawString( ci::toString(i), zero2 );
            glPopMatrix();
        }
         */
        /*
        i = -1;
        while (++i < mLightBulbsZD.size()) {
            mLightBulbsZD[i].getMid(&mid);
            glPushMatrix();
            glTranslatef(mid.x,mid.y,mid.z);
            glScalef(fontScale, -fontScale, fontScale);
            mTextureFont->drawString( ci::toString(i), zero2 );
            glPopMatrix();
        }
         */

        
        ci::gl::disableAlphaBlending();
        ci::gl::enableDepthRead();


    }
     
    
    
    
    
     
    
    
    glPopMatrix();
    
    mTimeLast = mTime;
}

void IsoGridMarcherApp::drawAxes(){
    glPushMatrix();
    glTranslatef(0.0f, -0.00005f, 0.0f);
    glColor3f(1.0f,0,0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() ,0.05f,0.025f );
    glColor3f(0, 1.0f, 0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() ,0.05f,0.025f);
    glColor3f(0, 0, 1.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() ,0.05f,0.025f);
    glPopMatrix();
}

/*----------------------------------------------------------------------------------*/

void IsoGridMarcherApp::mouseDown( MouseEvent event ){
    updateCamRot(event.getPos());
}

void IsoGridMarcherApp::mouseDrag( MouseEvent event ){
    updateCamRot(event.getPos());
}

void IsoGridMarcherApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void IsoGridMarcherApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void IsoGridMarcherApp::resize(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( IsoGridMarcherApp, RendererGl )
