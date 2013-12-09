#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Camera.h"
#include "cinder/Utilities.h"
#include "cinder/Color.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"
#include "cinder/Rand.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/params/Params.h"

#include <vector>
#include "Utils.h"
#include "Edge.h"
#include "CubeMatrix.h"

#include "Resources.h"


using namespace ci;
using namespace ci::app;
using namespace std;

/*----------------------------------------------------------------------------------*/

static const int APP_WINDOW_WIDTH(1024),
                 APP_WINDOW_HEIGHT(1024);

static int FBO_WIDTH(APP_WINDOW_WIDTH),
           FBO_HEIGHT(APP_WINDOW_HEIGHT);

static ci::Vec2f SHADER_TEXEL_SIZE(1.0f / FBO_WIDTH, 1.0f / FBO_HEIGHT);

static float SHADER_BLUR_SCALE(0.7f),
             SHADER_BLUR_STRENGTH(0.25f);

const static int SHADER_BLUR_AMOUNT_MAX(20);
static int       SHADER_BLUR_AMOUNT(SHADER_BLUR_AMOUNT_MAX);

static bool USE_SHADER(true);

/*----------------------------------------------------------------------------------*/


static bool VIEW_AXES(false);
static bool VIEW_BG_SPHERE(true);
static bool TICK(false);
static int  TICK_FREQUENCE(20);

static bool MATRIX_DEBUG_VIEW_POINTS(false),
            MATRIX_DEBUG_VIEW_AXIS_X(false),
            MATRIX_DEBUG_VIEW_AXIS_Y(false),
            MATRIX_DEBUG_VIEW_AXIS_Z(false),
            MATRIX_DEBUG_VIEW_AXIS_XD(false),
            MATRIX_DEBUG_VIEW_AXIS_YD(false),
            MATRIX_DEBUG_VIEW_AXIS_ZD(false),
            MATRIX_DEBUG_VIEW_CUBES(false),
            MATRIX_DRAW_BULBS_OFF(false),
            MATRIX_DRAW_POINTS(false);
static int  MATRIX_SIZE_X(60),
            MATRIX_SIZE_Y(6),
            MATRIX_SIZE_Z(60);
static ci::Vec2f MATRIX_BULB_SIZE_ON(0.2f,0.05f),
                 MATRIX_BULB_SIZE_OFF(0.005f,0.005f);
static bool MATRIX_FORM_OBJECTS(false);


static int  CAMERA_MODE(1); //0 = persp, 1 = ortho
static bool CAMERA_SNAP(true);
static int  CAMERA_SNAP_AXIS_X(3),
            CAMERA_SNAP_AXIS_Y(3),
            CAMERA_SNAP_AXIS_Z(3);

static bool LIGHT_ROTATE(false);

static ci::Colorf BLACK(0,0,0),
                  BLACK_0(0.15f,0.1164f,0.135f),
                  BLACK_1(0.25f,0.25f,0.35f);
static const ci::Color WHITE(1,1,1),
                      WHITE_2(0.9,1,1);

static const ci::Vec3f ZERO(0,0,0);




/*----------------------------------------------------------------------------------*/

class IsoGridMarcherApp : public AppNative {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseWheel(MouseEvent event );
    void updateCams();
    void updateLights();
    void updateCamRot(const ci::Vec2f& pos);
    
    void resize();
    
    void drawScenePartsAll();
    void drawScenePartsOcclusive();
    void drawScenePartsEmmissive();
    void drawScenePartsDebug();
    void processFBO();
    
    void changeMatrixSize();
    
    void setupParams();
    
	void update();
	void draw();
    void drawAxes();
    void drawFBO(ci::gl::Fbo& fbo);
    
    /*----------------------------------------------------------------------------------*/
    
    ci::Timer mTimer;
    float mTimerDiff;
    
    float mTime;
    float mTimeLast;
    float mTimeDelta;
    int   mTick;
    
    
    /*----------------------------------------------------------------------------------*/
    
    //Camera
    
    ci::CameraPersp  mCameraPersp;
    ci::CameraOrtho  mCameraOrtho;
    ci::Vec3f        mCameraEye;
    ci::Vec3f        mCameraEyeTarget;
    float            mCameraZoom;
    float            mCameraRotX;
    float            mCameraRotY;
    float            mMouseWheelOffset;
    
    /*----------------------------------------------------------------------------------*/
    
