#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Light.h"

/*
#include "PRDCB/gl/glUtil.h"
#include "PRDCB/math/PRMath.h"
*/

#include <vector>
#include "cinder/Vector.h"
#include "cinder/Rand.h"

#include "TriMeshBatch.h"
#include "SegCube.h"
#include "SegLine.h"
#include "SegTriangle.h"

#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static int   NUM_PATHS = 2000;
static bool  ANIM_Y  = false;
static float SCALE_X = 1;
static float SCALE_Z = 1;
static float OFFSET_Y = 1;


class GeomQuadWall_2ElementsOptApp : public AppNative {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseWheel(MouseEvent event );
    void resize();
    void updateCamRot(const ci::Vec2f& pos);
    void setupPaths();
    //   void setupShadowMap();
	void update();
    void drawGeometry();
	void draw();
    void drawAxes();
    
    float sawf(float n);
    
    /*----------------------------------------------------------------------------------*/
    
    //Camera
    
    ci::CameraPersp* mCamera;
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
    
    ci::gl::GlslProg mShader;
    ci::gl::Fbo      mDepthFbo;
    ci::gl::Light*   mLight0;
    ci::gl::Material mMaterial0;
    ci::gl::Material mMaterial1;
    ci::gl::Material mMaterial2;
    
    /*----------------------------------------------------------------------------------*/
    
    std::vector<std::vector<ci::Vec3f>> mPaths;
    
    TriMeshBatch*            mBatchSegLine;
    TriMeshBatch*            mBatchSegCube;
    TriMeshBatch*            mBatchSegTri;
    std::vector<SegCube>     mSegsCube;
    std::vector<SegLine>     mSegsLine;
    std::vector<SegTriangle> mSegsTri;
    
    
    ci::params::InterfaceGlRef mParams;
    
};

static const Color BLUE( 1, 0.25f, 1.0f ), RED( 1.0f, 0.55f, 0.55f ),
BLUE2(184.0f/255.0f,48.0f/255.0f,94.0f/255.0f);

void GeomQuadWall_2ElementsOptApp::prepareSettings(Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}

float GeomQuadWall_2ElementsOptApp::sawf(float n){
        return 2.0f * (n  - floorf((0.5f + n )));
}

void GeomQuadWall_2ElementsOptApp::setup(){
    mCamera = new ci::CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f );
    mCamera->setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mTick = 0;
    
    mCameraEye.set(1, 1, 1);
    
    mCamera->lookAt(mCameraEye, ci::Vec3f::zero());
    
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mMouseWheelOffset = 0.0f;
    
    mLight0 = new gl::Light( gl::Light::DIRECTIONAL, 0 );
	mLight0->lookAt( Vec3f( 20,20,20 ), Vec3f( 0, 0, 0 ) );
	mLight0->setAmbient( Color( 0.3f, 0.3f, 0.3f ) );
	mLight0->setDiffuse( Color( 1,1,1 ) );
	mLight0->setSpecular( Color( 1,1,1 ) );
	mLight0->update( *mCamera );
	mLight0->enable();
    
    mMaterial0.setAmbient( BLUE2);
	mMaterial0.setDiffuse( BLUE2 );
    mMaterial0.setSpecular( Color(1,1,1));
	mMaterial0.setShininess( 125.0f );
    
    mMaterial1.setAmbient( Color(1,1,1) );
	mMaterial1.setDiffuse( Color(1,1,1) );
    mMaterial1.setSpecular( Color(1,1,1));
	mMaterial1.setShininess( 125.0f );
    
    mMaterial2.setAmbient( Color(1,1,1) );
	mMaterial2.setDiffuse( Color(1,1,1) );
    mMaterial2.setSpecular( Color(1,1,1));
	mMaterial2.setShininess( 125.0f );
    
    mParams = ci::params::InterfaceGl::create( app::getWindow(), "NEXT Geometry Wall", ci::app::toPixels( ci::Vec2i( 200, 150 ) ) );
    mParams->addText("SEGMENTS");
    
    mParams->addParam("Num Paths", &NUM_PATHS);
    mParams->addParam("Offset Y", &OFFSET_Y);
    mParams->addButton("SETUP", std::bind(&GeomQuadWall_2ElementsOptApp::setupPaths,this));
    mParams->addSeparator();
    mParams->addParam("Scale X", &SCALE_X);
    mParams->addParam("Scale Z", &SCALE_Z);
    mParams->addParam("Anim  Y", &ANIM_Y);
    
    mBatchSegLine = new TriMeshBatch();
    mBatchSegCube = new TriMeshBatch();
    mBatchSegTri  = new TriMeshBatch();
    
    setupPaths();
}

