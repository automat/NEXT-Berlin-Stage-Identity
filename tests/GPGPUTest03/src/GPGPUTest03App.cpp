#include "cinder/app/AppNative.h"

#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/Rand.h"
#include "FileWatcher.h"

#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

static bool        DEBUG_DRAW_FBO(true);
static const int   FBO_DIM(512);
static const float FBO_COUNT(FBO_DIM * FBO_DIM);

static const string FILE_TO_WATCH("program_template_frag.glsl");
static const string PATH_TO_WATCH("/Users/automat/Projects/next/tests/GPGPUTest03/resources/" + FILE_TO_WATCH);

static const int   APP_WINDOW_WIDTH(1024),APP_WINDOW_HEIGHT(512);
static const float APP_FPS(30.0f);

/*----------------------------------------------------------------------------------------------------*/

class GPGPUTest03App : public AppNative {
public:
    void prepareSettings(Settings* settings);
    void resize();
	void setup();
	void update();
	void draw();
    
    /*----------------------------------------------------------------------------------------------------*/
    
    FileWatcher mFileWatcher;
    void updateFileWatcher();
    
    /*----------------------------------------------------------------------------------------------------*/
    
    CameraPersp mCamera;
    Vec3f       mCameraEye;
    Vec3f       mCameraTarget;
    
    float mTime;
    float mTimeLast;
    float mTimeDelta;
    float mTimeStart;
    float mTimeElapsed;
    float mFrame;
    
    /*----------------------------------------------------------------------------------------------------*/
    
    gl::GlslProg mOutShader; //retrieve data from gpu
    gl::GlslProg mPosShader; //update postion
    
    gl::Texture  mTextureIndex; //texture containing indices
    
    gl::VboMesh mVboMesh; //data geometry representation
    
    gl::Fbo mFboPosition; //data position
    bool    mAttachmentIndex; //!mAttachmentIndex // (index + 1) % 2 //switch by !bool -> int
    
    void initShaders();
    void loadShader(gl::GlslProg& prog, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL);
    void initData();
    void initVbo();
    
    void setupFboPositionInitial();

    void applyInitialTexToFbo(Surface32f& surface, gl::Fbo& fbo);
    
    void updatePositionFbo();
    void retrieveFboData();
    
    /*----------------------------------------------------------------------------------------------------*/
};

void GPGPUTest03App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void GPGPUTest03App::resize(){
    mCamera.setAspectRatio(app::getWindowAspectRatio());
}

void GPGPUTest03App::setup(){
    mFileWatcher.addFile(PATH_TO_WATCH);
    
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(), 0.0001f, 5.0f);
    mCameraEye.set(2, 2, 2);
    mCameraTarget.set(0,0,0);
    mCamera.lookAt(mCameraEye,mCameraTarget);
    
    this->initShaders();
    this->initData();
    this->initVbo();
}

/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest03App::updateFileWatcher(){
    if(mFileWatcher.fileDidChange(PATH_TO_WATCH)){
        gl::GlslProg progTry;
        bool success = true;
        std::cout << PATH_TO_WATCH + " modified." << std::endl;
        try {
            progTry = gl::GlslProg(loadResource("passthru_vert.glsl"),loadFile(PATH_TO_WATCH));
        } catch (gl::GlslProgCompileExc& exc) {
            std::cout << exc.what()<<std::endl;
            success = false;
        } catch (...){
            success = false;
            throw "Can´t load shader.";
        }
        
        if(!success){
        } else {
            std::cout << "Shader updated." << std::endl;
            mPosShader = progTry; //copy shader
        }
    }
}

/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest03App::initShaders(){
    this->loadShader(mOutShader, loadResource("out_vert.glsl"),      loadResource("out_frag.glsl"));
    this->loadShader(mPosShader, loadResource("passthru_vert.glsl"), loadFile(PATH_TO_WATCH));
}

