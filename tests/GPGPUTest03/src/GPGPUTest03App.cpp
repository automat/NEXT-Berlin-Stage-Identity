#include "cinder/app/AppNative.h"

#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include <cstdio>
#include <unistd.h>

#include "cinder/Rand.h"
#include "FileWatcher.h"

#include "cinder/params/Params.h"

#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

static bool        DEBUG_DRAW_FBO(true);
static bool        DRAW_BOX(true);
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
    
    gl::GlslProg mOutShader; // retrieve data from gpu
    gl::GlslProg mPosShader; // update postion
    GLenum       mBuffers[6];
    size_t       mBuffersSize;
    
    gl::Fbo      mFboPosition;     // data position
    gl::Texture  mTextureIndex;    // texture containing indices
    gl::VboMesh  mVboMesh;         // data geometry representation
    bool         mAttachmentIndex; // !mAttachmentIndex // (index + 1) % 2 //switch by !bool -> int
    
    void initShaders();
    void loadShader(gl::GlslProg& prog, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL);
    void initData();
    void initVbo();
    
    void setupFboProgram();

    void updatePositionFbo();
    void retrieveFboData();
    
    /*----------------------------------------------------------------------------------------------------*/

    params::InterfaceGlRef mParams;
};

void GPGPUTest03App::setup(){
    mDataArray.x = (float)FBO_DIM;   // fbo width
    mDataArray.y = (float)FBO_DIM;   // fbo height
    mDataArray.z = (float)FBO_COUNT; // fbo count
    
    mDataApplication.x = app::getWindowWidth();  // width
    mDataApplication.y = app::getWindowHeight(); // height
    mDataApplication.z = 0.0;                    // time
    mDataApplication.w = 0.0;                    // frame
    
    mDataMouse.x = 0.0; // x
    mDataMouse.y = 0.0; // y
    mDataMouse.z = 0.0; // mouse down
    mDataMouse.w = 0.0; // mouse wheel
    
    
    mBuffers[0] = GL_COLOR_ATTACHMENT0; // position ping
    mBuffers[1] = GL_COLOR_ATTACHMENT1; // position pong
    mBuffers[2] = GL_COLOR_ATTACHMENT2; // color ping
    mBuffers[3] = GL_COLOR_ATTACHMENT3; // color pong;
    mBuffers[4] = GL_COLOR_ATTACHMENT4; // size ping
    mBuffers[5] = GL_COLOR_ATTACHMENT5; // size pong;
    mBuffersSize = sizeof(mBuffers) / sizeof(mBuffers[0]);
    
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    
    mParams = params::InterfaceGl::create(app::getWindow(), "CONTROL", Vec2f(200,100));
    mParams->addParam("Show debug view fbo", &DEBUG_DRAW_FBO);
    mParams->minimize();

    mFileWatcher.addFile(PATH_TO_WATCH);
    
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(), 0.0001f, 5.0f);
    mCameraEye.set(2, 2, 2);
    mCameraTarget.set(0,0,0);
    mCamera.lookAt(mCameraEye,mCameraTarget);
    
    this->initShaders();
    this->initData();
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
    this->initVbo();
}

