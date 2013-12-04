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
#include "LightBulb.h"
#include "LightCube.h"
#include "LightCubeMatrix.h"

#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

/*----------------------------------------------------------------------------------*/

static const int APP_WINDOW_WIDTH  = 1024,
                 APP_WINDOW_HEIGHT = 1024;

static const int FBO_WIDTH  = APP_WINDOW_WIDTH,
                 FBO_HEIGHT = APP_WINDOW_HEIGHT;

static float SHADER_TEXEL_SIZE_X = 1.0f / FBO_WIDTH,
             SHADER_TEXEL_SIZE_Y = 1.0f / FBO_HEIGHT;

static float SHADER_BLUR_SCALE    = 1.0f,
             SHADER_BLUR_STRENGTH = 0.75f;

const static int SHADER_BLUR_AMOUNT_MAX = 20;
static int   SHADER_BLUR_AMOUNT = SHADER_BLUR_AMOUNT_MAX;


/*----------------------------------------------------------------------------------*/
static bool VIEW_AXES(false);
static bool VIEW_BG_SPHERE(true);
static bool TICK(false);

static bool MATRIX_DEBUG_VIEW_POINTS(false),
            MATRIX_DEBUG_VIEW_AXIS_X(false),
            MATRIX_DEBUG_VIEW_AXIS_Y(false),
            MATRIX_DEBUG_VIEW_AXIS_Z(false),
            MATRIX_DEBUG_VIEW_AXIS_XD(false),
            MATRIX_DEBUG_VIEW_AXIS_YD(false),
            MATRIX_DEBUG_VIEW_AXIS_ZD(false),
            MATRIX_DRAW_BULBS_OFF(true),
            MATRIX_DRAW_GRID_POINTS(true);

static int  CAMERA_MODE(0); //0 = persp, 1 = ortho
static bool CAMERA_SNAP(true);
static int  CAMERA_SNAP_AXIS_X(3),
            CAMERA_SNAP_AXIS_Y(3),
            CAMERA_SNAP_AXIS_Z(3);
static bool LIGHT_ROTATE(false);

static ci::Colorf BLACK(0,0,0),
                  BLACK_0(0.06f,0.06f,0.225f),
                  BLACK_1(0.25f,0.25f,0.35f);
static const ci::Color WHITE(1,1,1);

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
    
    void drawScenePartsAll();
    void drawScenePartsOcclusive();
    void drawScenePartsEmmissive();
    void drawScenePartsDebug();
    
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
    
    
    LightCubeMatrix* mLightMatrix;
    
    /*----------------------------------------------------------------------------------*/
    
    ci::gl::Fbo         mFbo0;
    ci::gl::Fbo         mFbo1;
    ci::gl::Fbo         mFbo2;
    
    ci::gl::GlslProgRef mShaderBlur;
    ci::gl::GlslProgRef mShaderBlend;
    
    /*----------------------------------------------------------------------------------*/


    ci::params::InterfaceGlRef mParams;

};

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
    format.setSamples( 4 );
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
    
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mCameraEye.set(3,3,3);
    
    mCameraPersp.setEyePoint(mCameraEye);
    mCameraPersp.setCenterOfInterestPoint(ZERO);
    mCameraOrtho.setEyePoint(mCameraEye);
    mCameraOrtho.setCenterOfInterestPoint(ZERO);
    
    /*----------------------------------------------------------------------------------*/
    
    mLight0 = new gl::Light( gl::Light::DIRECTIONAL, 0 );
	mLight0->lookAt( Vec3f( 5,5,5 ), ZERO );
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
    
    mMaterial2.setAmbient( WHITE );
	mMaterial2.setDiffuse( WHITE );
    mMaterial2.setSpecular( Color(1,1,1));
	mMaterial2.setShininess( 215.0f );

    /*----------------------------------------------------------------------------------*/

    mLightMatrix = new LightCubeMatrix();
    mLightMatrix->setPointsNum(10);
    
    this->setupParams();
}




void IsoGridMarcherApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
    
    /*----------------------------------------------------------------------------------*/
    
    SHADER_TEXEL_SIZE_X   = std::max(0.0f,std::min(SHADER_TEXEL_SIZE_X, 1.0f));
    SHADER_TEXEL_SIZE_Y   = std::max(0.0f,std::min(SHADER_TEXEL_SIZE_Y, 1.0f));
    SHADER_BLUR_STRENGTH  = std::max(0.0f,std::min(SHADER_BLUR_STRENGTH,1.0f));
    SHADER_BLUR_SCALE     = std::max(0.0f,SHADER_BLUR_SCALE);
    SHADER_BLUR_AMOUNT    = std::max(1,std::min(SHADER_BLUR_AMOUNT,SHADER_BLUR_AMOUNT_MAX));
    
    ci::Vec2f TEXEL_SIZE  = ci::Vec2f(SHADER_TEXEL_SIZE_X, SHADER_TEXEL_SIZE_Y);
    
    /*----------------------------------------------------------------------------------*/
    
    this->updateCams();
    this->updateLights();
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    gl::clear(WHITE);
    glEnable(GL_DEPTH_TEST);
    
    /*----------------------------------------------------------------------------------*/
    
    if(LIGHT_ROTATE){
        float light0RotationXZ = mTime * 0.125f;
        mLight0->lookAt(ci::Vec3f(cosf(light0RotationXZ) * 5, 5, sinf(light0RotationXZ) * 5), ZERO );
    }
    
    mLightMatrix->setDrawBulbsOff(MATRIX_DRAW_BULBS_OFF);
    mLightMatrix->setDrawGridPoints(MATRIX_DRAW_GRID_POINTS);
    mLightMatrix->switchOn();
    
    
    // Draw complete scene
    mFbo0.bindFramebuffer();
    mFbo0.getTexture().bind(1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->drawScenePartsAll();
    mFbo0.unbindFramebuffer();

    
    // Draw only emissive objects
    mFbo1.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColorMask(false, false, false, false);
    this->drawScenePartsOcclusive();
    glColorMask(true, true, true, true);
    this->drawScenePartsEmmissive();
    mFbo1.unbindFramebuffer();
    
    /*
    //Draw first pass blur x
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    
    mFbo2.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mShaderBlur->bind();
    mShaderBlur->uniform("uTex", 0);
    mShaderBlur->uniform("uOrientation",  0);
    mShaderBlur->uniform("uTexelSize",    TEXEL_SIZE);
    mShaderBlur->uniform("uBlurStrength", SHADER_BLUR_STRENGTH);
    mShaderBlur->uniform("uBlurScale",    SHADER_BLUR_SCALE);
    mShaderBlur->uniform("uBlurAmount",   SHADER_BLUR_AMOUNT);
    
    drawFBO(mFbo1);
    mShaderBlur->unbind();
    mFbo2.unbindFramebuffer();
    
    
    //Draw first pass blur y
    mFbo1.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mShaderBlur->bind();
    mShaderBlur->uniform("uTex", 0);
    mShaderBlur->uniform("uOrientation",  1);
    mShaderBlur->uniform("uTexelSize",    TEXEL_SIZE);
    mShaderBlur->uniform("uBlurStrength", SHADER_BLUR_STRENGTH);
    mShaderBlur->uniform("uBlurScale",    SHADER_BLUR_SCALE);
    mShaderBlur->uniform("uBlurAmount",   SHADER_BLUR_AMOUNT);
    
    drawFBO(mFbo2);
    mShaderBlur->unbind();
    mFbo1.unbindFramebuffer();
    
    
    mFbo2.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mShaderBlend->bind();
    mShaderBlend->uniform("uTex0", 0);
    mShaderBlend->uniform("uTex1", 1);
    drawFBO(mFbo1);
    mShaderBlend->unbind();
    
    mFbo2.unbindFramebuffer();
     */

}

