// derived from http://web.imrc.kist.re.kr/~jwkim/Glsl/
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
#include "FileWatcher.h"
#include "GeomHelper.h"
#include "cinder/TriMesh.h"

#include "GLHelper.h"

#include <iostream>
#include <math.h>

using namespace ci;
using namespace ci::app;
using namespace std;

static const int RES_SHADOW_MAP = 1024;
static const int STAGE_WIDTH(3552), STAGE_HEIGHT(1105);
static const int STAGE_SCALE(2);

static const int   APP_WIDTH(STAGE_WIDTH / STAGE_SCALE), APP_HEIGHT(STAGE_HEIGHT / STAGE_SCALE);
static const float APP_FPS(30.0);



class ShaderSetupApp : public AppBasic {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void resize();
	void initShadowMap();
	void updateShadowMap();
	void draw();
    void drawScene();
	void update();
    void reloadShader();
    void reloadFboRender();
    
    /*--------------------------------------------------------------------------------------------*/
    
    FileWatcher  mFileWatcher;
    
	gl::GlslProg mShader;
	gl::Fbo      mDepthFbo;
    gl::Fbo      mPostFbo;
	CameraPersp  mCamera;
	gl::Light*   mLight0; //light up
    gl::Light*   mLight1; //light scatter
   // TriMesh  mMesh;
    //TriMesh  mMeshDome;
    
    

    float mTime;
    int   mFrame;
};

void ShaderSetupApp::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void ShaderSetupApp::setup(){
    /*
	mCamera = new CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f );
	mCamera->lookAt( Vec3f( 5, 5, 5 ), Vec3f( 0, 0, 0 ) );
	mCamera->setPerspective( 45.0f, getWindowAspectRatio(), 0.001f, 100.0f );
     */
    mCamera.setPerspective(45.0f, getWindowAspectRatio(), 0.001f, 100.0f);
    mCamera.lookAt(Vec3f(5,5,5),Vec3f(0,0,0));
	
	mLight0 = new gl::Light( gl::Light::POINT, 0 );
	mLight0->setAmbient( Color( 0.3f, 0.3f, 0.3f ) );
	mLight0->setDiffuse( Color( 0.5f, 0.5f, 0.5f ) );
	mLight0->setSpecular( Color( 0.5f, 0.5f, 0.5f ) );
	mLight0->setShadowParams( 120.0f, 0.5f, 8.0f ); //fov, near, far
    
    mLight1 = new gl::Light( gl::Light::POINT, 1 );
    mLight1->lookAt(Vec3f(4,4,4), Vec3f::zero());
    mLight1->setAmbient( Color( 0.3f, 0.3f, 0.3f ) );
	mLight1->setDiffuse( Color( 0.5f, 0.5f, 0.5f ) );
	mLight1->setSpecular( Color( 0.5f, 0.5f, 0.5f ) );
    mLight1->setShadowParams( 120.0f, 0.5f, 8.0f );
	
	
    /*
    GeomHelper::genIcosahedron(&mMesh,2);
    GeomHelper::invertNormals(mMesh);
    
    GeomHelper::genIcosahedron(&mMeshDome,3);
    mMeshDome.recalculateNormals();
    */
    
    /*--------------------------------------------------------------------------------------------*/
    
	mDepthFbo = gl::Fbo( RES_SHADOW_MAP, RES_SHADOW_MAP, false, false, true );
	mDepthFbo.bindDepthTexture();
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture(GL_TEXTURE_2D, NULL);
    
    this->reloadFboRender();
    
    /*--------------------------------------------------------------------------------------------*/
    
    mFileWatcher.addFile(PATH_SHADOWMAP_VERT_GLSL);
    mFileWatcher.addFile(PATH_SHADOWMAP_FRAG_GLSL);
	this->reloadShader();
    
    mTime = 0;
    mFrame = 0;
}


void ShaderSetupApp::drawScene(){
    GLHelper::DrawPlane(Vec3f(0,-0.5,0), Vec2f(35,35));
    /*
    glPushMatrix();
    glTranslatef(0, -1, 0);
    glScalef(15,1,15);
    gl::draw(mMeshDome);
    glPopMatrix();
    */
    /*
    glPushMatrix();
    glScalef(8,8,8);
    gl::draw(mMeshDome);
    glPopMatrix();
    */
    gl::drawCube(Vec3f::zero(), Vec3f(1,1,1));
    gl::drawCube(Vec3f(2,-0.5,1), Vec3f(0.5,0.25,0.5));
    gl::drawCube(Vec3f(-2,-0.5,2), Vec3f(0.5,0.25,0.5));
    gl::drawCube(Vec3f(4,-0.5,0), Vec3f(0.5,0.25,0.5));
    gl::drawCube(Vec3f(-3,-0.5,-1), Vec3f(0.5,0.25,0.5));
    gl::drawCube(Vec3f(-2,-0.5,3), Vec3f(0.125,2,0.125));
    gl::drawCube(Vec3f(-1,-0.5,4), Vec3f(0.125,8,0.125));
    /*
    glPushMatrix();
    glTranslatef(2,1.25,0);
    gl::draw(mMesh);
    glPopMatrix();
    */
}

