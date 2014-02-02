#include "cinder/app/AppNative.h"
#include "Resources.h"

#include <OpenGL/OpenGL.h>
#include <vector>
#include <cstdio>

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Surface.h"

#include "cinder/Camera.h"

#include "cinder/params/Params.h"

#include "NLogo.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Light.h"

#include "cinder/gl/Texture.h"



using namespace ci;
using namespace ci::app;
using namespace std;


static const int   APP_WIDTH(800), APP_HEIGHT(600);
static const float APP_FPS(30.0);


static float CAMERA_FOV(45.0f);

class Logo_00App : public AppNative {
public:
    void prepareSettings(Settings* settings);
    void resize();
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
    
    /*----------------------------------------------------------------------------------------------------*/
    
    CameraPersp mCamera;
    Vec3f       mCameraEye;
    Vec3f       mCameraTarget;
    
    float mTime;
    float mFrame;
    
    /*----------------------------------------------------------------------------------------------------*/
    
    NLogo mLogo;
    gl::Light*    mLightLogo;
    gl::Material* mMaterialLogo;
    
    gl::Texture mFontTexture;
    
  


    
    
    params::InterfaceGlRef mParams;
    
};

void Logo_00App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
    //settings->setResizable(false);
}

void Logo_00App::setup(){
    mCamera.setPerspective(CAMERA_FOV, app::getWindowAspectRatio(), 0.00001f, 5.0);
    mCameraEye.set(0,2,0);
    mCameraTarget.set(0, 0, 0);
    mCamera.lookAt(mCameraEye, mCameraTarget);
    
    /*----------------------------------------------------------------------------------------------------*/
    
    mLightLogo = new gl::Light(gl::Light::DIRECTIONAL,0);
    mLightLogo->lookAt(Vec3f(3,3,3), Vec3f(0,0,0));
    mLightLogo->setAmbient(Color(0.1,0.1,0.1));
    mLightLogo->setDiffuse(Color(1,1,1));
    mLightLogo->setSpecular(Color(1,1,1));
    //mLightDebug->update(*mCameraRef);
    mLightLogo->enable();
    
    mMaterialLogo = new gl::Material();
    mMaterialLogo->setAmbient(Color( 1, 0.25f, 1.0f ));
    mMaterialLogo->setDiffuse(Color( 1, 0.25f, 1.0f ));
    mMaterialLogo->setSpecular(Color(1,1,1));
    mMaterialLogo->setShininess(10.0f);
    
    mFontTexture = gl::Texture(loadImage(app::loadResource(NEXT_FONT_512x512_PNG)));
    mFontTexture.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    mFontTexture.setMagFilter(GL_LINEAR);
    mFontTexture.setMinFilter(GL_LINEAR);
    
    /*----------------------------------------------------------------------------------------------------*/
    
    mParams = params::InterfaceGl::create(app::getWindow(), "CONTROL", ci::Vec2f(200,100));
    mParams->addText("CAMERA");
    mParams->addParam("fov", &CAMERA_FOV,"min=0.1 max=120.0 step=0.5");
    mParams->minimize();
    
}

/*----------------------------------------------------------------------------------------------------*/

void Logo_00App::update(){
    mTime  = app::getElapsedSeconds();
    mFrame = app::getElapsedFrames();
    
    mLogo.update(mTime);
}

void Logo_00App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    // Set camera view
    gl::setViewport(app::getWindowBounds());
    gl::setMatrices(mCamera);
    gl::enableDepthRead();
    gl::drawCoordinateFrame(2.0f);
    gl::enableAlphaBlending();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE);
    mFontTexture.bind();
    mMaterialLogo->apply();
    mLogo.draw();
    
    mFontTexture.unbind();
    glDisable(GL_TEXTURE);
    glDisable(GL_LIGHTING);
    
}

/*----------------------------------------------------------------------------------------------------*/

void Logo_00App::mouseDown( MouseEvent event )
{
}

void Logo_00App::resize(){
    mCamera.setAspectRatio(app::getWindowAspectRatio());
}


/*----------------------------------------------------------------------------------------------------*/


CINDER_APP_NATIVE( Logo_00App, RendererGl )