    ci::gl::Light*   mLight0;
    ci::gl::Material mMaterial0; //bg
    ci::gl::Material mMaterial1; //black
    ci::gl::Material mMaterial2; //black
    
    
    CubeMatrix* mLightMatrix;
    EdgePatternSequence mLightPatternSeq;
    EdgePatternSequence mPattern0; //bubble
    EdgePatternSequence mPattern1;
    EdgePatternSequence mPattern2;
    EdgePatternSequence mPattern3;
    EdgePatternSequence mPattern4;
    EdgePatternSequence mPattern5;
    EdgePatternSequence mPattern6;
    EdgePatternSequence mPattern7;
    EdgePatternSequence mPattern8;
    
    /*----------------------------------------------------------------------------------*/
    
    ci::gl::Fbo         mFbo0;
    ci::gl::Fbo         mFbo1;
    ci::gl::Fbo         mFbo2;
    
    ci::gl::GlslProgRef mShaderBlur;
    ci::gl::GlslProgRef mShaderBlend;
    
    /*----------------------------------------------------------------------------------*/


    ci::params::InterfaceGlRef mParams;

};

void IsoGridMarcherApp::changeMatrixSize(){
    mLightMatrix->setSize(MATRIX_SIZE_X, MATRIX_SIZE_Y, MATRIX_SIZE_Z);
}

void IsoGridMarcherApp::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);
    settings->setFrameRate(30);
}



