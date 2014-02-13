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

#include "cinder/Utilities.h"

#include <iostream>
#include <math.h>

using namespace ci;
using namespace ci::app;
using namespace std;

static const int RES_SHADOW_MAP = 1024;
static const int STAGE_WIDTH(3552), STAGE_HEIGHT(1105);
static const int STAGE_SCALE(2);

static const float BLUR_FACTOR(4.0f);
static const float BLUR_TEXEL_SIZE(1.0f/(float)RES_SHADOW_MAP);

static const int   APP_WIDTH(STAGE_WIDTH / STAGE_SCALE), APP_HEIGHT(STAGE_HEIGHT / STAGE_SCALE);
static const float APP_FPS(30.0);



class ShaderSetupApp : public AppBasic {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void resize();
	void initShadowMap();
	void drawDepthLight(gl::Fbo& fbo, gl::Light* light);
	void draw();
    void drawScene();
	void update();
    void reloadShader(gl::GlslProg* shader, const string& pathVert, const string& pathFrag);
    void watchShaderSource(gl::GlslProg* prog, const string& pathVert, const string& pathFrag);
    //void reloadDepthShader();
    void reloadFboRender();
    
    void drawDepthBuffer(gl::Fbo& fbo, const gl::Light* light);
    
    /*--------------------------------------------------------------------------------------------*/
    
    FileWatcher  mFileWatcher;
    
    gl::GlslProg mShaderDepth;
	gl::GlslProg mShader;
    gl::GlslProg mShaderBlurV5; //to blur the light depthmap
    gl::GlslProg mShaderBlurH5;
    gl::Fbo      mFboBlur;
    
    
	gl::Fbo      mFboLight0Depth;
    gl::Fbo      mFboLight1Depth;
    
    gl::Fbo      mFboDepth0;
    gl::Fbo      mFboDepth1;
    
    gl::Fbo      mFboPost;
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
    mCamera.setPerspective(45.0f, getWindowAspectRatio(), 0.5f, 16.0f);
    mCamera.lookAt(Vec3f(5,5,5),Vec3f(0,0,0));
	
	mLight0 = new gl::Light( gl::Light::POINT, 0 );
	mLight0->setAmbient( Color( 0.3f, 0.3f, 0.3f ) );
	mLight0->setDiffuse( Color( 0.5f, 0.5f, 0.5f ) );
	mLight0->setSpecular( Color( 0.5f, 0.5f, 0.5f ) );
	mLight0->setShadowParams( 120.0f, 0.5f, 8.0f ); //fov, near, far
    
    mLight1 = new gl::Light( gl::Light::POINT, 1 );
    mLight1->lookAt(Vec3f(-2,2,2), Vec3f::zero());
    mLight1->setAmbient( Color( 0.1f, 0.1f, 0.1f ) );
	mLight1->setDiffuse( Color( 0.2f, 0.2f, 0.2f ) );
	mLight1->setSpecular( Color( 0.2f, 0.2f, 0.2f ) );
    mLight1->setShadowParams( 120.0f, 0.5f, 16.0f );
	 
    /*--------------------------------------------------------------------------------------------*/
    
    // setup light depth fbos
	mFboLight0Depth = gl::Fbo( RES_SHADOW_MAP, RES_SHADOW_MAP, false, false, true );
	mFboLight0Depth.bindDepthTexture();
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );
    //glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture(GL_TEXTURE_2D, NULL);
    
    mFboLight1Depth = gl::Fbo( RES_SHADOW_MAP, RES_SHADOW_MAP, false, false, true );
	mFboLight1Depth.bindDepthTexture();
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );
    //glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE );
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture(GL_TEXTURE_2D, NULL);
    
    
    mFboDepth0 = gl::Fbo( RES_SHADOW_MAP, RES_SHADOW_MAP);
    mFboDepth1 = gl::Fbo( RES_SHADOW_MAP, RES_SHADOW_MAP);
    
    this->reloadFboRender();
    
    /*--------------------------------------------------------------------------------------------*/
    
    this->reloadShader(&mShader,       PATH_SHADOWMAP_VERT_GLSL, PATH_SHADOWMAP_FRAG_GLSL);
    this->reloadShader(&mShaderDepth,  PATH_DEPTH_VERT_GLSL, PATH_DEPTH_FRAG_GLSL);
    this->reloadShader(&mShaderBlurV5, PATH_PASS_THRU_VERT_GLSL, PATH_GAUSS_BLUR_V5_FRAG_GLSL);
    this->reloadShader(&mShaderBlurH5, PATH_PASS_THRU_VERT_GLSL, PATH_GAUSS_BLUR_H5_FRAG_GLSL);
    
    mTime = 0;
    mFrame = 0;
}


