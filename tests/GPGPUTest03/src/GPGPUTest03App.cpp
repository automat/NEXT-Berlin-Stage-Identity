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
static const int   FBO_DIM(1024);
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
    
    void mouseMove (MouseEvent event);
    void mouseDown (MouseEvent event);
    void mouseUp   (MouseEvent event);
    void mouseWheel(MouseEvent event);
    
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
    
    Vec4f mDataApplication;
    Vec4f mDataMouse;
    Vec3f mDataArray;
    
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
    
    void setupFboProgram();

    void updatePositionFbo();
    void retrieveFboData();
    
    /*----------------------------------------------------------------------------------------------------*/
};

void GPGPUTest03App::setup(){
    mFileWatcher.addFile(PATH_TO_WATCH);
    
    mDataArray.x = FBO_DIM;
    mDataArray.y = FBO_DIM;
    mDataArray.z = FBO_COUNT;
    
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(), 0.0001f, 5.0f);
    mCameraEye.set(2, 2, 2);
    mCameraTarget.set(0,0,0);
    mCamera.lookAt(mCameraEye,mCameraTarget);
    
    mDataApplication.x = app::getWindowWidth();
    mDataApplication.y = app::getWindowHeight();
    mDataApplication.z = 0.0;
    mDataApplication.w = 0.0;
    
    mDataMouse.x = 0.0;
    mDataMouse.y = 0.0;
    mDataMouse.z = 0.0;
    mDataMouse.w = 0.0;
    
    this->initShaders();
    this->initData();
    this->initVbo();
}

void GPGPUTest03App::mouseMove(MouseEvent event){
    mDataMouse.x = event.getX();
    mDataMouse.y = event.getY();
}

void GPGPUTest03App::mouseDown(MouseEvent event){
    mDataMouse.z = 1;
}

void GPGPUTest03App::mouseUp(MouseEvent event){
    mDataMouse.z = 0;
}

void GPGPUTest03App::mouseWheel(MouseEvent event){
    mDataMouse.w += event.getWheelIncrement();
}

void GPGPUTest03App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void GPGPUTest03App::resize(){
    mCamera.setAspectRatio(app::getWindowAspectRatio());
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
    this->setupFboProgram();
}

void GPGPUTest03App::setupFboProgram(){
    //Setup fbo & textures
    gl::Fbo::Format formatFboRGBA16;
    formatFboRGBA16.enableColorBuffer(true,2);
    formatFboRGBA16.setMinFilter(GL_NEAREST);
    formatFboRGBA16.setMagFilter(GL_NEAREST);
    formatFboRGBA16.setColorInternalFormat(GL_RGBA16F_ARB);
    const GLenum buffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    mFboPosition = gl::Fbo(FBO_DIM,FBO_DIM,formatFboRGBA16);
    
    //setup fbo program data
    Surface32f surface(mFboPosition.getTexture()); //retrieve dims
    Surface32f::Iter itr(surface.getIter());
    
    while (itr.line()) { // vertical
        while (itr.pixel()) { //horizontal
            itr.r() = 0.0f;//-0.5f + (float)itr.x() / (float)width_1;
            itr.g() = 0.0f;//-0.5f + Rand::randFloat();
            itr.b() = 0.0f;//-0.5f + (float)itr.y() / (float)height_1;
            itr.a() = 1.0f; //w
        }
    }
    
    gl::Texture::Format formatTex;
    formatTex.setWrap(GL_REPEAT, GL_REPEAT);
    formatTex.setMinFilter(GL_NEAREST);
    formatTex.setMagFilter(GL_NEAREST);
    
    //pass data texture to fbo
    gl::Texture texture(surface,formatTex);
    mFboPosition.bindFramebuffer();
    glDrawBuffers(2, buffers);
    gl::setMatricesWindow(mFboPosition.getSize(),false);
    gl::setViewport(mFboPosition.getBounds());
    gl::clear();
    gl::draw(texture);
    mFboPosition.unbindFramebuffer();
    
    //setup tex index data
    
    mTextureIndex = gl::Texture(FBO_DIM,FBO_DIM,formatTex);
    int width = mFboPosition.getWidth();
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
    
    mTextureIndex.update(surface);
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
    
    mPosShader.uniform("uDataApplication", mDataApplication); // send data application
    mPosShader.uniform("uDataMouse", mDataMouse);             // send data mouse
    mPosShader.uniform("uDataIndex", 1);                      // send data indices
    mPosShader.uniform("uCount", FBO_COUNT);                  // send data count
    mPosShader.uniform("uDataPosition", 0);                   // send data prev position
    mPosShader.uniform("uDataArray", mDataArray);             // send data array
    
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
    mTime              = (float)app::getElapsedSeconds();
    mTimeElapsed       = mTime - mTimeStart;
    mTimeDelta         = mTime - mTimeLast;
    mTimeDelta         = std::max(0.0f, std::min(mTimeDelta, 1.0f));
    mDataApplication.z = mTime;
    mDataApplication.w = (float)app::getElapsedFrames();
    
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
        
        gl::disableAlphaBlending();
        
    }
    
    //Swap color attachment index
    mAttachmentIndex = !mAttachmentIndex;
    mTimeLast = mTime;
}

CINDER_APP_NATIVE( GPGPUTest03App, RendererGl )