void IsoGridMarcherApp::setup(){
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mTick = 0;
    mMouseWheelOffset = 0.0f;
    
    /*----------------------------------------------------------------------------------*/
    
    ci::gl::enableDepthRead();
    ci::gl::enableDepthWrite();
    
    gl::Fbo::Format format;
    format.setSamples( 16 );
	mFbo0 = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format);
    mFbo1 = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format);
    mFbo2 = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format);
    
    try {
        mShaderBlur = gl::GlslProg::create(loadResource(POST_VERT_GLSL),
                                           loadResource(POST_BLUR_FRAG_GLSL));
    } catch (gl::GlslProgCompileExc &exc) {
    	std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch( ... ) {
		std::cout << "Unable to load shader" << std::endl;
	}
    
    try {
        mShaderBlend = gl::GlslProg::create(loadResource(POST_VERT_GLSL),
                                            loadResource(POST_BLEND_FRAG_GLSL));
    } catch (gl::GlslProgCompileExc &exc) {
    	std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch( ... ) {
		std::cout << "Unable to load shader" << std::endl;
	}
    
    /*----------------------------------------------------------------------------------*/
    
    this->updateCams();
    
    mCameraZoom = 5.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mCameraEye = mCameraEyeTarget = ci::Vec3f(6,-6,6);
    
    mCameraPersp.setEyePoint(mCameraEye);
    mCameraPersp.setCenterOfInterestPoint(ZERO);
    mCameraOrtho.setEyePoint(mCameraEye);
    mCameraOrtho.setCenterOfInterestPoint(ZERO);
    
    /*----------------------------------------------------------------------------------*/
    
    mLight0 = new gl::Light( gl::Light::DIRECTIONAL, 0 );
	mLight0->lookAt( Vec3f( 5,-5,5 ), ZERO );
	mLight0->setAmbient( Color( 0.3f, 0.3f, 0.3f ) );
	mLight0->setDiffuse( Color( 0.5f, 0.5f, 0.5f ) );
	mLight0->setSpecular( Color( 1,1,1 ) );
    this->updateLights();
	mLight0->enable();
    
    mMaterial0.setAmbient( BLACK_0 );
	mMaterial0.setDiffuse( BLACK_0 );
    mMaterial0.setSpecular( Color(1,1,1));
	mMaterial0.setShininess( 215.0f );
    
    mMaterial1.setAmbient( BLACK );
	mMaterial1.setDiffuse( BLACK );
    mMaterial1.setSpecular( BLACK_1);
	mMaterial1.setShininess( 215.0f );
    
    mMaterial2.setAmbient( WHITE_2 );
	mMaterial2.setDiffuse( WHITE_2 );
    mMaterial2.setSpecular( Color(1,1,1));
	mMaterial2.setShininess( 215.0f );

    /*----------------------------------------------------------------------------------*/

    mLightMatrix = new CubeMatrix();
    mLightPatternSeq = EdgePatternSequence(6);
    mLightPatternSeq[ 0][ 0] = 1;
    mLightPatternSeq[ 0][ 9] = 1;
    
    mLightPatternSeq[ 1].pos.y = 1;
    mLightPatternSeq[ 1][ 0] = 1;
    mLightPatternSeq[ 1][14] = 1;
    
    mLightPatternSeq[ 2].pos.y = 2;
    mLightPatternSeq[ 2][14] = 1;
    
    mLightPatternSeq[ 3].pos.x = -1;
    mLightPatternSeq[ 3][ 0] = 1;
    
    
    mLightPatternSeq[ 4].pos.set(-1,2,0);
    mLightPatternSeq[ 4][11] = 1;
    
    mLightPatternSeq[ 5].pos.set(-2,2,0);
    mLightPatternSeq[ 4][11] = 1;
    
    
    
    /*
    (*mLightPatternSeq)[1].position.y = 0;
    (*mLightPatternSeq)[1][11] = 1;
    */
    
    this->changeMatrixSize();
    this->setupParams();
    
    /*
    mPattern0 = EdgePatternSequence(7);
    mPattern0.pos.x = floor(mLightMatrix->getNumCubesX() * 0.5f);
    mPattern0.pos.y = floor(mLightMatrix->getNumCubesY() * 0.5f);
    mPattern0.pos.z = floor(mLightMatrix->getNumCubesZ() * 0.5f);
    mPattern0[0].pos.set(1, 1, 0);
    mPattern0[0][4] = 1;
    mPattern0[0][9] = 1;
    mPattern0[1].pos.set(0, 1, 0);
    mPattern0[1][4] = 1;
    mPattern0[2].pos.set(-1, 1, 0);
    mPattern0[2][4] = 1;
    mPattern0[2][8] = 1;
    mPattern0[3].pos.set(-1, 0, 0);
    mPattern0[3][0] = 1;
    mPattern0[3][8] = 1;
    mPattern0[4].pos.set(0,0, 0);
    mPattern0[4][0] = 1;
    mPattern0[5].pos.set(1,-1, 0);
    mPattern0[5][8] = 1;
     mPattern0[5][14] = 1;
    mPattern0[6].pos.set(1, 0, 0);
    mPattern0[6][9] = 1;
    
    mPattern1 = EdgePatternSequence(mPattern0);
    mPattern1.pos.x = mPattern0.pos.x + 4;
    mPattern1.pos.z = mPattern0.pos.z + 3;
    
    mPattern2 = EdgePatternSequence(mPattern0);
    mPattern2.pos.z = mPattern2.pos.z + 3;
    
    mPattern3 = EdgePatternSequence(mPattern0);
    mPattern3.pos.x = mPattern0.pos.x - 4;
    mPattern3.pos.z = mPattern0.pos.z + 3;
    
    mPattern4 = EdgePatternSequence(mPattern0);
    mPattern4.pos.x = mPattern0.pos.x - 4;
    
    mPattern5 = EdgePatternSequence(mPattern0);
    mPattern5.pos.x = mPattern0.pos.x + 4;
    
    mPattern6 = EdgePatternSequence(mPattern0);
    mPattern6.pos.x = mPattern0.pos.x + 4;
    mPattern6.pos.z = mPattern0.pos.z - 3;
    
    mPattern7 = EdgePatternSequence(mPattern0);
    mPattern7.pos.z = mPattern0.pos.z - 3;
    
    mPattern8 = EdgePatternSequence(mPattern0);
    mPattern8.pos.x = mPattern0.pos.x - 4;
    mPattern8.pos.z = mPattern0.pos.z - 3;
    */
}




void IsoGridMarcherApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
    
    /*----------------------------------------------------------------------------------*/
    
    this->updateCams();
    this->updateLights();
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    gl::clear(BLACK);
    glEnable(GL_DEPTH_TEST);
    
    /*----------------------------------------------------------------------------------*/
    
    if(LIGHT_ROTATE){
        float light0RotationXZ = mTime * 0.125f;
        mLight0->lookAt(ci::Vec3f(cosf(light0RotationXZ) * 5, 5, sinf(light0RotationXZ) * 5), ZERO );
    }
    mLightMatrix->update();
    mLightMatrix->setDrawEdgesOff(MATRIX_DRAW_BULBS_OFF);
    mLightMatrix->setDrawPoints(MATRIX_DRAW_POINTS);
    mLightMatrix->setEdgeSizeOff(MATRIX_BULB_SIZE_OFF);
    mLightMatrix->setEdgeSizeOn(MATRIX_BULB_SIZE_ON);
    
    
    if(TICK && mTick % TICK_FREQUENCE == 0){
        mLightMatrix->switchRandom(0.0125f);
    }
    
    if(MATRIX_FORM_OBJECTS){
        mLightMatrix->getEdgesX()[15].switchOn();
        mLightMatrix->getEdgesX()[31].switchOn();
        mLightMatrix->getEdgesX()[47].switchOn();
        mLightMatrix->getEdgesX()[23].switchOn();
        mLightMatrix->getEdgesX()[39].switchOn();
        
        mLightMatrix->getEdgesY()[47].switchOn();
        mLightMatrix->getEdgesY()[43].switchOn();
        mLightMatrix->getEdgesY()[11].switchOn();
        mLightMatrix->getEdgesY()[7].switchOn();
        mLightMatrix->getEdgesY()[15].switchOn();
        
        mLightMatrix->getEdgesXD()[3].switchOn();
    }
    
    mLightMatrix->switchOff();
    //mLightMatrix->getLightCube(*mLightMatrix->getLightCube(0, 0, 0),0,floor(abs(sinf(mTime)  *(mLightMatrix->getNumCubesY()))),0)->switchRandom(0.15f);
    //mLightMatrix->getLightCube(*mLightMatrix->getLightCube(0, 0, 0),1,floor(abs(sinf(mTime*2)*(mLightMatrix->getNumCubesY()))),0)->switchRandom(0.15f);
    //mLightMatrix->getLightCube(0, 0, 0)->switchOn();
    
  
    
    //mLightPatternSeq.pos.x = floor(abs(sinf(mTime*3)*(mLightMatrix->getNumCubesX())));
   // mLightPatternSeq.pos.y = floor(abs(sinf(mTime*3)*(mLightMatrix->getNumCubesY())));
    
    /*
    int centerX = floor(mLightMatrix->getNumCubesX() * 0.5f);
    int centerZ = floor(mLightMatrix->getNumCubesZ() * 0.5f);
    
    float step = float(M_PI)*0.5f/10;
    float stepY = mLightMatrix->getNumCubesY() + 1;
    
    mPattern1.pos.y = 1 + floor(sinf(step*1 + mTime*2)*stepY);
    mPattern2.pos.y = 1 + floor(sinf(step*2+ mTime*2)*stepY);
    mPattern3.pos.y = 1 + floor(sinf(step*3+ mTime*2)*stepY);
    
    mPattern4.pos.y = 1 + floor(sinf(step*4 + mTime*2)*stepY);
    mPattern0.pos.y = 1 + floor(sinf(step*5+ mTime*2)*stepY);
    mPattern5.pos.y = 1 + floor(sinf(step*6+ mTime*2)*stepY);
    
    mPattern6.pos.y = 1 + floor(sinf(step*7 + mTime*2)*stepY);
    mPattern7.pos.y = 1 + floor(sinf(step*8+ mTime*2)*stepY);
    mPattern8.pos.y = 1 + floor(sinf(step*9+ mTime*2)*stepY);
    
    //mPattern2.pos.y = 1 + floor(sinf((float)M_PI + mTime*2)*4);
    
    
   // mLightMatrix->applyPatternSeq(mLightPatternSeq);
    
    mLightMatrix->applyPatternSeq(mPattern0);
    mLightMatrix->applyPatternSeq(mPattern1);
    mLightMatrix->applyPatternSeq(mPattern2);
    mLightMatrix->applyPatternSeq(mPattern3);
    mLightMatrix->applyPatternSeq(mPattern4);
    mLightMatrix->applyPatternSeq(mPattern5);
    mLightMatrix->applyPatternSeq(mPattern6);
    mLightMatrix->applyPatternSeq(mPattern7);
    mLightMatrix->applyPatternSeq(mPattern8);
    */
    //mLightMatrix->getLightBulbsX()[1].switchOn();
    
    //mLightMatrix->switchRandom(0.0125f);
    //mLightMatrix->switchOn();
    
    this->processFBO();
}