void GeomQuadWall_2ElementsOptApp::setupPaths(){
    int numPaths = NUM_PATHS;
    
    int i,j;
    float ni;
    mPaths.resize(numPaths);
    int pathNumPoints = 30;
    
    mSegsCube.resize(0);
    mSegsLine.resize(0);
    mSegsTri.resize(0);

    
    
    i = -1;
    while (++i < numPaths) {
        j = -1;
        while (++j < pathNumPoints) {
            ni = (float)j/(float)pathNumPoints;
            mPaths[i].push_back(ci::Vec3f(0,0.0025,(-0.5+ni)));
        }
    }
    
    int numSegCubeVertices = 0;
    int numSegCubeIndices  = 0;
    
    int numSegLineVertices = 0;
    int numSegLineIndices  = 0;
    
    int numSegTriVertices = 0;
    int numSegTriIndices  = 0;
    float randX,randY,randZ;
    float rand;
    
    i = -1;
    while (++i < mPaths.size()) {
        
        rand = ci::randFloat();
        
        if (rand< 0.3f) {
            
            mSegsCube.push_back(SegCube(&mPaths[i]));
            SegCube& cube = mSegsCube.back();
            cube.setScale(ci::randFloat(),ci::randFloat( )*0.145f,ci::randFloat()*0.015f);
            cube.setPathOffset(0.0f, ci::randFloat(-1,1) * OFFSET_Y);
            
            numSegCubeVertices += cube.getNumVertices();
            numSegCubeIndices  += cube.getNumIndices();
            
            
        }
        /*
         else if (rand >= 0.1f && rand < 0.7f ) {
         
         mSegsTri.push_back(SegTriangle(&mPaths[i]));
         SegTriangle& tri = mSegsTri.back();
         // tri.setScale(ci::randFloat(),ci::randFloat( )*0.145f,ci::randFloat()*0.015f);
         randX = randY =ci::randFloat() * 0.125f;
         // randY = ci::randFloat() * 0.125f;
         randZ = ci::randFloat() * 0.125f;
         tri.setScale(randX, randY, randZ);
         tri.setPathOffset(ci::randFloat(-0.00015f,0.00015f), ci::randFloat(-1,1));
         
         numSegTriVertices += tri.getNumVertices();
         numSegTriIndices  += tri.getNumIndices();
         
         }
         */
        else{
            mSegsLine.push_back(SegLine(&mPaths[i],10,6));
            SegLine& line = mSegsLine.back();
            line.setScale(ci::randFloat() * 0.0035f);
            line.setPathOffset(ci::randFloat(-0.0015f,0.0015f), ci::randFloat(-1,1) * OFFSET_Y);
            
            numSegLineVertices += line.getNumVertices();
            numSegLineIndices  += line.getNumIndices();
            
        }
        
    }
    
    mBatchSegCube->resize(numSegCubeVertices, numSegCubeIndices);
    mBatchSegLine->resize(numSegLineVertices, numSegLineIndices);
    mBatchSegTri->resize( numSegTriVertices,  numSegTriIndices);

    
   
    
}


void GeomQuadWall_2ElementsOptApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
    
    int i,j;
    float ni,nj;
    
    std::vector<std::vector<ci::Vec3f>>::iterator itrPaths = mPaths.begin();
    std::vector<ci::Vec3f>::iterator itrPath;
    
    float pathsSize = (float)mPaths.size();
    float pathSize;
    
    i = 0;
    while (itrPaths != mPaths.end()) {
        ni       = (float)i / pathsSize;
        itrPath  = itrPaths->begin();
        pathSize = (float)itrPaths->size();
        
        j = 0;
        while (itrPath != itrPaths->end()) {
            nj = (float)j / pathSize;
            
            itrPath->y = sinf(nj * (float)M_PI * 2 + mTime * 0.5)*0.25f * (float)ANIM_Y;
            itrPath->x = sinf(nj * (float)M_PI * 2 + mTime)*0.055f+ (-0.5 + ni)*2 *(float)SCALE_X;
            itrPath->z = (-0.5+nj)*2 * SCALE_Z;
            
            ++j;
            ++itrPath;
        }
        ++i;
        ++itrPaths;
    }
    
    
    mBatchSegCube->reset();
    
    float n;
    i = 0;
    for (std::vector<SegCube>::iterator itr = mSegsCube.begin(); itr != mSegsCube.end(); itr++) {
        ni = (float)i/(float)(mSegsCube.size());
        n  = (1 +sawf(ni * (float)M_PI*16 + mTime*0.25f)) * 0.5f;
        
        itr->setPathRange(n,n+0.1f);
        itr->update();
        mBatchSegCube->put(*itr);
        ++i;
    }
    
    mBatchSegLine->reset();
    
    i = 0;
    for (std::vector<SegLine>::iterator itr = mSegsLine.begin(); itr != mSegsLine.end(); itr++) {
        ni = (float)i/(float)(mSegsLine.size());
        n  = (1 +sawf(ni * (float)M_PI*2 + mTime*0.25f)) * 0.5f;
        
        itr->setPathRange(n,n+0.15f);
        itr->update();
        mBatchSegLine->put(*itr);
        ++i;
    }
    
    mBatchSegTri->reset();
    
    i = 0;
    for (std::vector<SegTriangle>::iterator itr = mSegsTri.begin(); itr != mSegsTri.end(); itr++) {
        ni = (float)i/(float)(mSegsTri.size());
        n  = (1 +sawf(ni * (float)M_PI*2 + mTime*0.25f)) * 0.5f;
        
        itr->setPathRange(n,n+0.15f);
        itr->update();
        mBatchSegTri->put(*itr);
        ++i;
    }
    
}