void GPGPUTest03App::loadShader(gl::GlslProg &prog, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL){
    try{
        prog = gl::GlslProg(refVertGLSL,refFragGLSL);
    } catch (gl::GlslProgCompileExc &exc){
        std::cout << exc.what() << std::endl;
    } catch (...){
        throw "Can't load shader.";
    }
}

/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest03App::initData(){
    mAttachmentIndex = 0;
    
    gl::Fbo::Format formatFboRGBA16;
    formatFboRGBA16.enableColorBuffer(true,2);
    formatFboRGBA16.setMinFilter(GL_NEAREST);
    formatFboRGBA16.setMagFilter(GL_NEAREST);
    formatFboRGBA16.setColorInternalFormat(GL_RGBA16F_ARB);
    
    mFboPosition = gl::Fbo(FBO_DIM,FBO_DIM,formatFboRGBA16);
    
    gl::Texture::Format formatTex;
    formatTex.setWrap(GL_REPEAT, GL_REPEAT);
    formatTex.setMinFilter(GL_NEAREST);
    formatTex.setMagFilter(GL_NEAREST);
    
    mTextureIndex = gl::Texture(FBO_DIM,FBO_DIM,formatTex);
    
    this->setupFboPositionInitial();

}

void GPGPUTest03App::setupFboPositionInitial(){
    gl::Texture& indices = mTextureIndex;
    gl::Fbo& fbo = mFboPosition;
    
    //setup fbo pos data
    Surface32f surface(fbo.getTexture()); //retrieve dims
    Surface32f::Iter itr = surface.getIter();
    int width    = fbo.getWidth();
    int width_1  = fbo.getWidth() - 1;
    int height_1 = fbo.getHeight() - 1;
    while (itr.line()) { // vertical
        while (itr.pixel()) { //horizontal
            itr.r() = -0.5f + (float)itr.x() / (float)width_1;
            itr.g() = -0.5f + Rand::randFloat();
            itr.b() = -0.5f + (float)itr.y() / (float)height_1;
            itr.a() = 1.0f; //w
        }
    }
    this->applyInitialTexToFbo(surface, fbo);
    
    //setup tex index data;
    int index;
    itr = surface.getIter(); //reuse surface
    while (itr.line()) {
        while (itr.pixel()) {
            index = itr.y() * width + itr.x();
            itr.r() = index;
            itr.g() = 0.0f;
            itr.b() = 0.0f;
            itr.a() = 1.0f;
        }
    }
    
    indices.update(surface);
}

void GPGPUTest03App::applyInitialTexToFbo(Surface32f &surface, gl::Fbo &fbo){
    gl::Texture::Format format;
    format.setWrap(GL_REPEAT, GL_REPEAT);
    format.setMinFilter(GL_NEAREST);
    format.setMagFilter(GL_NEAREST);
    
    gl::Texture texture(surface,format);
    
    fbo.bindFramebuffer();
    const GLenum buffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1}; //create ping pong color attachment
    glDrawBuffers(2, buffers);
    gl::setMatricesWindow(fbo.getSize(),false);
    gl::setViewport(fbo.getBounds());
    gl::clear();
    gl::draw(texture);
    fbo.unbindFramebuffer();
}

/*----------------------------------------------------------------------------------------------------*/


void GPGPUTest03App::initVbo(){
    gl::VboMesh::Layout layout;
    layout.setStaticPositions();
    layout.setStaticTexCoords2d();
    layout.setStaticNormals();
    
    int numVertices = FBO_DIM * FBO_DIM;
    vector<Vec2f>    texcoords;
    vector<uint32_t> indices;
    
    gl::VboMesh::Layout meshLayout;
    meshLayout.setStaticIndices();
    meshLayout.setStaticPositions();
    meshLayout.setStaticTexCoords2d();
    meshLayout.setStaticNormals();
    
    glPointSize(1.0f);
    mVboMesh = gl::VboMesh(numVertices,numVertices,meshLayout,GL_POINTS);
    int y,x;
    y = -1;
    while (++y < FBO_DIM) {
        x = -1;
        while (++x < FBO_DIM) {
            indices.push_back(y * FBO_DIM + x);
            texcoords.push_back(Vec2f(x/(float)FBO_DIM,y/(float)FBO_DIM));
        }
    }
    mVboMesh.bufferIndices(indices);
    mVboMesh.bufferTexCoords2d(0, texcoords);
    mVboMesh.unbindBuffers();
};

