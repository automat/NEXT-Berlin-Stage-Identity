#include "cinder/app/AppNative.h"
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
static const int FBO_DIM(256);
static const float FBO_ITEMS_NUM(FBO_DIM * FBO_DIM);


static const int   APP_WINDOW_WIDTH(1024),APP_WINDOW_HEIGHT(512);
static const float APP_FPS(30.0f);

class GPGPUTest00App : public AppNative {
  public:
    void prepareSettings(Settings* settings);
    void resize();
	void setup();
	void update();
	void draw();
    
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
    
    gl::Texture  mTextureIndex;
    
    gl::VboMesh mVboMesh; //data representation
    
    gl::Fbo::Format mRGBA16Format;
    
    int     mFboDim;
    gl::Fbo mFboPosition;
    gl::Fbo mFboVelocity;
    bool    mAttachmentIndex; //!mAttachmentIndex // (index + 1) % 2
    
    
    void initShaders();
    void loadShader(gl::GlslProg& prog, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL);
    void initFbos();
    void initVbo();
    
    void setupFboPositionInitial();
    void setupFboVelocityInitial();
    void applyInitialTexToFbo(Surface32f& surface, gl::Fbo& fbo);
    
    void updatePositionFbo();
    void updateVelocityFbo();
    void retrieveFboData();
    
    /*----------------------------------------------------------------------------------------------------*/
};

void GPGPUTest00App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void GPGPUTest00App::resize(){
    mCamera.setAspectRatio(app::getWindowAspectRatio());
}

void GPGPUTest00App::setup(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(), 0.0001f, 5.0f);
    mCameraEye.set(2, 2, 2);
    mCameraTarget.set(0,0,0);
    mCamera.lookAt(mCameraEye,mCameraTarget);
    
    this->initShaders();
    this->initFbos();
    
    this->initVbo();
}

/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest00App::initShaders(){
    this->loadShader(mOutShader, loadResource("out_vert.glsl"),      loadResource("out_frag.glsl"));
    this->loadShader(mPosShader, loadResource("passthru_vert.glsl"), loadResource("pos_frag.glsl"));
    this->loadShader(mVelShader, loadResource("passthru_vert.glsl"), loadResource("vel_frag.glsl"));
}

void GPGPUTest00App::loadShader(gl::GlslProg &prog, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL){
    try{
        prog = gl::GlslProg(refVertGLSL,refFragGLSL);
    } catch (gl::GlslProgCompileExc &exc){
        std::cout << exc.what() << std::endl;
    } catch (...){
        throw "Can't load shader.";
    }
}



/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest00App::initFbos(){
    mRGBA16Format.enableColorBuffer(true,2);
    mRGBA16Format.setMinFilter(GL_NEAREST);
    mRGBA16Format.setMagFilter(GL_NEAREST);
    mRGBA16Format.setColorInternalFormat(GL_RGBA16F_ARB);
    
    mAttachmentIndex = 0;
    
    mFboDim  = FBO_DIM;
    
    
    mFboPosition = gl::Fbo(mFboDim,mFboDim,mRGBA16Format);
    mFboVelocity = gl::Fbo(mFboDim,mFboDim,mRGBA16Format);
    mTextureIndex = gl::Texture(mFboDim,mFboDim);
    mTextureIndex.setWrap(GL_REPEAT, GL_REPEAT);
    mTextureIndex.setMinFilter(GL_NEAREST);
    mTextureIndex.setMagFilter(GL_NEAREST);
    
    
    this->setupFboPositionInitial();
    this->setupFboVelocityInitial();
}