void ShaderSetupApp::drawScene(){
    //GLHelper::DrawPlane(Vec3f(0,-0.5,0), Vec2f(35,35));
    gl::drawCube(Vec3f(0,-0.5,0), Vec3f(35,0.05f,35));
    
    
    gl::drawCube(Vec3f::zero(), Vec3f(1,1,1));
    gl::drawCube(Vec3f(2,-0.5,1), Vec3f(0.5,0.25,0.5));
    gl::drawCube(Vec3f(-2,-0.5,2), Vec3f(0.5,0.25,0.5));
    gl::drawCube(Vec3f(4,-0.5,0), Vec3f(0.5,0.25,0.5));
    gl::drawCube(Vec3f(-3,-0.5,-1), Vec3f(0.5,0.25,0.5));
    gl::drawCube(Vec3f(-2,-0.5,3), Vec3f(0.125,2,0.125));
    gl::drawCube(Vec3f(-1,-0.5,4), Vec3f(0.125,8,0.125));
    
    glPushMatrix();
    glTranslatef(0, 2, 0);
    glRotatef(mTime * 64.0f, 1, 1, 1);
    gl::drawCube(Vec3f(-0.5,0,0), Vec3f(0.25f,0.25f,0.25f));
    gl::drawCube(Vec3f( 0.5,0,0), Vec3f(0.25f,0.25f,0.25f));
    gl::drawCube(Vec3f( 0,-0.5,0), Vec3f(0.25f,0.25f,0.25f));
    gl::drawCube(Vec3f( 0,0.5,0), Vec3f(0.25f,0.25f,0.25f));
    gl::drawCube(Vec3f( 0,0,-0.5), Vec3f(0.25f,0.25f,0.25f));
    gl::drawCube(Vec3f( 0,0,0.5), Vec3f(0.25f,0.25f,0.25f));
    glPopMatrix();
}

void ShaderSetupApp::drawDepthLight(gl::Fbo& fbo, gl::Light* light){
	fbo.bindFramebuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonOffset( 1.0f, 1.0f );
        glEnable( GL_POLYGON_OFFSET_FILL );
        glClear( GL_DEPTH_BUFFER_BIT );
            glPushAttrib( GL_VIEWPORT_BIT );
            glViewport( 0, 0, RES_SHADOW_MAP, RES_SHADOW_MAP );
            light->setShadowRenderMatrices();
            this->drawScene();
            glPopAttrib();
        glDisable( GL_POLYGON_OFFSET_FILL );
    fbo.unbindFramebuffer();
}

void ShaderSetupApp::drawDepthBuffer(gl::Fbo &fbo, const gl::Light *light){
    const CameraPersp& shadowCamera = light->getShadowCamera();
    fbo.bindFramebuffer();
    
    mShaderDepth.bind();
    mShaderDepth.uniform("uNear", shadowCamera.getNearClip());
    mShaderDepth.uniform("uFar",  shadowCamera.getFarClip());
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
            glPushAttrib( GL_VIEWPORT_BIT );
            glViewport( 0, 0, RES_SHADOW_MAP, RES_SHADOW_MAP );
            gl::setMatrices(shadowCamera);
            this->drawScene();
            glPopAttrib();
    mShaderDepth.unbind();
    fbo.unbindFramebuffer();
    
}

void ShaderSetupApp::update(){
    mTime  = app::getElapsedSeconds();
    mFrame = app::getElapsedFrames();
    
    /*--------------------------------------------------------------------------------------------*/
    
    this->watchShaderSource(&mShader,      PATH_SHADOWMAP_VERT_GLSL, PATH_SHADOWMAP_FRAG_GLSL);
    this->watchShaderSource(&mShaderDepth, PATH_DEPTH_VERT_GLSL, PATH_DEPTH_FRAG_GLSL);
    
    /*--------------------------------------------------------------------------------------------*/
    
    mCamera.lookAt(Vec3f(cosf(mTime*0.125f)*5,4+sinf(mTime),sinf(mTime*0.125f)*5), Vec3f::zero());
    mLight0->lookAt( Vec3f(cosf(mTime)*2.25f,3.0+(0.5 + sinf(mTime) * 0.5) * 3.0,sinf(mTime)*0.25f),Vec3f::zero());//Vec3f(cosf(mTime) * 6, 1.5, sinf(mTime) * 6), Vec3f( 0, 0, 0 ) );
}