/*----------------------------------------------------------------------------------------------------*/


void GPGPUTest03App::updatePositionFbo(){
    mFboPosition.bindFramebuffer();
    //! bind the opposite attachment to draw to
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + !mAttachmentIndex);
    
    //! set window according to fbo size
    gl::setViewport(mFboPosition.getBounds());
    gl::setMatricesWindow(mFboPosition.getSize(),false);
    gl::clear(); //clear current color attachement
    
    //! bind current color attachment as previous data
    mFboPosition.bindTexture(0,mAttachmentIndex);
    mTextureIndex.bind(1);
    mPosShader.bind();
    
    mPosShader.uniform("uDataPosition", 0); //send data to uniform
    mPosShader.uniform("uDataIndex", 1); //data index
    mPosShader.uniform("uCount", FBO_COUNT); //num data
    mPosShader.uniform("uTime", mTime); //time
    mPosShader.uniform("uFrame",mFrame); // frames;
    
    gl::drawSolidRect(mFboPosition.getBounds());
    mPosShader.unbind();
    mFboPosition.unbindFramebuffer();
}


//Transfer data from fbo to vbo
void GPGPUTest03App::retrieveFboData(){
    //bind color_attachment last written to
    mFboPosition.bindTexture(0,mAttachmentIndex);
    mOutShader.bind();
    mOutShader.uniform("uDataPosition", 0);
    gl::draw(mVboMesh);
    mOutShader.unbind();
}


/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest03App::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeElapsed = mTime - mTimeStart;
    mTimeDelta   = mTime - mTimeLast;
    mTimeDelta   = std::max(0.0f, std::min(mTimeDelta, 1.0f));
    mFrame       = (float)app::getElapsedFrames();
    
    mCameraEye.set(sinf(mTime)*(float)M_2_PI, 1.0f, 1.0f);
    mCamera.lookAt(mCameraEye, mCameraTarget);
    
    this->updatePositionFbo();
    this->updateFileWatcher();
}

void GPGPUTest03App::draw(){
    //Draw
    gl::clear(Color::black());
    gl::setViewport(app::getWindowBounds());
    gl::setMatrices(mCamera);
    
    glColor3f(1, 1, 1);
    gl::drawStrokedCube(Vec3f::zero(), Vec3f(1,1,1));
    
    gl::enable( GL_TEXTURE_2D );
    gl::enableDepthRead();
    
    this->retrieveFboData();
    
    gl::disableDepthWrite();
    gl::disable( GL_TEXTURE_2D );
    
    if(DEBUG_DRAW_FBO){
        gl::enableAlphaBlending();
        
        glColor3f(1,1,1);
        gl::setMatricesWindow(app::getWindowSize());
        mFboPosition.getTexture(!mAttachmentIndex).enableAndBind();
        gl::drawSolidRect(Rectf(0,0,128,128));
        mFboPosition.getTexture(!mAttachmentIndex).disable();
        mFboPosition.getTexture(mAttachmentIndex).enableAndBind();
        gl::drawSolidRect(Rectf(128,0,256,128));
        mFboPosition.getTexture(mAttachmentIndex).disable();
        gl::drawString("FBO POSITION", Vec2f(20,20));
        
        mTextureIndex.enableAndBind();
        gl::drawSolidRect(Rectf(0,128,128,256));
        mTextureIndex.disable();
        gl::drawString("TEX INDEX", Vec2f(20,128 + 20));
        
        gl::disableAlphaBlending();
        
    }
    
    //Swap color attachment index
    mAttachmentIndex = !mAttachmentIndex;
    mTimeLast = mTime;
}

CINDER_APP_NATIVE( GPGPUTest03App, RendererGl )