void IsoGridMarcherApp::draw(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl::clear(WHITE);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    
    //this->drawScenePartsOcclusive();
    //this->drawScenePartsAll();
    drawFBO(mFbo1);
    
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
        ci::gl::drawSphere(ZERO, 2.5f,100);
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
        if(MATRIX_DEBUG_VIEW_AXIS_X) mLightMatrix->drawDebugLightBulbsX();
        if(MATRIX_DEBUG_VIEW_AXIS_Y) mLightMatrix->drawDebugLightBulbsY();
        if(MATRIX_DEBUG_VIEW_AXIS_Z) mLightMatrix->drawDebugLightBulbsZ();
        if(MATRIX_DEBUG_VIEW_AXIS_XD)mLightMatrix->drawDebugLightBulbsXD();
        if(MATRIX_DEBUG_VIEW_AXIS_YD)mLightMatrix->drawDebugLightBulbsYD();
        if(MATRIX_DEBUG_VIEW_AXIS_ZD)mLightMatrix->drawDebugLightBulbsZD();
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

void IsoGridMarcherApp::updateCams(){
    float near = 0.001f;
    float far  = 1000.0f;
    //Urgh

    /*
    mCameraPersp.setPerspective(45.0f,  app::getWindowAspectRatio(), near, far);
    mCameraOrtho.setOrtho(-app::getWindowAspectRatio() * mCameraZoom, app::getWindowAspectRatio() * mCameraZoom,-mCameraZoom,mCameraZoom, near, far);
     */
    
    mCameraPersp = ci::CameraPersp(mFbo0.getWidth(),mFbo0.getHeight(), 60.0f);
    mCameraPersp.setPerspective(60, mFbo0.getAspectRatio(), near, far);
    
    mCameraOrtho = ci::CameraOrtho();
    mCameraOrtho.setOrtho(-mCameraZoom,mCameraZoom , -mCameraZoom, mCameraZoom, near, far);
    
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
    mParams = ci::params::InterfaceGl::create( app::getWindow(), "CONTROL", ci::app::toPixels( ci::Vec2i( 200, 450 ) ) );
    mParams->addText("Debug View");
    mParams->addParam("Bg Sphere", &VIEW_BG_SPHERE);
    mParams->addParam("Axes", &VIEW_AXES);
    mParams->addParam("Grid Points", &MATRIX_DEBUG_VIEW_POINTS);
    mParams->addParam("Axis X", &MATRIX_DEBUG_VIEW_AXIS_X);
    mParams->addParam("Axis Y", &MATRIX_DEBUG_VIEW_AXIS_Y);
    mParams->addParam("Axis Z", &MATRIX_DEBUG_VIEW_AXIS_Z);
    mParams->addParam("Axis XD",&MATRIX_DEBUG_VIEW_AXIS_XD);
    mParams->addParam("Axis YD",&MATRIX_DEBUG_VIEW_AXIS_YD);
    mParams->addParam("Axis ZD",&MATRIX_DEBUG_VIEW_AXIS_ZD);
    mParams->addText("Camera");
    mParams->addParam("Mode", &CAMERA_MODE, "min=0 max=1 step=1");
    mParams->addParam("Snap", &CAMERA_SNAP);
    mParams->addParam("Snap X", &CAMERA_SNAP_AXIS_X, "min=1 max=4 step=1");
    mParams->addParam("Snap Y", &CAMERA_SNAP_AXIS_Y, "min=1 max=4 step=1");
    mParams->addParam("Snap Z", &CAMERA_SNAP_AXIS_Z, "min=1 max=4 step=1");
    mParams->addText("Light");
    mParams->addParam("Rotate", &LIGHT_ROTATE);
    mParams->addText("Matrix");
    mParams->addParam("TICK", &TICK);
    mParams->addParam("Draw Bulbs Off", &MATRIX_DRAW_BULBS_OFF);
    mParams->addParam("Draw Gird Points", &MATRIX_DRAW_GRID_POINTS);
    
    mParams->minimize();
    
    
    
}

CINDER_APP_NATIVE( IsoGridMarcherApp, RendererGl )
