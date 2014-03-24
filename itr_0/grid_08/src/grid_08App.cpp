#include "Resources.h"

#include "cinder/Cinder.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/DisplayList.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/ip/Hdr.h"
#include "cinder/TriMesh.h"
#include "cinder/Json.h"

#include "Config.h"

#include "FileWatcher.h"
#include "Controller.h"
#include "FrustumOrtho.h"
#include "InfoPanel.h"

#include "cinder/Utilities.h"

#include <iostream> 
#include <math.h>

using namespace ci;
using namespace ci::app;
using namespace std;

#define PATH_CONFIG_JSON "/Users/automat/Projects/next/itr_0/grid_08/resources/config.json"

#define PATH_GLSL_CELL_VERT "/Users/automat/Projects/next/itr_0/grid_08/resources/cell_vert.glsl"
#define PATH_GLSL_CELL_FRAG "/Users/automat/Projects/next/itr_0/grid_08/resources/cell_frag.glsl"

/*--------------------------------------------------------------------------------------------*/

static int   MODEL_ZOOM;
static float MODEL_SCALE;

static bool SHOW_INFO(false);

/*--------------------------------------------------------------------------------------------*/


class grid_08App : public AppBasic {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void resize();
	void initShadowMap();
	void drawDepthLight(gl::Fbo& fbo, gl::Light* light);
    void keyDown(KeyEvent event);
	void draw();
    void drawScene();
	void update();
    
    void loadShader(DataSourceRef vertRef, DataSourceRef pathRef, gl::GlslProg* shader);
    void loadFboRender();
    void updateView();
    
    
    
    FileWatcher    mFileWatcher;
    void           reloadConfig();
    
    FrustumOrtho   mFrustum;
    CameraOrtho    mCamera;
    CameraOrtho    mCameraDebug;
    Controller*    mController;
    
    /*--------------------------------------------------------------------------------------------*/
    
    gl::GlslProg mShaderDepth;
	gl::GlslProg mShader;
    
	gl::Fbo      mFboLight0Depth;
    gl::Fbo      mFboLight1Depth;
    
    gl::Fbo      mFboPost;
    
	gl::Light*   mLight0; //light up
    gl::Light*   mLight1; //light scatter
    
    void reloadShader(gl::GlslProg *prog, const string& pathVertAbs, const string& pathFragAbs);
    void watchShaderSource(const string& pathVertAbs, const string& pathFragAbs, gl::GlslProg* shader);
    
    
    
    float mTime;
    int   mFrame;
};

void grid_08App::prepareSettings(Settings* settings){
    config::Load(PATH_CONFIG_JSON);
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void grid_08App::setup(){
    /*--------------------------------------------------------------------------------------------*/
    
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio, aspectRatio, -1, 1, -10, 1.0f);
    mCamera.lookAt(Vec3f(-1,1,-1), Vec3f::zero());
    
    float cameraDebugZoom = 6.0f;
    mCameraDebug.setOrtho(-aspectRatio * cameraDebugZoom, aspectRatio * cameraDebugZoom, -cameraDebugZoom, cameraDebugZoom, -10, 100.0f);
    mCameraDebug.lookAt(Vec3f(0,1,0), Vec3f::zero());
	
    /*--------------------------------------------------------------------------------------------*/
    
    
	mLight0 = new gl::Light( gl::Light::POINT, 0 );
    mLight1 = new gl::Light( gl::Light::POINT, 1 );
    
    this->loadFboRender();
    this->loadShader(loadFile(PATH_GLSL_CELL_VERT),
                     loadFile(PATH_GLSL_CELL_FRAG), &mShader);
    
    /*--------------------------------------------------------------------------------------------*/
    
    reloadConfig();
    
    mFileWatcher.addFile(PATH_CONFIG_JSON);
    mFileWatcher.addFile(PATH_GLSL_CELL_FRAG);
    mFileWatcher.addFile(PATH_GLSL_CELL_VERT);
    mController = new Controller(WORLD_NUM_CELLS_XY,WORLD_NUM_CELLS_XY);
}

void grid_08App::reloadConfig(){
    config::Load(PATH_CONFIG_JSON);
    
    using namespace config;
    
    MODEL_SCALE = WORLD_MODEL_SCALE_INITIAL;
    MODEL_ZOOM  = WORLD_MODEL_ZOOM_INITIAL;
    
    
    mLight0->lookAt(LIGHT0_EYE, LIGHT0_TARGET);
	mLight0->setAmbient( LIGHT0_AMBIENT );
	mLight0->setDiffuse( LIGHT0_DIFFUSE );
	mLight0->setSpecular(LIGHT0_SPECULAR );
    
    mLight1->lookAt(LIGHT1_EYE, LIGHT1_TARGET);
	mLight1->setAmbient( LIGHT1_AMBIENT );
	mLight1->setDiffuse( LIGHT1_DIFFUSE );
	mLight1->setSpecular(LIGHT1_SPECULAR );
}


void grid_08App::drawScene(){
    glPushMatrix();
    glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
    mController->draw();
    glPopMatrix();
}


void grid_08App::update(){
    mFrustum.set(mCamera,1.1f);
    
    mController->update();
    mController->checkFrustum(mFrustum);
    mController->transform(MODEL_SCALE);
    
    if(mFileWatcher.fileDidChange(PATH_CONFIG_JSON)){
        reloadConfig();
        mController->reset();
    }
    
    if(mFileWatcher.fileDidChange(PATH_GLSL_CELL_VERT) ||
       mFileWatcher.fileDidChange(PATH_GLSL_CELL_FRAG)){
        loadShader(loadFile(PATH_GLSL_CELL_VERT), loadFile(PATH_GLSL_CELL_FRAG), &mShader);
    }
}