void ShaderSetupApp::updateShadowMap(){
	mDepthFbo.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonOffset( 1.0f, 1.0f );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glClear( GL_DEPTH_BUFFER_BIT );
    glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, RES_SHADOW_MAP, RES_SHADOW_MAP );
    mLight0->setShadowRenderMatrices();
   // mLight1->setShadowRenderMatrices();
    this->drawScene();
    glPopAttrib();
	glDisable( GL_POLYGON_OFFSET_FILL );
    mDepthFbo.unbindFramebuffer();
}

void ShaderSetupApp::update(){
    mTime  = app::getElapsedSeconds();
    mFrame = app::getElapsedFrames();
    
    if (mFileWatcher.fileDidChange(PATH_SHADOWMAP_VERT_GLSL)) {
        cout << PATH_SHADOWMAP_VERT_GLSL << " modified." << endl;
        this->reloadShader();
    }
    
    if (mFileWatcher.fileDidChange(PATH_SHADOWMAP_FRAG_GLSL)) {
        cout << PATH_SHADOWMAP_VERT_GLSL << " modified." << endl;
        this->reloadShader();
    }
    
    mCamera.lookAt(Vec3f(cosf(mTime*0.125f)*5,4+sinf(mTime),sinf(mTime*0.125f)*5), Vec3f::zero());
    mLight0->lookAt( Vec3f(cosf(mTime)*2,6,sinf(mTime)*2),Vec3f::zero());//Vec3f(cosf(mTime) * 6, 1.5, sinf(mTime) * 6), Vec3f( 0, 0, 0 ) );
}

void ShaderSetupApp::draw(){
	updateShadowMap();
    
    ci::gl::enableDepthRead();
    ci::gl::enableDepthWrite();
    
    mPostFbo.bindFramebuffer();
    mPostFbo.bindTexture(0);
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl::enableDepthRead();
    
    glEnable( GL_LIGHTING );
    gl::setMatrices(mCamera );
    mLight0->update(mCamera );
   // mLight1->update(mCamera);
    
    mDepthFbo.bindDepthTexture(1);
	mShader.bind();
	mShader.uniform( "depthTexture",1);
    mShader.uniform( "shadowTransMatrix", mLight0->getShadowTransformationMatrix( mCamera ) );
    //mShader.uniform( "shadowTransMatrix1", mLight1->getShadowTransformationMatrix(mCamera));
    this->drawScene();
	mShader.unbind();
	mDepthFbo.unbindTexture();
    
    glDisable( GL_LIGHTING );
    gl::drawFrustum( mLight0->getShadowCamera() );
    gl::drawFrustum( mLight1->getShadowCamera());
    mPostFbo.unbindFramebuffer();
    
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0, 1);
    gl::setMatricesWindow( getWindowSize(),false );
    gl::draw(mPostFbo.getTexture(), app::getWindowBounds());
}

/*--------------------------------------------------------------------------------------------*/

void ShaderSetupApp::resize(){
	glViewport( 0, 0, getWindowWidth(), getWindowHeight() );
	mCamera.setPerspective( 45.0f, getWindowAspectRatio(), 0.1f, 100.0f );
    this->reloadFboRender();
}

void ShaderSetupApp::reloadShader(){
    gl::GlslProg prog;
    bool success = true;
    
    try {
        prog = gl::GlslProg(loadFile(PATH_SHADOWMAP_VERT_GLSL),loadFile(PATH_SHADOWMAP_FRAG_GLSL));
    } catch (gl::GlslProgCompileExc &exc) {
        cout << exc.what() << endl;
        success = false;
    } catch (...){
        success = false;
        throw "Can´t load shader.";
    }
    
    if(success){
        cout << "Shader updated." << endl;
        mShader = prog;
    }
}

void ShaderSetupApp::reloadFboRender(){
    gl::Fbo::Format formatFboRender;
    formatFboRender.setSamples(4);
    
    mPostFbo = gl::Fbo(app::getWindowWidth(),app::getWindowHeight(),formatFboRender);
    
    
}


CINDER_APP_BASIC( ShaderSetupApp, RendererGl )