void GPGPUTest00App::setupFboPositionInitial(){
    gl::Texture& indices = mTextureIndex;
    gl::Fbo& fbo = mFboPosition;
    
    //setup fbo pos data
    Surface32f surface(fbo.getTexture()); //retrieve dims
    Surface32f::Iter itr = surface.getIter();
    int width    = fbo.getWidth() ;
    int height   = fbo.getHeight();
    int width_1  = width - 1;
    int height_1 = height - 1;
    int len      = width * height;
    int y,x;
    int index;
    float xNormalized, yNormalized;
    float indexNormalized;
    y = -1;
    while (itr.line()) { // vertical
        y++;
        x = -1;
        while (itr.pixel()) { //horizontal
            x++;
            index = y * width + x;
            xNormalized     = (float)x / (float)width_1;
            yNormalized     = (float)y / (float)height_1;
            indexNormalized = (float)index/(float)len;
            
            
            
            /*
            itr.r() = cosf(indexNormalized * (float)M_PI * 2) * 0.5f;//0.5f + Rand::randFloat() * 0.5f; //x
            itr.g() = -0.5f + Rand::randFloat(); //y
            itr.b() = sinf(indexNormalized * (float)M_PI * 2) * 0.5f;//0.5f + Rand::randFloat() * 0.5f; //y2d
             */

            float u = xNormalized * (float)M_PI * 2;
            float v = yNormalized * (float)M_PI;
            float r = 0.25f - Rand::randFloat() * 0.0125f;
            itr.r() = r * sinf(v) * cosf(u);
            itr.g() = r * sinf(v) * sinf(u);
            itr.b() = r * cosf(v);
            itr.a() = 1.0f; //unused
        }
    }
    this->applyInitialTexToFbo(surface, fbo);
    
    
    //setup tex index data;
    itr = surface.getIter();
    y = -1;
    while (itr.line()) {
        y++;
        x = -1;
        while (itr.pixel()) {
            x++;
            index = y * width + x;
            itr.r() = index;
            itr.g() = 0.0f;
            itr.b() = 0.0f;
            itr.a() = 1.0f;
        }
    }
    
    indices.update(surface);
    
}

void GPGPUTest00App::setupFboVelocityInitial(){
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

void GPGPUTest00App::applyInitialTexToFbo(Surface32f &surface, gl::Fbo &fbo){
    gl::Texture texture(surface);
    texture.setWrap(GL_REPEAT, GL_REPEAT);
    texture.setMinFilter(GL_NEAREST);
    texture.setMagFilter(GL_NEAREST);
    
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


void GPGPUTest00App::initVbo(){
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


void GPGPUTest00App::updatePositionFbo(){
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
    mPosShader.uniform("uIndexMap", 1);
    mPosShader.uniform("uTime", mTime);
    mPosShader.uniform("uNum", FBO_ITEMS_NUM);

    gl::drawSolidRect(mFboPosition.getBounds());
    mPosShader.unbind();
    mFboPosition.unbindFramebuffer();
}

void GPGPUTest00App::updateVelocityFbo(){
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
void GPGPUTest00App::retrieveFboData(){
    //bind color_attachment last written to
    mFboPosition.bindTexture(0,mAttachmentIndex);
    mOutShader.bind();
    mOutShader.uniform("uPositionMap", 0);
    mOutShader.uniform("uTime", mTime);
    gl::draw(mVboMesh);
    mOutShader.unbind();
}


/*----------------------------------------------------------------------------------------------------*/

void GPGPUTest00App::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeElapsed = mTime - mTimeStart;
    mTimeDelta   = mTime - mTimeLast;
    mTimeDelta   = std::max(0.0f, std::min(mTimeDelta, 1.0f));
    
    mCameraEye.set(sinf(mTime)*(float)M_2_PI, 1.0f, 1.0f);
    mCamera.lookAt(mCameraEye, mCameraTarget);
    
   
    
    this->updateVelocityFbo();
    this->updatePositionFbo();
    
}

void GPGPUTest00App::draw(){
    
    
    //Draw
    gl::clear(Color::black());
    gl::setViewport(app::getWindowBounds());
    gl::setMatrices(mCamera);
    
    
    glColor3f(1, 1, 1);
    gl::drawStrokedCube(Vec3f::zero(), Vec3f(1,1,1));
    
    
    
    //gl::enableAlphaBlending();
    gl::enable( GL_TEXTURE_2D );
    gl::enableDepthRead();
    //gl::enableDepthWrite();
    
   
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

CINDER_APP_NATIVE( GPGPUTest00App, RendererGl )
