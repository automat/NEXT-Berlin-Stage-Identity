#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Frustum.h"
#include "FrustumOrtho.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const int APP_WIDTH(1200),APP_HEIGHT(400);
static const int APP_WIDTH_2(APP_WIDTH / 2);
static const float APP_FPS(30);
static const float ASPECT_RATIO(float(APP_WIDTH_2) / float(APP_HEIGHT));

class FrustumTestApp : public AppNative {
private:
  public:
    void prepareSettings(Settings* settins);
	void setup();
	void update();
	void draw();
    void keyDown(KeyEvent event);
    
    GLfloat mVertices[12];
    GLfloat mColors[16];
    
    void drawObjects();
    void checkFrustum();
    
    CameraOrtho   mCameraOrtho;
    CameraOrtho   mCameraDebug;
    ci::Matrix44f mTransform;
    float         mScale;
    
    FrustumOrtho mFrustumOrtho;
};


void FrustumTestApp::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
    settings->setResizable(false);
}

void FrustumTestApp::setup(){
    mCameraOrtho.setOrtho(-ASPECT_RATIO, ASPECT_RATIO  , -1, 1, 0.01f, 3);
    mCameraOrtho.lookAt(Vec3f(0,1,0), Vec3f::zero());

    float cameraDebugZoom = 3.0f;
    mCameraDebug.setOrtho(-ASPECT_RATIO * cameraDebugZoom, ASPECT_RATIO * cameraDebugZoom, -cameraDebugZoom, cameraDebugZoom, 10, -10);
    mCameraDebug.lookAt(Vec3f(1,1,1), Vec3f::zero());

    Rectf rect(-0.5, -0.5, 0.5, 0.5);
    mScale = 1.0f;
    
    //
    
    mVertices[ 0] = rect.x1;
    mVertices[ 1] = 0;
    mVertices[ 2] = rect.y1;
    
    mVertices[ 3] = rect.x2;
    mVertices[ 4] = 0;
    mVertices[ 5] = rect.y1;
    
    mVertices[ 6] = rect.x2;
    mVertices[ 7] = 0;
    mVertices[ 8] = rect.y2;
    
    mVertices[ 9] = rect.x1;
    mVertices[10] = 0;
    mVertices[11] = rect.y2;
    
    mColors[ 0] = mColors[ 4] = mColors[ 8] = mColors[12] = 1.0f;
    mColors[ 1] = mColors[ 2] = mColors[ 5] = mColors[ 6] =
    mColors[ 9] = mColors[10] = mColors[13] = mColors[14] = 0.0f;
    mColors[ 3] = mColors[ 7] = mColors[11] = mColors[15] = 1.0f;
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
}