void IsoGridMarcherApp::processFBO(){
    // Draw complete scene
    mFbo0.bindFramebuffer();
    mFbo0.getTexture().bind(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->drawScenePartsAll();
    mFbo0.unbindFramebuffer();
    
    if (!USE_SHADER)return;
    
    // Draw only emissive objects
    mFbo1.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColorMask(false, false, false, false);
    this->drawScenePartsOcclusive();
    glColorMask(true, true, true, true);
    this->drawScenePartsEmmissive();
    mFbo1.unbindFramebuffer();
    
    
    //Draw first pass blur x
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    
    mFbo2.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mShaderBlur->bind();
    mShaderBlur->uniform("uTex", 0);
    mShaderBlur->uniform("uOrientation",  0);
    mShaderBlur->uniform("uTexelSize",    SHADER_TEXEL_SIZE);
    mShaderBlur->uniform("uBlurStrength", SHADER_BLUR_STRENGTH);
    mShaderBlur->uniform("uBlurScale",    SHADER_BLUR_SCALE);
    mShaderBlur->uniform("uBlurAmount",   SHADER_BLUR_AMOUNT);
    
    drawFBO(mFbo1);
    mShaderBlur->unbind();
    mFbo2.unbindFramebuffer();
    
    
    
    //Draw second pass blur y
    mFbo1.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mShaderBlur->bind();
    mShaderBlur->uniform("uTex", 0);
    mShaderBlur->uniform("uOrientation",  1);
    mShaderBlur->uniform("uTexelSize",    SHADER_TEXEL_SIZE);
    mShaderBlur->uniform("uBlurStrength", SHADER_BLUR_STRENGTH);
    mShaderBlur->uniform("uBlurScale",    SHADER_BLUR_SCALE);
    mShaderBlur->uniform("uBlurAmount",   SHADER_BLUR_AMOUNT);
    
    drawFBO(mFbo2);
    mShaderBlur->unbind();
    mFbo1.unbindFramebuffer();
    
    
    //blend original and blurred glowmap
    mFbo2.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mShaderBlend->bind();
    mShaderBlend->uniform("uTex0", 0);
    mShaderBlend->uniform("uTex1", 1);
    drawFBO(mFbo1);
    mShaderBlend->unbind();
    
    mFbo2.unbindFramebuffer();
}

void IsoGridMarcherApp::draw(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl::clear(WHITE);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    
    //this->drawScenePartsOcclusive();
    //this->drawScenePartsAll();
    drawFBO(USE_SHADER ? mFbo2 : mFbo0);
    
    mFbo0.getTexture().unbind();
    mFbo1.getTexture().unbind();
    mFbo2.getTexture().unbind();
    
    
    mParams->draw();
    
    mTimeLast = mTime;
}

/*----------------------------------------------------------------------------------*/

void IsoGridMarcherApp::drawScenePartsOcclusive(){
    glEnable(GL_LIGHTING);
    mLight0->enable();
    
    if(VIEW_BG_SPHERE){
        mMaterial0.apply();
        ci::gl::drawSphere(ZERO, 10,100);
    } else {
        mMaterial2.apply();
        ci::gl::drawSphere(ZERO, 10,100);

    }
    
    mMaterial1.apply();
    mLightMatrix->drawOcclusive();
    mLight0->disable();
    glDisable(GL_LIGHTING);
    
    this->drawScenePartsDebug();
    //ci::gl::drawColorCube(ci::Vec3f(0,0,0), ci::Vec3f(0.25f,0.25f,0.25f));
}

void IsoGridMarcherApp::drawScenePartsEmmissive(){
    glEnable(GL_LIGHTING);
    mLight0->enable();
    mMaterial2.apply();
    mLightMatrix->drawEmissive();
    mLight0->disable();
    glDisable(GL_LIGHTING);
}

void IsoGridMarcherApp::drawScenePartsDebug(){
    if(VIEW_AXES)drawAxes();
    
    if(MATRIX_DEBUG_VIEW_POINTS ||
       MATRIX_DEBUG_VIEW_AXIS_X ||
       MATRIX_DEBUG_VIEW_AXIS_Y ||
       MATRIX_DEBUG_VIEW_AXIS_Z ||
       MATRIX_DEBUG_VIEW_AXIS_XD ||
       MATRIX_DEBUG_VIEW_AXIS_YD ||
       MATRIX_DEBUG_VIEW_AXIS_ZD){
        ci::gl::disableDepthRead();
        ci::gl::color(Color::white());
        ci::gl::enableAlphaBlending();
        if(MATRIX_DEBUG_VIEW_POINTS) mLightMatrix->drawDebugPoints();
        if(MATRIX_DEBUG_VIEW_AXIS_X) mLightMatrix->drawDebugEdgesX();
        if(MATRIX_DEBUG_VIEW_AXIS_Y) mLightMatrix->drawDebugEdgesY();
        if(MATRIX_DEBUG_VIEW_AXIS_Z) mLightMatrix->drawDebugEdgesZ();
        if(MATRIX_DEBUG_VIEW_AXIS_XD)mLightMatrix->drawDebugEdgesXD();
        if(MATRIX_DEBUG_VIEW_AXIS_YD)mLightMatrix->drawDebugEdgesYD();
        if(MATRIX_DEBUG_VIEW_AXIS_ZD)mLightMatrix->drawDebugEdgesZD();
        if(MATRIX_DEBUG_VIEW_CUBES)  mLightMatrix->drawDebugCubes();
        ci::gl::disableAlphaBlending();
        ci::gl::enableDepthRead();
    }
}

void IsoGridMarcherApp::drawScenePartsAll(){
    this->drawScenePartsOcclusive();
    this->drawScenePartsEmmissive();
}


/*----------------------------------------------------------------------------------*/

void IsoGridMarcherApp::drawFBO(ci::gl::Fbo& fbo){
    glEnable( GL_TEXTURE_2D );
	gl::setMatricesWindow( getWindowSize() );
    //flip Y, could be done in shader
	gl::translate( 0, getWindowHeight() );
    gl::scale( 1, -1 );
    gl::draw( fbo.getTexture(0), app::getWindowBounds() );
    glDisable(GL_TEXTURE_2D);
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
    this->updateCamRot(event.getPos());
}

void IsoGridMarcherApp::mouseDrag( MouseEvent event ){
    this->updateCamRot(event.getPos());
}

void IsoGridMarcherApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void IsoGridMarcherApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

/*----------------------------------------------------------------------------------*/

void IsoGridMarcherApp::resize(){
    FBO_WIDTH  = app::getWindowWidth();
    FBO_HEIGHT = app::getWindowHeight();
    
    SHADER_TEXEL_SIZE.x = 1.0f / float(FBO_WIDTH);
    SHADER_TEXEL_SIZE.y = 1.0f / float(FBO_HEIGHT);
    
    mCameraPersp = ci::CameraPersp(mFbo0.getWidth(),mFbo0.getHeight(), 60.0f);
    
    gl::Fbo::Format format;
    format.setSamples( 16 );
	mFbo0 = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format);
    mFbo1 = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format);
    mFbo2 = gl::Fbo( FBO_WIDTH, FBO_HEIGHT, format);
}

