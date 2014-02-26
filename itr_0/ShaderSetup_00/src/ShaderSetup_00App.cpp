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
#include <iostream>
#include <math.h>

#include "FileWatcher.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const int   RES_SHADOW_MAP(2048);
static const float BLUR_FACTOR(4.0f);
static const float BLUR_TEXEL_SIZE(1.0f/(float)RES_SHADOW_MAP);

static const int   STAGE_WIDTH(3552), STAGE_HEIGHT(1105);
static const int   STAGE_SCALE(2);

static const int   APP_WIDTH(STAGE_WIDTH / STAGE_SCALE), APP_HEIGHT(STAGE_HEIGHT / STAGE_SCALE);
static const float APP_FPS(30.0);

/*--------------------------------------------------------------------------------------------*/

class ShaderSetup_00App : public AppBasic {
  public:
	void setup();
    void prepareSettings(Settings* settings);

  	void update();
    void draw();
    
    
    void drawSceneDepth();
    void drawSceneShadowMap();
    void drawScene();
    void drawSceneRender();
    
    void drawTexture(uint textureUnit, const Rectf& rect);
    
    void setShadowMapTexMatrix();
    
    float mTime;
    int   mFrame;
    
    uint  mFbo1Id;
    uint  mFbo2Id;
    uint  mDepthTexture1Id;
    uint  mColorTexture1Id;
    uint  mColorTexture2Id;
    float mLinearDepthScalar;
    
    Matrix44f mMatModelView;
    Matrix44f mMatProjection;
    
    gl::GlslProg mShaderDepth;
    gl::GlslProg mShaderRender;
    gl::GlslProg mShaderBlurH;
    
    CameraPersp  mCamera;
    gl::Light*   mLight;
    gl::Material mMaterial;
    
    FileWatcher mFileWatcher;
    void watchShaderSource(gl::GlslProg* shader, const string& pathVert, const string& pathFrag);
    void reloadShader(gl::GlslProg* shader, const string& pathVert, const string& pathFrag);
    
};

void ShaderSetup_00App::setup(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(), 0.001f, 16.0f);
    mCamera.lookAt(Vec3f(3,3,3),Vec3f::zero());
    
    float lightNear = 0.001f;
    float lightFar  = 40.0f;
    mLinearDepthScalar = 1.0f / (lightFar - lightNear);

    
    mLight = new gl::Light( gl::Light::POINT, 0 );
	mLight->setAmbient( Color( 0.3f, 0.3f, 0.3f ) );
	mLight->setDiffuse( Color( 0.5f, 0.5f, 0.5f ) );
	mLight->setSpecular( Color(1,1,1 ) );
    mLight->lookAt(Vec3f(4,4,4), Vec3f::zero());
	mLight->setShadowParams( 120.0f, lightNear, lightFar ); //fov, near, far
    mLight->enable();
    
    mMaterial.setEmission(ColorAf(0.125,0,0.15,1.0));
    mMaterial.setAmbient(ColorAf(0.25,0,0,1));
    mMaterial.setDiffuse(ColorAf(1,0,1,1));
    mMaterial.setSpecular(ColorAf(1,1,1,1));
    mMaterial.setShininess(2.0f);
    
    //
    //  Setup fbo
    //
    
    GLfloat border[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glActiveTexture(GL_TEXTURE0);
    
    //  depth texture
    glGenTextures(1, &mDepthTexture1Id);
    glBindTexture(GL_TEXTURE_2D, mDepthTexture1Id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, RES_SHADOW_MAP, RES_SHADOW_MAP, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
    glBindTexture(GL_TEXTURE_2D, NULL);
    
    
    //  color texture
    glGenTextures(1, &mColorTexture1Id);
    glBindTexture(GL_TEXTURE_2D, mColorTexture1Id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, RES_SHADOW_MAP, RES_SHADOW_MAP, 0, GL_LUMINANCE, GL_FLOAT, 0);
    glBindTexture(GL_TEXTURE_2D, NULL);
    
    //  gen framebuffer for depth texture
    glGenFramebuffers(1, &mFbo1Id);
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo1Id);
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture1Id, 0 );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture1Id, 0 );
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    
    // gen pingpong fbo for blur
    glGenFramebuffers(1, &mFbo2Id);
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo2Id);
        glGenTextures(1, &mColorTexture2Id);
        glBindTexture(GL_TEXTURE_2D, mColorTexture2Id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, RES_SHADOW_MAP, RES_SHADOW_MAP, 0, GL_LUMINANCE, GL_FLOAT, 0);
        glBindTexture(GL_TEXTURE_2D, NULL);
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture2Id, 0 );
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    
    
    
    //  check fbo status
    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(fboStatus != GL_FRAMEBUFFER_COMPLETE){
        throw "Incomplete frambuffer gen.";
    }
    
    
    
    
    //
    //  Create Shaders
    //
    
    this->reloadShader(&mShaderDepth, PATH_DEPTH_VERT_GLSL,     PATH_DEPTH_FRAG_GLSL);
    this->reloadShader(&mShaderRender, PATH_SHADOWMAP_VERT_GLSL, PATH_SHADOWMAP_FRAG_GLSL);
    this->reloadShader(&mShaderBlurH, PATH_PASS_THRU_VERT_GLSL, PATH_GAUSS_BLUR_H5_FRAG_GLSL);
    
    
    
    
}