void FrustumTestApp::keyDown(cinder::app::KeyEvent event){
    switch (event.getCode()) {
        case KeyEvent::KEY_LEFT:
            break;
        case KeyEvent::KEY_RIGHT:
            break;
        case KeyEvent::KEY_1:
            mCameraOrtho.lookAt(Vec3f(1,0,0),Vec3f::zero());
            break;
        case KeyEvent::KEY_2:
            mCameraOrtho.lookAt(Vec3f(0,1,0),Vec3f::zero());
            break;
        case KeyEvent::KEY_3:
            mCameraOrtho.lookAt(Vec3f(0,0,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_4:
            mCameraOrtho.lookAt(Vec3f(1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_5:
            mCameraDebug.lookAt(Vec3f(-1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_6:
            mCameraDebug.lookAt(Vec3f(1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_7:
            mCameraDebug.lookAt(Vec3f(-1,1,-1),Vec3f::zero());
            break;
        case KeyEvent::KEY_8:
            mCameraDebug.lookAt(Vec3f(0,1,0),Vec3f::zero());
            break;
        default:
            break;
    }
}


void FrustumTestApp::update(){
    float time = app::getElapsedSeconds();
    //float cameraZoom = 0.25f + (0.5f + sinf(time) * 0.5f) * 1.75f;
  
    mScale     = 1.0f + (0.5f + sinf(time) * 0.5f) * 1.5f;
    mTransform = mTransform.identity();
    mTransform.scale(Vec3f(mScale,mScale,mScale));
    mTransform.rotate(Vec3f(sinf(time)*(float)M_PI,0,0));
    
    
    mFrustumOrtho.set(mCameraOrtho);
}

void FrustumTestApp::drawObjects(){
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    glLineWidth(3);
    glPointSize(10.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, 0, &mColors[0]);
    glVertexPointer(3, GL_FLOAT, 0, &mVertices[0]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_POINTS, 0, 4);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPointSize(1.0f);
    glLineWidth(1);
    glPopAttrib();
    glPopMatrix();
}

void setColor(float r, float g, float b, float a, int index, GLfloat* color){
    index *= 4;
    color[index  ] = r;
    color[index+1] = g;
    color[index+2] = b;
    color[index+3] = a;
}


void FrustumTestApp::checkFrustum(){
    static Vec3f temp;
    
    temp.set(mVertices[0], mVertices[1], mVertices[2]);
    temp = mTransform.transformPoint(temp);

    if(mFrustumOrtho.contains(temp)){
        setColor(1, 1, 1, 1, 0, mColors);
    } else {
        setColor(1, 0, 0, 1, 0, mColors);
    }
    
    //
    
    temp.set(mVertices[3], mVertices[4], mVertices[5]);
    temp = mTransform.transformPoint(temp);

    if(mFrustumOrtho.contains(temp)){
        setColor(1, 1, 1, 1, 1, mColors);
    } else {
        setColor(1, 0, 0, 1, 1, mColors);
    }
    
    //
    
    temp.set(mVertices[6], mVertices[7], mVertices[8]);
    temp = mTransform.transformPoint(temp);

    if(mFrustumOrtho.contains(temp)){
        setColor(1, 1, 1, 1, 2, mColors);
    } else {
        setColor(1, 0, 0, 1, 2, mColors);
    }
    
    //
    
    temp.set(mVertices[9], mVertices[10], mVertices[11]);
    temp = mTransform.transformPoint(temp);
    
    if(mFrustumOrtho.contains(temp)){
        setColor(1, 1, 1, 1, 3, mColors);
    } else {
        setColor(1, 0, 0, 1, 3, mColors);
    }
}

void drawPoint(const Vec3f& p, float size = 1.0f){
    glPointSize(size);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &p.x);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPointSize(1.0f);
}

void drawQuad(const Vec3f& p0,const Vec3f& p1,const Vec3f& p2,const Vec3f& p3){
    gl::drawLine(p0, p1);
    gl::drawLine(p1, p2);
    gl::drawLine(p2, p3);
    gl::drawLine(p3, p0);
}



void FrustumTestApp::draw(){
    /*------------------------------------------------------------------------------*/
    //  Render View
    /*------------------------------------------------------------------------------*/
    
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, APP_WIDTH_2,  app::getWindowHeight()),
    glScissor(0, 0, APP_WIDTH_2, app::getWindowHeight());
    gl::clear( Color( 0, 0, 0 ) );
    glColor3f(1,1,1);
    
    gl::setMatrices(mCameraOrtho);
    gl::drawCoordinateFrame(4.0f);

    this->checkFrustum();
    this->drawObjects();
    glPopAttrib();
    
    /*------------------------------------------------------------------------------*/
    //  Debug View
    /*------------------------------------------------------------------------------*/

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(APP_WIDTH_2, 0, APP_WIDTH_2,  app::getWindowHeight()),
    glScissor(APP_WIDTH_2, 0, APP_WIDTH_2, app::getWindowHeight());
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCameraDebug);
    gl::drawCoordinateFrame(4.0f);

    glColor3f(0, 1, 0);
    mFrustumOrtho.draw();
    
    this->drawObjects();
    glPopAttrib();
}

CINDER_APP_NATIVE( FrustumTestApp, RendererGl )