void IsoGridMarcherApp::updateCams(){
    float near = CAMERA_MODE == 0 ? 0.0001f : -3;
    float far  = 1000.0f;
    float aspectRatio = mFbo0.getAspectRatio();
    
    mCameraPersp.setPerspective(45.0f, aspectRatio, near, far);
    mCameraOrtho.setOrtho(-aspectRatio * mCameraZoom, aspectRatio * mCameraZoom , -mCameraZoom, mCameraZoom, near, far);
    
    mCameraZoom += mMouseWheelOffset;
    mCameraZoom = std::max(-5.0f,std::min(mCameraZoom, 5.0f));
    
    if(CAMERA_SNAP){
        float camRotX = (float)M_PI_4 * (float)CAMERA_SNAP_AXIS_X;
        float camRotY = (float)M_PI_4 * (float)CAMERA_SNAP_AXIS_Y;
        float camRotZ = (float)M_PI_4 * (float)CAMERA_SNAP_AXIS_Z;
        
        mCameraEyeTarget.set(sinf(camRotX),
                             sinf(camRotY),
                             cosf(camRotZ));
        
    }else {
        mCameraEyeTarget.set(cosf(mCameraRotX) * mCameraZoom,
                             sinf(mCameraRotY) * mCameraZoom,
                             sinf(mCameraRotX) * mCameraZoom);
    }
    
    mCameraEye = mCameraEye.lerp(mTimeDelta*1.5f, mCameraEyeTarget);
    
    
    if(CAMERA_MODE == 0){
        mCameraPersp.setEyePoint(mCameraEye);
        mCameraPersp.setCenterOfInterestPoint(ci::Vec3f::zero());
        gl::setMatrices(mCameraPersp);
    } else if(CAMERA_MODE == 1){
        mCameraOrtho.setEyePoint(mCameraEye);
        mCameraOrtho.setCenterOfInterestPoint(ci::Vec3f::zero());
        gl::setMatrices(mCameraOrtho);
    }
}