void ShaderSetupApp::draw(){
    drawDepthLight(mFboLight0Depth, mLight0);
    drawDepthLight(mFboLight1Depth, mLight1);
    
    /*--------------------------------------------------------------------------------------------*/
    
    drawDepthBuffer(mFboDepth0, mLight0);
    drawDepthBuffer(mFboDepth1, mLight1);
    
    /*--------------------------------------------------------------------------------------------*/
    
    
    mFboPost.bindFramebuffer();
    mFboPost.bindTexture(0);
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl::enableDepthRead();
    
    glEnable( GL_LIGHTING );
    gl::setMatrices(mCamera );
    mLight0->update(mCamera );
    mLight1->update(mCamera);
    
    mFboLight0Depth.bindDepthTexture(1);
    mFboLight1Depth.bindDepthTexture(2);
	mShader.bind();
	mShader.uniform( "uLightDepthTexture0",1);
    mShader.uniform( "uLightDepthTexture1",2);
    mShader.uniform( "uShadow0Matrix", mLight0->getShadowTransformationMatrix(mCamera));
    mShader.uniform( "uShadow1Matrix", mLight1->getShadowTransformationMatrix(mCamera));
    this->drawScene();
	mShader.unbind();
	mFboLight0Depth.unbindTexture();
    mFboLight1Depth.unbindTexture();
    
    glDisable( GL_LIGHTING );
    gl::drawFrustum( mLight0->getShadowCamera() );
    gl::drawFrustum( mLight1->getShadowCamera());
    mFboPost.unbindFramebuffer();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0, 1);
    gl::setMatricesWindow( getWindowSize(),false );
    gl::draw(mFboPost.getTexture(), app::getWindowBounds());
    glDisable(GL_DEPTH_TEST);
    
    /*
    gl::setMatricesWindow( getWindowSize(),false );
    Surface32f shadowMapSurface( mFboLight1Depth.getDepthTexture() );
    ip::hdrNormalize( &shadowMapSurface );
    gl::color( Color::white() );
    gl::draw( gl::Texture( shadowMapSurface ), Rectf( 512, 0, 1024, 512 ) );
    */
    
    gl::setMatricesWindow( getWindowSize(),false );
    glPushMatrix();
    glTranslatef(0, app::getWindowHeight() - 256, 0);;
    gl::draw(mFboDepth0.getTexture(), Rectf(0,0,256,256));
    gl::draw(mFboDepth1.getTexture(), Rectf(256,0,512,256));
    glPopMatrix();
    
    
    
    // draw fps
    gl::setMatricesWindow( getWindowSize(),true );
    gl::enableAlphaBlending();
    glColor3f(1, 1, 1);
    gl::drawString(toString(getAverageFps()), ci::Vec2f(10,10));
    gl::disableAlphaBlending();
    
    glEnable(GL_DEPTH_TEST);
    
    
    
   
}

/*--------------------------------------------------------------------------------------------*/

void ShaderSetupApp::resize(){
	glViewport( 0, 0, getWindowWidth(), getWindowHeight() );
	mCamera.setPerspective( 45.0f, getWindowAspectRatio(), 0.1f, 100.0f );
    this->reloadFboRender();
}

void ShaderSetupApp::watchShaderSource(gl::GlslProg* prog, const string &pathVert, const string &pathFrag){
    if(!mFileWatcher.hasFile(pathVert)){
        mFileWatcher.addFile(pathVert);
    }
    
    if(!mFileWatcher.hasFile(pathFrag)){
        mFileWatcher.addFile(pathFrag);
    }
    
    if (mFileWatcher.fileDidChange(pathVert)) {
        cout << pathVert << " modified." << endl;
        this->reloadShader(prog, pathVert, pathFrag);
    }
    
    if (mFileWatcher.fileDidChange(pathFrag)) {
        cout << pathFrag << " modified." << endl;
        this->reloadShader(prog, pathVert, pathFrag);
    }

}

void ShaderSetupApp::reloadShader(gl::GlslProg* shader, const string& pathVert, const string& pathFrag){
    gl::GlslProg prog;
    bool success = true;
    
    try {
        prog = gl::GlslProg(loadFile(pathVert),loadFile(pathFrag));
    } catch (gl::GlslProgCompileExc &exc) {
        cout << exc.what() << endl;
        success = false;
    } catch (...){
        success = false;
        throw "Can´t load shader.";
    }
    
    if(success){
        cout << "Shader updated." << endl;
        *shader = prog;
    }
}


void ShaderSetupApp::reloadFboRender(){
    gl::Fbo::Format formatFboRender;
    formatFboRender.setSamples(4);
    
    mFboPost = gl::Fbo(app::getWindowWidth(),app::getWindowHeight(),formatFboRender);
    
    
}

CINDER_APP_BASIC( ShaderSetupApp, RendererGl )