void GPGPUTest03App::setupFboProgram(){
    //Setup fbo & textures
    gl::Fbo::Format formatFboRGBA16;
    formatFboRGBA16.enableColorBuffer(true,mBuffersSize);
    formatFboRGBA16.setMinFilter(GL_NEAREST);
    formatFboRGBA16.setMagFilter(GL_NEAREST);
    formatFboRGBA16.setColorInternalFormat(GL_RGBA16F_ARB);
    
    mFboPosition = gl::Fbo(FBO_DIM,FBO_DIM,formatFboRGBA16);
    
    //setup fbo program data
    Surface32f surface(mFboPosition.getTexture()); //retrieve dims
    Surface32f::Iter itr(surface.getIter());
    
    int width    = mFboPosition.getWidth();
    int height   = mFboPosition.getHeight();
    int width_1  = width - 1;
    int height_1 = height - 1;
    
    while (itr.line()) { // vertical
        while (itr.pixel()) { //horizontal
            itr.r() =-0.5f + (float)itr.x() / (float)width_1;
            itr.g() = 0.0f;
            itr.b() =-0.5f + (float)itr.y() / (float)height_1;
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
    glDrawBuffers(mBuffersSize - 2, mBuffers); // fill all except size buffe
    gl::setMatricesWindow(mFboPosition.getSize(),false);
    gl::setViewport(mFboPosition.getBounds());
    gl::clear();
    gl::draw(texture);
    mFboPosition.unbindFramebuffer();
    
    // fill size data
    itr = surface.getIter();
    while (itr.line()) {
        while (itr.pixel()) {
            itr.r() = 1.0f;
            itr.g() = itr.b() = 0.0; // unused
            itr.a() = 1.0; // for debug display
        }
    }
    
    
    GLenum buffersToFill[2] = {
        mBuffers[4], mBuffers[5]
    };
    
    texture.update(surface);
    mFboPosition.bindFramebuffer();
    glDrawBuffers(2, buffersToFill);
    gl::setMatricesWindow(mFboPosition.getSize(), false);
    gl::setViewport(mFboPosition.getBounds());
    gl::clear();
    gl::draw(texture);
    mFboPosition.unbindFramebuffer();
    
    //setup tex index data
    mTextureIndex = gl::Texture(FBO_DIM,FBO_DIM,formatTex);
    int index;
    itr = surface.getIter(); //reuse surface
    while (itr.line()) {
        while (itr.pixel()) {
            index = itr.y() * width + itr.x();
            itr.r() = index;
            itr.g() = 0.0f;
            itr.b() = 0.0f;
            itr.a() = 1.0f; // for debug display
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
    int indexPing     = mAttachmentIndex;  // position current
    int indexPong     = !mAttachmentIndex; // position next
    int indexPosPrev  = indexPing;
    int indexPosNext  = indexPong;
    int indexColPrev  = indexPing + 2;
    int indexColNext  = indexPong + 2;
    int indexSizePrev = indexPing + 4;
    int indexSizeNext = indexPong + 4;
    
    int numBuffersToDraw  = 3;
    GLenum buffersToDraw[numBuffersToDraw];
    buffersToDraw[0] = GL_COLOR_ATTACHMENT0 + indexPosNext;
    buffersToDraw[1] = GL_COLOR_ATTACHMENT0 + indexColNext;
    buffersToDraw[2] = GL_COLOR_ATTACHMENT0 + indexSizeNext;
    
    mFboPosition.bindFramebuffer();
    // bind the opposite attachment to draw to
    // glDrawBuffer(GL_COLOR_ATTACHMENT0 + !mAttachmentIndex);
    glDrawBuffers(numBuffersToDraw, buffersToDraw);
    
    // set window according to fbo size
    gl::setViewport(mFboPosition.getBounds());
    gl::setMatricesWindow(mFboPosition.getSize(),false);
    gl::clear(); //clear current color attachement
    
    // bind current color attachment as previous data
    mFboPosition.bindTexture(0,indexPosPrev);
    mFboPosition.bindTexture(1,indexColPrev);
    mFboPosition.bindTexture(2,indexSizePrev);
    mTextureIndex.bind(3);
    mPosShader.bind();
    
    mPosShader.uniform("uDataApplication", mDataApplication); // send data application
    mPosShader.uniform("uDataMouse", mDataMouse);             // send data mouse
    mPosShader.uniform("uDataArray", mDataArray);             // send data array
    mPosShader.uniform("uDataIndex", 3);                      // send data indices
    
    mPosShader.uniform("uDataPosition", 0);                   // send data prev position
    mPosShader.uniform("uDataColor", 1);                      // send data prev color
    mPosShader.uniform("uDataSize", 2);                       // send data prev size
    
    gl::drawSolidRect(mFboPosition.getBounds());
    mPosShader.unbind();
    mFboPosition.unbindFramebuffer();
}


//Transfer data from fbo to vbo
void GPGPUTest03App::retrieveFboData(){
    int indexPos  = mAttachmentIndex;
    int indexCol  = mAttachmentIndex + 2;
    int indexSize = mAttachmentIndex + 4;
    //bind color_attachment last written to
    mFboPosition.bindTexture(0,indexPos);
    mFboPosition.bindTexture(1,indexCol);
    mFboPosition.bindTexture(2,indexSize);
    mOutShader.bind();
    mOutShader.uniform("uDataPosition", 0);
    mOutShader.uniform("uDataColor", 1);
    mOutShader.uniform("uDataSize", 2);
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
    gl::enableAlphaBlending();
    
    this->retrieveFboData();
    
    gl::disableDepthWrite();
    gl::disableAlphaBlending();
    gl::disable( GL_TEXTURE_2D );
    
    if(DEBUG_DRAW_FBO){
        gl::enableAlphaBlending();
        
        int indexPing     = !mAttachmentIndex;
        int indexPong     = mAttachmentIndex;
        int indexPosPrev  = indexPing;
        int indexPosNext  = indexPong;
        int indexColPrev  = indexPing + 2;
        int indexColNext  = indexPong + 2;
        int indexSizePrev = indexPing + 4;
        int indexSizeNext = indexPong + 4;
        
        static const float paddingTL = 10.0f;
        static const int   size = 64;

        
        glColor3f(1,1,1);
        gl::setMatricesWindow(app::getWindowSize());
        
        gl::pushMatrices();
        gl::translate(Vec2f(app::getWindowWidth() - size * 2,0));
        
        mFboPosition.getTexture(indexPosPrev).enableAndBind();
        gl::drawSolidRect(Rectf(0,0,size,size));
        mFboPosition.getTexture(indexPosPrev).disable();
        mFboPosition.getTexture(indexPosNext).enableAndBind();
        gl::drawSolidRect(Rectf(size,0,size * 2,size));
        mFboPosition.getTexture(indexPosNext).disable();
        gl::drawString("FBO POSITION", Vec2f(paddingTL,paddingTL));
        
        gl::translate(0, size);
        mFboPosition.getTexture(indexColPrev).enableAndBind();
        gl::drawSolidRect(Rectf(0,0,size,size));
        mFboPosition.getTexture(indexColPrev).disable();
        mFboPosition.getTexture(indexColNext).enableAndBind();
        gl::drawSolidRect(Rectf(size,0,size * 2,size));
        mFboPosition.getTexture(indexColNext).disable();
        gl::drawString("FBO COLOR", Vec2f(paddingTL,paddingTL));
        
        gl::translate(0, size);
        mFboPosition.getTexture(indexSizePrev).enableAndBind();
        gl::drawSolidRect(Rectf(0,0,size,size));
        mFboPosition.getTexture(indexSizePrev).disable();
        mFboPosition.getTexture(indexSizeNext).enableAndBind();
        gl::drawSolidRect(Rectf(size,0,size * 2,size));
        mFboPosition.getTexture(indexSizeNext).disable();
        gl::drawString("FBO SIZE", Vec2f(paddingTL,paddingTL));
        
        gl::popMatrices();
        
        
        gl::disableAlphaBlending();
        
    }
    
    mParams->draw();
    
    //Swap color attachment index
    mAttachmentIndex = !mAttachmentIndex;
    mTimeLast = mTime;
}

/*----------------------------------------------------------------------------------------------------*/

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

CINDER_APP_NATIVE( GPGPUTest03App, RendererGl )
