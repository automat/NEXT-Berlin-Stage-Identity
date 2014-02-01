#include "cinder/app/AppNative.h"

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

#include "Room.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// Stage size: 3552 x 1105

static const int STAGE_WIDTH(3552), STAGE_HEIGHT(1105);
static const int STAGE_SCALE(2);

static const int   APP_WIDTH(STAGE_WIDTH / STAGE_SCALE), APP_HEIGHT(STAGE_HEIGHT / STAGE_SCALE);
static const float APP_FPS(30.0);

static const float ROOM_SIZE(5.0f);

static float CAMERA_FOV(65.0f);

class Room_00App : public AppNative {
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

    Room mRoom;
    
    
    params::InterfaceGlRef mParams;

};

void Room_00App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
    //settings->setResizable(false);
}

void Room_00App::setup(){
    mCamera.setPerspective(CAMERA_FOV, app::getWindowAspectRatio(), 0.00001f, ROOM_SIZE * 2);
    mCameraEye.set(ROOM_SIZE * 0.5, ROOM_SIZE * 0.25, ROOM_SIZE * 0.5);
    mCameraTarget.set(0, 0, 0);
    mCamera.lookAt(mCameraEye, mCameraTarget);
    
    /*----------------------------------------------------------------------------------------------------*/

    mRoom = Room(ROOM_SIZE,&mCamera);
    
    /*----------------------------------------------------------------------------------------------------*/
    
    mParams = params::InterfaceGl::create(app::getWindow(), "CONTROL", ci::Vec2f(200,100));
    mParams->addText("CAMERA");
    mParams->addParam("fov", &CAMERA_FOV,"min=0.1 max=120.0 step=0.5");
    mParams->minimize();
    
}

/*----------------------------------------------------------------------------------------------------*/

void Room_00App::update(){
    mTime  = app::getElapsedSeconds();
    mFrame = app::getElapsedFrames();
    
    float cameraEyeRotAngle = mTime * 0.0125f * M_PI * 2.0;
    const Vec3f& roomDim = mRoom.getDim();
    
    mCamera.setFov(CAMERA_FOV);
    mCameraEye.set(cosf(cameraEyeRotAngle) * roomDim.x * 0.5f,
                   roomDim.y * 0.25f,
                   sinf(cameraEyeRotAngle) * roomDim.y * 0.5f);
    mCamera.lookAt(mCameraEye,mCameraTarget);
    
    mRoom.update();
}

void Room_00App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    // Set camera view
    gl::setViewport(app::getWindowBounds());
    gl::setMatrices(mCamera);
    gl::enableDepthRead();
    gl::drawCoordinateFrame(2.0f);
    
    
    mRoom.draw();
    
    mParams->draw();
}

/*----------------------------------------------------------------------------------------------------*/

void Room_00App::mouseDown( MouseEvent event )
{
}

void Room_00App::resize(){
    mCamera.setAspectRatio(app::getWindowAspectRatio());
}


/*----------------------------------------------------------------------------------------------------*/


CINDER_APP_NATIVE( Room_00App, RendererGl )