void IsoGridMarcherApp::updateLights(){
    if(CAMERA_MODE == 0){
        mLight0->update(mCameraPersp);
    } else if(CAMERA_MODE == 1){
        mLight0->update(mCameraOrtho);
    }
}


void IsoGridMarcherApp::setupParams(){
    mParams = ci::params::InterfaceGl::create( app::getWindow(), "CONTROL", ci::app::toPixels( ci::Vec2i( 250, 800 ) ) );
    mParams->addText("Shader");
    mParams->addParam("Use", &USE_SHADER);
    mParams->addParam("Blur Scale", &SHADER_BLUR_SCALE, "min=0 step=0.05");
    mParams->addParam("Blur Strength", &SHADER_BLUR_STRENGTH, "min=0 max=1 step=0.05");
    
    mParams->addSeparator();
    mParams->addText("Debug View");
   // mParams->addParam("Bg Sphere", &VIEW_BG_SPHERE);
    mParams->addParam("Axes", &VIEW_AXES);
    mParams->addParam("Grid Points", &MATRIX_DEBUG_VIEW_POINTS);
    mParams->addParam("Axis X", &MATRIX_DEBUG_VIEW_AXIS_X);
    mParams->addParam("Axis Y", &MATRIX_DEBUG_VIEW_AXIS_Y);
    mParams->addParam("Axis Z", &MATRIX_DEBUG_VIEW_AXIS_Z);
    mParams->addParam("Axis XD",&MATRIX_DEBUG_VIEW_AXIS_XD);
    mParams->addParam("Axis YD",&MATRIX_DEBUG_VIEW_AXIS_YD);
    mParams->addParam("Axis ZD",&MATRIX_DEBUG_VIEW_AXIS_ZD);
    
    mParams->addSeparator();
    mParams->addText("Camera");
    mParams->addParam("Mode", &CAMERA_MODE, "min=0 max=1 step=1");
    mParams->addParam("Snap", &CAMERA_SNAP);
    mParams->addParam("Snap X", &CAMERA_SNAP_AXIS_X, "min=1 max=4 step=1");
    mParams->addParam("Snap Y", &CAMERA_SNAP_AXIS_Y, "min=1 max=4 step=1");
    mParams->addParam("Snap Z", &CAMERA_SNAP_AXIS_Z, "min=1 max=4 step=1");
    
    mParams->addSeparator();
    mParams->addText("Light");
    mParams->addParam("Rotate", &LIGHT_ROTATE);
    mParams->addSeparator();
    
    mParams->addText("Time");
    mParams->addParam("Tick", &TICK);
    mParams->addParam("Tick Frequence", &TICK_FREQUENCE, "min=1 max=50 step=1");
    mParams->addSeparator();
    
    mParams->addText("Matrix");
    mParams->addParam("Size X", &MATRIX_SIZE_X, "min=2 max=25 step=1");
    mParams->addParam("Size YY", &MATRIX_SIZE_Y, "min=2 max=25 step=1");
    mParams->addParam("Size Z", &MATRIX_SIZE_Z, "min=2 max=25 step=1");
    mParams->addButton("DO IT", std::bind(&IsoGridMarcherApp::changeMatrixSize,this));
    mParams->addParam("Bg Sphere", &VIEW_BG_SPHERE);

    mParams->addParam("Draw Bulbs Off", &MATRIX_DRAW_BULBS_OFF);
    mParams->addParam("Draw Gird Points", &MATRIX_DRAW_POINTS);
    mParams->addParam("Bulb Size Off X", &MATRIX_BULB_SIZE_OFF.x, "min=0 max=1 step=0.0001");
    mParams->addParam("Bulb Size Off Y", &MATRIX_BULB_SIZE_OFF.y, "min=0 max=1 step=0.0001");
    mParams->addParam("Bulb Size On X", &MATRIX_BULB_SIZE_ON.x,  "min=0 max=1 step=0.0001");
    mParams->addParam("Bulb Size On Y", &MATRIX_BULB_SIZE_ON.y,  "min=0 max=1 step=0.0001");
    mParams->addParam("Form Objects", &MATRIX_FORM_OBJECTS);
    mParams->minimize();
    
    
    
}

CINDER_APP_NATIVE( IsoGridMarcherApp, RendererGl )