void ShaderSetup_00App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void ShaderSetup_00App::update(){
    mTime  = app::getElapsedSeconds();
    mFrame = app::getElapsedFrames();
    
    this->watchShaderSource(&mShaderDepth,  PATH_DEPTH_VERT_GLSL,     PATH_DEPTH_FRAG_GLSL);
    this->watchShaderSource(&mShaderRender, PATH_SHADOWMAP_VERT_GLSL, PATH_SHADOWMAP_FRAG_GLSL);
    
    //mCamera.lookAt(Vec3f(cosf(mTime) * 3,3,sinf(mTime) * 3), Vec3f::zero());
    float lightDist = 10;
    mLight->lookAt(Vec3f(cosf(-mTime*0.25f) * lightDist,8 + (0.5f + sinf(mTime) * 0.5f), sinf(-mTime*0.25f) * lightDist),Vec3f::zero());
}

void ShaderSetup_00App::draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);

	this->drawSceneDepth();
    this->drawSceneRender();
    
    //  debug
    this->drawTexture(mColorTexture1Id, Rectf(0,0,512,512));
}

void ShaderSetup_00App::drawSceneDepth(){
    glBindFramebuffer(GL_FRAMEBUFFER, mFbo1Id);
        mShaderDepth.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1, 1, 1, 1);
        //glEnable(GL_CULL_FACE); // cull front faces - this helps with artifacts and shadows with exponential shadow mapping
        //glCullFace(GL_FRONT);
    glPolygonOffset( 1.0f, 1.0f );
    glEnable( GL_POLYGON_OFFSET_FILL );
    
        glEnable(GL_DEPTH_TEST);
        glPushAttrib( GL_VIEWPORT_BIT );
            mShaderDepth.uniform("uLinearDepthConstant", mLinearDepthScalar);
            glViewport( 0, 0, RES_SHADOW_MAP, RES_SHADOW_MAP );
            mLight->setShadowRenderMatrices();
            this->drawScene();
        glPopAttrib();
    glDisable( GL_POLYGON_OFFSET_FILL );
        //glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        mShaderDepth.unbind();
    
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

void ShaderSetup_00App::drawScene(){
    gl::drawCube(Vec3f(0,-0.5,0), Vec3f(40,0.05f,40));
    gl::drawCube(Vec3f::zero(), Vec3f(1,1,1));
    gl::drawCube(Vec3f(0,2,0), Vec3f(1,1,1));
}

void ShaderSetup_00App::drawSceneRender(){
    glPushAttrib(GL_VIEWPORT_BIT);
        glEnable(GL_DEPTH_TEST);
        mShaderRender.bind();
        glEnable(GL_TEXTURE_2D);
    
        const int texUnitShadowMapColor = 1;
    
        //  bind shadow map color texture to texture unit 1
        glActiveTexture(GL_TEXTURE0 + texUnitShadowMapColor);
        glBindTexture(GL_TEXTURE_2D, mColorTexture1Id);
    
        mShaderRender.uniform("u_ShadowMap", texUnitShadowMapColor);// send color
        mShaderRender.uniform("u_LinearDepthConstant", mLinearDepthScalar);// send linear depth scalar
    
        //shadow matrix
        static const Matrix44f bias(0.5, 0.0, 0.0, 0.0,
                                    0.0, 0.5, 0.0, 0.0,
                                    0.0, 0.0, 0.5, 0.0,
                                    0.5, 0.5, 0.5, 1.0);
         

    
    
        mShaderRender.uniform("u_ShadowTransMatrix",bias * mLight->getShadowTransformationMatrix(mCamera));
        //mShaderRender.uniform("key", )
    
            gl::setViewport(app::getWindowBounds());
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            gl::enableDepthRead();
            glEnable(GL_LIGHTING);
            gl::setMatrices(mCamera);
                mMaterial.apply();
                mLight->update(mCamera);
                this->drawScene();
            glDisable(GL_LIGHTING);
    
        //  unbind shadow map texture form unit 1 & reset active texture to 0
        glActiveTexture(GL_TEXTURE0 + texUnitShadowMapColor);
        glBindTexture(GL_TEXTURE_2D, NULL);
        glActiveTexture(GL_TEXTURE0);
    
        glDisable(GL_TEXTURE_2D);
        mShaderRender.unbind();
    
        // debug draw light frustum
        glColor3f(1,1,1);
        gl::drawFrustum(mLight->getShadowCamera());
        glDisable(GL_DEPTH_TEST);
    
    glPopAttrib();
}



void ShaderSetup_00App::drawTexture(uint textureUnit, const Rectf& rect){
    glEnable(GL_TEXTURE_2D);
    glPushAttrib(GL_VIEWPORT_BIT);
        gl::setMatricesWindow(app::getWindowSize(), false);
        glBindTexture(GL_TEXTURE_2D, textureUnit);
        gl::drawSolidRect(Rectf(rect.x1,app::getWindowHeight() - rect.y2,
                                rect.x2,app::getWindowHeight() - rect.y1));
        glBindTexture(GL_TEXTURE_2D, NULL);
    glPopAttrib();
    glDisable(GL_TEXTURE_2D);
}


void ShaderSetup_00App::watchShaderSource(gl::GlslProg* prog, const string &pathVert, const string &pathFrag){
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

void ShaderSetup_00App::reloadShader(gl::GlslProg* shader, const string& pathVert, const string& pathFrag){
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

CINDER_APP_BASIC( ShaderSetup_00App, RendererGl )