/*
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Resources.h"

#include "FileWatcher.h"
#include "GeomHelper.h"
#include "GLHelper.h"

#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const int APP_WIDTH(800),APP_HEIGHT(600);
const float FPS(30);
const float CAMERA_FOV(65.0f);
const int SHADOW_MAP_RES(1024);

class ShaderSetupApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void update();
    void resize();
	void draw();
    
    void drawScene();
    
    void reloadShader();
    
    void initShadowMap();
    void updateShadowMap();

 
    float mTime;
    float mFrame;
    
    FileWatcher mFileWatcher;
    
    CameraPersp mCamera;
    Vec3f       mCameraEye;
    Vec3f       mCameraTarget;
    
    gl::Light*   mLight;
    gl::GlslProg mShader;
    gl::GlslProg mShaderShadow;
    gl::Fbo      mDepthFbo;
    
};

void ShaderSetupApp::resize(){
    mCamera.setAspectRatio(app::getWindowAspectRatio());
}

void ShaderSetupApp::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(FPS);
}

void ShaderSetupApp::setup(){
    
    mFileWatcher.addFile(PATH_ROOM_VERT_GLSL);
    mFileWatcher.addFile(PATH_ROOM_FRAG_GLSL);
    
 
    mCamera.setPerspective(CAMERA_FOV, app::getWindowAspectRatio(), 0.0001f, 10);
    mCameraEye.set(2,2,2);
    mCameraTarget.set(0, 0, 0);
    mCamera.lookAt(mCameraEye, mCameraTarget);
    
    this->reloadShader();
    
    mLight = new gl::Light(gl::Light::POINT,0);
    mLight->setShadowParams(60.0f, 0.5f, 0.8f);
    mLight->enable();
    
    gl::enableDepthWrite();
	gl::enableDepthRead();
    
    this->initShadowMap();
    

    
    
}

void ShaderSetupApp::initShadowMap(){
    mDepthFbo = gl::Fbo(SHADOW_MAP_RES,SHADOW_MAP_RES, false, false, true);
    mDepthFbo.bindDepthTexture();
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
}

void ShaderSetupApp::update(){
    mTime  = app::getElapsedSeconds();
    mFrame = app::getElapsedFrames();
    
    if(mFileWatcher.fileDidChange(PATH_ROOM_VERT_GLSL)){
        cout << PATH_ROOM_VERT_GLSL << "modified." << endl;
        this->reloadShader();
    }
    
    if(mFileWatcher.fileDidChange(PATH_ROOM_FRAG_GLSL)){
        cout << PATH_ROOM_FRAG_GLSL << "modified." << endl;
        this->reloadShader();
    }
    
    float cameraEyeRotAngle = mTime * 0.0125f * M_PI * 2.0;
    
    mCamera.setFov(CAMERA_FOV);
    mCameraEye.set(cosf(cameraEyeRotAngle) * 3,
                   1.5f,
                   sinf(cameraEyeRotAngle) * 3);
    mCamera.lookAt(mCameraEye,mCameraTarget);

}

void ShaderSetupApp::drawScene(){
    GLHelper::DrawPlane(Vec3f(0,-0.5,0), Vec2f(6,6));
    gl::drawCube(Vec3f::zero(), Vec3f(1,1,1));
    gl::drawCube(Vec3f(2,0,1), Vec3f(0.5,0.25,0.5));
    gl::drawCube(Vec3f(-2,0,3), Vec3f(0.125,2,0.125));
}

void ShaderSetupApp::updateShadowMap(){
    mDepthFbo.bindFramebuffer();
    
	glPolygonOffset( 1.0f, 1.0f );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glClear( GL_DEPTH_BUFFER_BIT );
    
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, SHADOW_MAP_RES, SHADOW_MAP_RES );
    
    mLight->setShadowRenderMatrices();
    this->drawScene();
    
	glPopAttrib();
    
	glDisable( GL_POLYGON_OFFSET_FILL );
    
	mDepthFbo.unbindFramebuffer();
}

void ShaderSetupApp::draw()
{
	// clear out the window with black
	gl::clear();
    
    gl::enableDepthWrite();
    glEnable(GL_LIGHTING);
    this->updateShadowMap();
    
    gl::enableDepthRead();
    glEnable(GL_TEXTURE_2D);
    mDepthFbo.bindDepthTexture();
    
    mShader.bind();
    mShader.uniform("uTime", mTime);
    
    gl::setMatrices(mCamera);
    
    glColor3f(1,1,1);
    mLight->setPosition(Vec3f(cosf(mTime),1,sinf(mTime)));
    mLight->update(mCamera);
    gl::drawSphere(mLight->getPosition(), 0.05f);

    this->drawScene();
    
    mShader.unbind();
    mDepthFbo.unbindTexture();
    
    glDisable(GL_LIGHTING);
    
    //draw depth fbo
    static const int size = 128;
    gl::enableAlphaBlending();
    glColor3f(1,1,1);
    gl::setMatricesWindow(app::getWindowSize());
    gl::pushMatrices();
    mDepthFbo.getDepthTexture().enableAndBind();
    gl::drawSolidRect(Rectf(0,0,size,size));
    mDepthFbo.getDepthTexture().disable();
    gl::popMatrices();
    gl::disableAlphaBlending();
    
}

void ShaderSetupApp::reloadShader(){
    gl::GlslProg prog;
    bool success = true;
    
    try {
        prog = gl::GlslProg(loadFile(PATH_ROOM_VERT_GLSL),loadFile(PATH_ROOM_FRAG_GLSL));
    } catch (gl::GlslProgCompileExc &exc) {
        cout << exc.what() << endl;
        success = false;
    } catch (...){
        success = false;
        throw "Can´t load shader.";
    }
    
    if(success){
        cout << "Shader updated." << endl;
        mShader = prog;
    }
}

CINDER_APP_NATIVE( ShaderSetupApp, RendererGl )
*/