void grid_08App::draw(){
    //mFboPost.bindFramebuffer();
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl::enableDepthRead();
    
#ifdef WORLD_DEBUG_DRAW_CELL_AREA
    glPushMatrix();
    glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
    mController->debugArea();
    glPopMatrix();
#endif
#ifdef WORLD_DEBUG_DRAW_CELL
    glPushMatrix();
    glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
    mController->debugDraw();
    glPopMatrix();
#endif
    
    glEnable( GL_LIGHTING );
    gl::setMatrices(mCamera );
    mLight0->update(mCamera );
    mLight1->update(mCamera);
    mShader.bind();
#ifdef WORLD_DRAW_CELL
    this->drawScene();
#endif
	mShader.unbind();
	
    glDisable( GL_LIGHTING );
    //mFboPost.unbindFramebuffer();
    
    // Draw Fbo
    /*
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glClearColor(0,0,0, 1);
     gl::setMatricesWindow( getWindowSize(),false );
     gl::draw(mFboPost.getTexture(), app::getWindowBounds());*/
}

/*--------------------------------------------------------------------------------------------*/

void grid_08App::keyDown(KeyEvent event){
    
    switch (event.getCode()) {
        case KeyEvent::KEY_5:
            mCamera.lookAt(Vec3f(1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_6:
            mCamera.lookAt(Vec3f(0,1,0),Vec3f::zero());
            break;
        case KeyEvent::KEY_7:
            mCamera.lookAt(Vec3f(-1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_8:
            mCamera.lookAt(Vec3f(1,1,-1),Vec3f::zero());
            break;
        case KeyEvent::KEY_9:
            mCamera.lookAt(Vec3f(-1,1,-1),Vec3f::zero());
            break;
        case KeyEvent::KEY_UP:
            MODEL_ZOOM = MAX(WORLD_MODEL_ZOOM_MIN,MIN(MODEL_ZOOM+1,WORLD_MODEL_ZOOM_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_DOWN:
            MODEL_ZOOM = MAX(WORLD_MODEL_ZOOM_MIN,MIN(MODEL_ZOOM-1,WORLD_MODEL_ZOOM_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_LEFT:
            MODEL_SCALE = MAX(WORLD_MODEL_SCALE_MIN,MIN(MODEL_SCALE-WORLD_MODEL_SCALE_STEP,WORLD_MODEL_SCALE_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_RIGHT:
            MODEL_SCALE = MAX(WORLD_MODEL_SCALE_MIN,MIN(MODEL_SCALE+WORLD_MODEL_SCALE_STEP,+WORLD_MODEL_SCALE_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_f:
            SHOW_INFO = !SHOW_INFO;
            break;
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
        default:
            break;
    }
}
/*--------------------------------------------------------------------------------------------*/

void grid_08App::reloadShader(gl::GlslProg *prog, const string& pathVertAbs, const string& pathFragAbs){
    gl::GlslProg temp;
    bool success = true;
    
    try {
        temp = gl::GlslProg(loadFile(pathVertAbs),loadFile(pathFragAbs));
    } catch (gl::GlslProgCompileExc &exc) {
        cout << exc.what() << endl;
        success = false;
    } catch (...){
        success = false;
        throw "Can´t load shader.";
    }
    
    if(success){
        cout << "Shader updated." << endl;
        *prog = temp;
    }
}

void grid_08App::watchShaderSource(const string& pathVertAbs, const string& pathFragAbs, gl::GlslProg* shader){
    if(!mFileWatcher.hasFile(pathVertAbs)){
        mFileWatcher.addFile(pathVertAbs);
    }
    
    if(!mFileWatcher.hasFile(pathFragAbs)){
        mFileWatcher.addFile(pathFragAbs);
    }
    
    if (mFileWatcher.fileDidChange(pathVertAbs)) {
        cout << pathVertAbs << " modified." << endl;
        reloadShader(shader, pathVertAbs, pathFragAbs);
    }
    
    if (mFileWatcher.fileDidChange(pathFragAbs)) {
        cout << pathFragAbs << " modified." << endl;
        reloadShader(shader, pathVertAbs, pathFragAbs);
    }
}

void grid_08App::loadShader(DataSourceRef refVertGLSL, DataSourceRef refFragGLSL, gl::GlslProg *prog){
    try {
        *prog = gl::GlslProg(refVertGLSL,refFragGLSL);
    } catch (gl::GlslProgCompileExc &exc) {
        cout << exc.what() << endl;
    } catch (...){
        throw "Can't load shader.";
    }
}


/*--------------------------------------------------------------------------------------------*/

void grid_08App::updateView(){
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio * MODEL_ZOOM, aspectRatio * MODEL_ZOOM, -MODEL_ZOOM, MODEL_ZOOM, 0.0001, 5.0f);
}

void grid_08App::resize(){
	updateView();
    this->loadFboRender();
}



void grid_08App::loadFboRender(){
    gl::Fbo::Format formatFboRender;
    formatFboRender.setSamples(4);
    
    mFboPost = gl::Fbo(app::getWindowWidth(),app::getWindowHeight(),formatFboRender);
}

CINDER_APP_BASIC( grid_08App, RendererGl )