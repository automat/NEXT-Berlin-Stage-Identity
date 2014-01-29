#include "cinder/app/AppNative.h"
#include "FileWatcher/FileWatcher.h"

#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/Rand.h"

#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

static bool DEBUG_DRAW_FBO(true);
static const int FBO_DIM(512);
static const float FBO_ITEMS_NUM(FBO_DIM * FBO_DIM);

static const string FILE_TO_WATCH("test.glsl");
static const string DIR_TO_WATCH("/Users/automat/Projects/next/tests/GPGPUTest01/xcode");

static const int   APP_WINDOW_WIDTH(1024),APP_WINDOW_HEIGHT(512);
static const float APP_FPS(30.0f);

class GPGPUTest01App : public AppNative, public FW::FileWatchListener {
public:
    void prepareSettings(Settings* settings);
    void resize();
	void setup();
	void update();
	void draw();
    
    /*----------------------------------------------------------------------------------------------------*/
    
    FW::FileWatcher*     mFileWatcher;
    FW::WatchID          mWatchId;
    void initFileWatcher();
    void handleFileAction(FW::WatchID watchId, const string &dir, const string &filename, FW::Action action);
    bool mUpdatingShader;
    
    /*----------------------------------------------------------------------------------------------------*/
    
    CameraPersp mCamera;
    Vec3f       mCameraEye;
    Vec3f       mCameraTarget;
    
    float mTime;
    float mTimeLast;
    float mTimeDelta;
    float mTimeStart;
    float mTimeElapsed;
    
    /*----------------------------------------------------------------------------------------------------*/
    
    gl::GlslProg mOutShader; //retrieve data from gpu
    gl::GlslProg mPosShader; //update postion
    gl::GlslProg mVelShader; //update velocity
    
    gl::Texture  mTextureIndex; //texture containing indices
    
    gl::VboMesh mVboMesh; //data geometry representation
    
    gl::Fbo mFboPosition; //data position
    gl::Fbo mFboVelocity; //data velocity
    bool    mAttachmentIndex; //!mAttachmentIndex // (index + 1) % 2 //switch by !bool -> int
    
    void initShaders();
    void loadShader(gl::GlslProg& prog, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL);
    void initData();
    void initVbo();
    
    void setupFboPositionInitial();
    void setupFboVelocityInitial();
    void applyInitialTexToFbo(Surface32f& surface, gl::Fbo& fbo);
    
    void updatePositionFbo();
    void updateVelocityFbo();
    void retrieveFboData();
    
    /*----------------------------------------------------------------------------------------------------*/
};

void GPGPUTest01App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void GPGPUTest01App::resize(){
    mCamera.setAspectRatio(app::getWindowAspectRatio());
}

void GPGPUTest01App::setup(){
    this->initFileWatcher();
    
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(), 0.0001f, 5.0f);
    mCameraEye.set(2, 2, 2);
    mCameraTarget.set(0,0,0);
    mCamera.lookAt(mCameraEye,mCameraTarget);
    
    this->initShaders();
    this->initData();
    this->initVbo();
}

/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest01App::initFileWatcher(){
    mFileWatcher    = new FW::FileWatcher();
    mUpdatingShader = false;
    mWatchId        = mFileWatcher->addWatch(DIR_TO_WATCH,this);
    
}

void GPGPUTest01App::handleFileAction(FW::WatchID watchId, const string &dir, const string &filename, FW::Action action){
    if((DIR_TO_WATCH + "/" + FILE_TO_WATCH) != filename || mUpdatingShader){
        return;
    }
    gl::GlslProg progTry;
    bool success = true;
    
    switch (action) {
        case FW::Actions::Add:
            //unhandled
            break;
            
        case FW::Actions::Delete:
            //unhandled
            break;
            
        case FW::Actions::Modified:
            std::cout << FILE_TO_WATCH + " modified." << std::endl;
            try {
                mUpdatingShader = true;
                progTry = gl::GlslProg(loadResource("passthru_vert.glsl"),loadFile(DIR_TO_WATCH + "/" + FILE_TO_WATCH));
            } catch (gl::GlslProgCompileExc& exc) {
                std::cout << exc.what()<<std::endl;
                success = false;
            } catch (...){
                success = false;
                throw "Can´t load shader.";
            }
            
            if(!success){
                mUpdatingShader = false;
            } else {
                std::cout << "Shader updated." << std::endl;
                mPosShader = progTry; //copy shader
                mUpdatingShader = false;
            }
            break;
            
        default:
            std::cout << "Should never happen!" << std::endl;
            break;
    }
}

/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest01App::initShaders(){
    this->loadShader(mOutShader, loadResource("out_vert.glsl"),      loadResource("out_frag.glsl"));
    this->loadShader(mPosShader, loadResource("passthru_vert.glsl"), loadResource("pos_frag.glsl"));
    this->loadShader(mVelShader, loadResource("passthru_vert.glsl"), loadResource("vel_frag.glsl"));
}