void GeomQuadWall_2ElementsOptApp::draw(){
	mCameraZoom += mMouseWheelOffset;
    //mCameraZoom = std::max(-5, std::min(mCameraZoom, 5));
    
    
    
    mCameraEyeTarget.set(cosf(mCameraRotX) * mCameraZoom,
                         sinf(mCameraRotY) * mCameraZoom,
                         sinf(mCameraRotX) * mCameraZoom);
    
    
    //mCameraEyeTarget.set(mCameraZoom,mCameraZoom,mCameraZoom);
    
    mCameraEye = mCameraEye.lerp(mTimeDelta * 2.0f, mCameraEyeTarget);
    
    
    mCamera->setEyePoint(mCameraEye);
    mCamera->setCenterOfInterestPoint(ci::Vec3f::zero());
    
    gl::setMatrices(*mCamera);
    
    
    gl::enableDepthRead();
    gl::clear( Color( 180.0f/255.0f,180.0f/255.0f,180.0f/255.0f ) );
    glDisable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    this->drawGeometry();
     mParams->draw();
    
    mTimeLast = mTime;
}

void GeomQuadWall_2ElementsOptApp::drawGeometry(){
    glPushMatrix();
 //   pr::gl::color1f(0.825f);
    //  pr::gl::drawGridCube(16);
    //pr::gl::drawGrid(32,0.25);
    // pr::gl::drawAxes();
    glPopMatrix();
    
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    mLight0->update( *mCamera );
    
    //! Draw Segments Cube
    mMaterial0.apply();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mBatchSegCube->getVertices()[0].x);
    glNormalPointer(GL_FLOAT, 0, &mBatchSegCube->getNormals()[0].x);
    glDrawElements(GL_TRIANGLES, mBatchSegCube->getNumIndices(), GL_UNSIGNED_INT, &mBatchSegCube->getIndices()[0]);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    //! Draw Segments Line
    mMaterial1.apply();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mBatchSegLine->getVertices()[0].x);
    glNormalPointer(GL_FLOAT, 0, &mBatchSegLine->getNormals()[0].x);
    glDrawElements(GL_TRIANGLES, mBatchSegLine->getNumIndices(), GL_UNSIGNED_INT, &mBatchSegLine->getIndices()[0]);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
    mMaterial2.apply();
    
    //! Draw Segemts Triangle
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mBatchSegTri->getVertices()[0].x);
    glNormalPointer(GL_FLOAT, 0, &mBatchSegTri->getNormals()[0].x);
    glDrawElements(GL_TRIANGLES, mBatchSegTri->getNumIndices(), GL_UNSIGNED_INT, &mBatchSegTri->getIndices()[0]);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glDisable(GL_LIGHTING);
    
    
    return;
    
    for (std::vector<SegCube>::iterator itr = mSegsCube.begin(); itr != mSegsCube.end(); itr++) {
        itr->drawPath();
        itr->drawDebug();
    }
    
    
    for (std::vector<SegLine>::iterator itr = mSegsLine.begin(); itr != mSegsLine.end(); itr++) {
        itr->drawPath();
        itr->drawDebug();
    }
}

void GeomQuadWall_2ElementsOptApp::drawAxes(){
    glPushMatrix();
    glTranslatef(0.0f, -0.005, 0.0f);
    glColor3f(1.0f,0,0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() ,0.05f,0.025f );
    glColor3f(0, 1.0f, 0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() ,0.05f,0.025f);
    glColor3f(0, 0, 1.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() ,0.05f,0.025f);
    glPopMatrix();
}

/*----------------------------------------------------------------------------------*/

void GeomQuadWall_2ElementsOptApp::mouseDown( MouseEvent event ){
    updateCamRot(event.getPos());
}

void GeomQuadWall_2ElementsOptApp::mouseDrag( MouseEvent event ){
    updateCamRot(event.getPos());
}

void GeomQuadWall_2ElementsOptApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void GeomQuadWall_2ElementsOptApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void GeomQuadWall_2ElementsOptApp::resize(){
    mCamera->setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( GeomQuadWall_2ElementsOptApp, RendererGl )
