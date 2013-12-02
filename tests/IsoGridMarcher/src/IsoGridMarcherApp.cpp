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
    
    std::vector<ci::Vec3f> mGridPoints;
    std::vector<LightBulb>  mLightBulbs;
    std::vector<LightCube>  mLightCubes;
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
    int numPointsAxes = 3;
    
    mNumPointsX = numPointsAxes;
    mNumPointsY = numPointsAxes;
    mNumPointsZ = numPointsAxes;
  
    int numPointsX_1 = mNumPointsX - 1;
    int numPointsY_1 = mNumPointsY - 1;
    int numPointsZ_1 = mNumPointsZ - 1;
    
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
  
    int bulb0Index;
    int bulb1Index;
    int bulb2Index;
    int bulb3Index;
    int bulb4Index;
    int bulb5Index;
    
    
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
                
                bulb0Index = -1;
                bulb1Index = -1;
                bulb2Index = -1;
                bulb3Index = -1;
                bulb4Index = -1;
                bulb5Index = -1;
                
                
                //bulbs X axis
                if (ix < numEdgesX){
                    mLightBulbs.push_back(LightBulb(&mGridPoints[indexCubeVertex0],&mGridPoints[indexCubeVertex1]));
                    bulb0Index = mLightBulbs.size() - 1;
                }
                
                //bulbs Y axis
                if (iy < numEdgesY) {
                    mLightBulbs.push_back(LightBulb(&mGridPoints[indexCubeVertex0],&mGridPoints[indexCubeVertex4]));
                    bulb1Index = mLightBulbs.size() - 1;
                }
                
                //bulbs Z axis
                if (iz < numEdgesZ) {
                    mLightBulbs.push_back(LightBulb(&mGridPoints[indexCubeVertex0],&mGridPoints[indexCubeVertex2]));
                    bulb2Index = mLightBulbs.size() - 1;
                }
                
                //bulbs diagonal Z axes
                if( iz < numEdgesZ && iy < numEdgesY){
                    mLightBulbs.push_back(LightBulb(&mGridPoints[indexCubeVertex0],&mGridPoints[indexCubeVertex6]));
                    bulb3Index = mLightBulbs.size() - 1;
                }
                
                
                //bulbs diagonal TOP / BOTTOM
                if( ix < numEdgesX && iz < numEdgesZ){
                    mLightBulbs.push_back(LightBulb(&mGridPoints[indexCubeVertex0],&mGridPoints[indexCubeVertex3]));
                    bulb4Index = mLightBulbs.size() - 1;
                }
                
                //bulbs diagonal X Axis
                if( ix < numEdgesX && iy < numEdgesY){
                    mLightBulbs.push_back(LightBulb(&mGridPoints[indexCubeVertex4],&mGridPoints[indexCubeVertex1]));
                    bulb5Index = mLightBulbs.size() - 1;
                }
                
                //Light Cubes
                if (ix < numEdgesX && iy < numEdgesY && iz < numEdgesZ) {
                    
                    std::cout << "---" << std::endl;
                    std::cout << bulb0Index << std::endl;
                    std::cout << bulb1Index << std::endl;
                    std::cout << bulb2Index << std::endl;
                    std::cout << bulb3Index << std::endl;
                    std::cout << bulb4Index << std::endl;
                    std::cout << bulb5Index << std::endl;
                    std::cout << "---" << std::endl;
                    
                    
                    mLightCubes.push_back(LightCube());
                    mLightCubes.back().setBottom(&(mLightBulbs[bulb0Index]),
                                                 &(mLightBulbs[bulb1Index]),
                                                 &(mLightBulbs[bulb2Index]),
                                                 &(mLightBulbs[bulb3Index]));
                    
                }
                
                
                
                //std::cout << indexCube << " : " << indexCubeVertex0 << " " << indexCubeVertex1 << " " << indexCubeVertex2 << " " << indexCubeVertex3 <<  std::endl;
                //std::cout << indexCube << " : " << indexCubeVertex4 << " " << indexCubeVertex5 << " " << indexCubeVertex6 << " " << indexCubeVertex7 <<  std::endl;

            }
        }
    }
    
    
    /*
     indexZ1 = k1 * numPointsXY + j  * mNumPointsZ + i;
     indexY1 = k  * numPointsXY + j1 * mNumPointsZ + i;
     indexX1 = k  * numPointsXY + j  * mNumPointsZ + i1;
     */
    /*
     if(i1 < mNumPointsZ){
     mLightBulbs.push_back(LightBulb(&mGridPoints[index],&mGridPoints[indexZ1]));
     }
     */
    /*
     if(j1 < mNumPointsY){
     mLightBulbs.push_back(LightBulb(&mGridPoints[index],&mGridPoints[indexY1]));
     }
     
     if(k1 < mNumPointsX) {
     mLightBulbs.push_back(LightBulb(&mGridPoints[index],&mGridPoints[indexX1]));
     }
     */

    
    /*--------------------------------------------------------------------------------*/
    // Setup Light Cubes
    /*--------------------------------------------------------------------------------*/

    /*
    int numEdgesX = numPointsX_1;
    int numEdgesY = numPointsY_1;
    int numEdgesZ = numPointsZ_1;
   
    int indexBulbZ;
    int indexBulbX;
    int indexBulbY;
    
    mLightCubes.push_back(LightCube());
    
    int cubeIndexX = 0;
    int cubeIndexY = 0;
    int cubeIndexZ = 0;
    
    

    
    indexBulbY = cubeIndexZ + ( 3 * numEdgesZ + 2) * cubeIndexY;
    indexBulbX = indexBulbZ + 1;
    indexBulbZ = indexBulbZ + 2;
    
    mLightCubes.back().setTop(&mLightBulbs[indexBulbZ], 0, 0, 0);
    mLightCubes.back().switchOn();
    */
   // std::cout << mLightCubes.size() << std::endl;
    
    
    
}


void IsoGridMarcherApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
    
    
    /*--------------------------------------------------------------------------------*/
    // Manage Light Cubes
    /*--------------------------------------------------------------------------------*/

    
    int i = -1;
    std::cout << mLightCubes.size() << std::endl;
    while (++i < mLightCubes.size()) {
        mLightCubes[i].switchOn();
    }
     
    /*
    int bulbIndex = floorf(abs(sinf(mTime*0.025f))*mLightBulbs.size());
    
     i = -1;
    while (++i < mLightBulbs.size()) {
        mLightBulbs[i].switchOn();
    }
     */
    /*
    mLightBulbs[bulbIndex].switchOn();
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
    
    /*--------------------------------------------------------------------------------*/
    // Draw Light Bulbs
    /*--------------------------------------------------------------------------------*/
    
    
    
    if (DRAW_LIGHT_BULBS) {
        i = -1;
        ci::Vec3f mid;
        while (++i < mLightBulbs.size()) {
            mLightBulbs[i].draw();
        }
        
        ci::gl::disableDepthRead();
        ci::gl::color(0,0,1);
        ci::gl::enableAlphaBlending();

        i = -1;
        while (++i < mLightBulbs.size()) {
            mLightBulbs[i].getMid(&mid);
            glPushMatrix();
            glTranslatef(mid.x,mid.y,mid.z);
            glScalef(fontScale, -fontScale, fontScale);
            mTextureFont->drawString( ci::toString(i), zero2 );
            glPopMatrix();
        }
        
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