void GPGPUTest01App::loadShader(gl::GlslProg &prog, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL){
    try{
        prog = gl::GlslProg(refVertGLSL,refFragGLSL);
    } catch (gl::GlslProgCompileExc &exc){
        std::cout << exc.what() << std::endl;
    } catch (...){
        throw "Can't load shader.";
    }
}

/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest01App::initData(){
    mAttachmentIndex = 0;
    
    gl::Fbo::Format formatFboRGBA16;
    formatFboRGBA16.enableColorBuffer(true,2);
    formatFboRGBA16.setMinFilter(GL_NEAREST);
    formatFboRGBA16.setMagFilter(GL_NEAREST);
    formatFboRGBA16.setColorInternalFormat(GL_RGBA16F_ARB);
    
    mFboPosition = gl::Fbo(FBO_DIM,FBO_DIM,formatFboRGBA16);
    mFboVelocity = gl::Fbo(FBO_DIM,FBO_DIM,formatFboRGBA16);
    
    gl::Texture::Format formatTex;
    formatTex.setWrap(GL_REPEAT, GL_REPEAT);
    formatTex.setMinFilter(GL_NEAREST);
    formatTex.setMagFilter(GL_NEAREST);
    
    mTextureIndex = gl::Texture(FBO_DIM,FBO_DIM,formatTex);
    
    this->setupFboPositionInitial();
    this->setupFboVelocityInitial();
}

void GPGPUTest01App::setupFboPositionInitial(){
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
            itr.a() = 1.0f; //unused
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

void GPGPUTest01App::setupFboVelocityInitial(){
    gl::Fbo& fbo = mFboVelocity;
    Surface32f surface(fbo.getTexture()); //retrieve dims
    Surface32f::Iter itr = surface.getIter();
    while (itr.line()) { // vertical
        while (itr.pixel()) { //horizontal
            itr.r() = 0.1f; //x
            itr.g() = 0.0f; //y
            itr.b() = 0.0f; //y2d
            itr.a() = 1.0f; //unused
        }
    }
    this->applyInitialTexToFbo(surface, fbo);
}

void GPGPUTest01App::applyInitialTexToFbo(Surface32f &surface, gl::Fbo &fbo){
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


void GPGPUTest01App::initVbo(){
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


void GPGPUTest01App::updatePositionFbo(){
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
    
    mPosShader.uniform("uPositionMap", 0); //send data to uniform
    mPosShader.uniform("uIndexMap", 1); //data index
    mPosShader.uniform("uNumItems", FBO_ITEMS_NUM); //num data
    mPosShader.uniform("uTime", mTime); //time
    
    gl::drawSolidRect(mFboPosition.getBounds());
    mPosShader.unbind();
    mFboPosition.unbindFramebuffer();
}

void GPGPUTest01App::updateVelocityFbo(){
    mFboVelocity.bindFramebuffer();
    //! bind the opposite attachment to draw to
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + !mAttachmentIndex);
    
    //! set window according to fbo size
    gl::setViewport(mFboVelocity.getBounds());
    gl::setMatricesWindow(mFboVelocity.getSize(),false);
    gl::clear(); //clear current color attachement
    
    //! bind current color attachment as previous data
    mFboVelocity.bindTexture(0,mAttachmentIndex);
    mVelShader.bind();
    mVelShader.uniform("uTime", mTime);
    
    gl::drawSolidRect(mFboPosition.getBounds());
    mVelShader.unbind();
    mFboVelocity.unbindFramebuffer();
}

//Transfer data from fbo to vbo
void GPGPUTest01App::retrieveFboData(){
    //bind color_attachment last written to
    mFboPosition.bindTexture(0,mAttachmentIndex);
    mOutShader.bind();
    mOutShader.uniform("uPositionMap", 0);
    mOutShader.uniform("uTime", mTime);
    gl::draw(mVboMesh);
    mOutShader.unbind();
}


/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest01App::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeElapsed = mTime - mTimeStart;
    mTimeDelta   = mTime - mTimeLast;
    mTimeDelta   = std::max(0.0f, std::min(mTimeDelta, 1.0f));
    
    mCameraEye.set(sinf(mTime)*(float)M_2_PI, 1.0f, 1.0f);
    mCamera.lookAt(mCameraEye, mCameraTarget);
    
    this->updateVelocityFbo();
    this->updatePositionFbo();
    
    mFileWatcher->update();
}

void GPGPUTest01App::draw(){
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
        
        mFboVelocity.getTexture().enableAndBind();
        gl::drawSolidRect(Rectf(0,128,128,256));
        mFboVelocity.getTexture().disable();
        gl::drawString("FBO VELOCITY", Vec2f(20,128 + 20));
        
        
        mTextureIndex.enableAndBind();
        gl::drawSolidRect(Rectf(0,256,128,384));
        mTextureIndex.disable();
        gl::drawString("TEX INDEX", Vec2f(20,256 + 20));
        
        gl::disableAlphaBlending();
        
    }
    
    //Swap color attachment index
    mAttachmentIndex = !mAttachmentIndex;
    mTimeLast = mTime;
}

CINDER_APP_NATIVE( GPGPUTest01App, RendererGl )
