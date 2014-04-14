#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Matrix44.h"
#include "SpeakerView.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Utilities.h"



using namespace ci;
using namespace ci::app;
using namespace std;



class SpeakerImageFilterApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
    void keyDown(KeyEvent event);
	void update();
	void draw();

    CameraOrtho mCamera;
    SpeakerView* mSpeakerView;

    Matrix44f mModelViewMatrix;
    Matrix44f mProjectionMatrix;
    float     mWindowWidth;
    float     mWindowHeight;

    Vec3f     mEcPoints[4];     // Eye coord
    Vec3f     mNdcPoints[4];    // Normal device coord
    Vec2f     mSaPoints[4];     // Screen aligned coord

};

void SpeakerImageFilterApp::prepareSettings(Settings *settings) {
    settings->setWindowSize(800, 600);
}

void SpeakerImageFilterApp::setup(){
    float aspectRatio = getWindowAspectRatio();
    float zoom = 1;
    mCamera.setOrtho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom, -1, 10);
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    mSpeakerView = new SpeakerView();
}

void SpeakerImageFilterApp::keyDown(KeyEvent event) {
    switch(event.getCode()){
        case KeyEvent::KEY_ESCAPE:
            this->quit();
        default:
            break;
    }
}

void SpeakerImageFilterApp::update(){
    mSpeakerView->update();

    mWindowWidth  = app::getWindowWidth();
    mWindowHeight = app::getWindowHeight();
}

void SpeakerImageFilterApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);
    gl::drawCoordinateFrame();

    float t = static_cast<float>(app::getElapsedSeconds());

    glPushMatrix();
    glTranslatef(cos(t) * 0.5f,sin(t * 0.125f) * 0.5f,sin(t) * 0.5f);
    mSpeakerView->draw();

    glGetFloatv(GL_MODELVIEW_MATRIX,  &mModelViewMatrix[0]);
    glGetFloatv(GL_PROJECTION_MATRIX, &mProjectionMatrix[0]);

    // get coords transformed by modeview transformation matrix
    mEcPoints[0] = mModelViewMatrix * mSpeakerView->getTL();
    mEcPoints[1] = mModelViewMatrix * mSpeakerView->getTR();
    mEcPoints[2] = mModelViewMatrix * mSpeakerView->getBL();
    mEcPoints[3] = mModelViewMatrix * mSpeakerView->getBR();

    // get normalized device coordinates
    mNdcPoints[0] = (mProjectionMatrix * Vec4f(mEcPoints[0],1.0f)).xyz();
    mNdcPoints[1] = (mProjectionMatrix * Vec4f(mEcPoints[1],1.0f)).xyz();
    mNdcPoints[2] = (mProjectionMatrix * Vec4f(mEcPoints[2],1.0f)).xyz();
    mNdcPoints[3] = (mProjectionMatrix * Vec4f(mEcPoints[3],1.0f)).xyz();

    // project to screen coords
    mSaPoints[0] = Vec2f( ( mNdcPoints[0].x + 1.0f ) / 2.0f * mWindowWidth, ( 1.0f - ( mNdcPoints[0].y + 1.0f ) / 2.0f ) * mWindowHeight );
    mSaPoints[1] = Vec2f( ( mNdcPoints[1].x + 1.0f ) / 2.0f * mWindowWidth, ( 1.0f - ( mNdcPoints[1].y + 1.0f ) / 2.0f ) * mWindowHeight );
    mSaPoints[2] = Vec2f( ( mNdcPoints[2].x + 1.0f ) / 2.0f * mWindowWidth, ( 1.0f - ( mNdcPoints[2].y + 1.0f ) / 2.0f ) * mWindowHeight );
    mSaPoints[3] = Vec2f( ( mNdcPoints[3].x + 1.0f ) / 2.0f * mWindowWidth, ( 1.0f - ( mNdcPoints[3].y + 1.0f ) / 2.0f ) * mWindowHeight );

    static gl::TextureFontRef debugTexFontRef = gl::TextureFont::create(Font("Apercu Mono",16));

    // Draw the shite
    gl::pushMatrices();
    gl::setMatricesWindow(app::getWindowSize(),true);
    glDisable(GL_DEPTH_TEST);
    glColor3f(1,0,0);
    glPointSize(15);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &mSaPoints[0]);
    glDrawArrays(GL_POINTS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    float left   = mSaPoints[2].x;
    float right  = mSaPoints[1].x;
    float top    = mSaPoints[0].y;
    float bottom = mSaPoints[3].y;

    float width  = right - left;
    float height = bottom - height;
    float midH   = left + width * 0.5f;
    float midV   = top + height * 0.5f;

    Vec2f midT = Vec2f(midH, top);
    Vec2f midB = Vec2f(midH, bottom);
    Vec2f midL = Vec2f(left, midV);
    Vec2f mitR = Vec2f(right, midV);

    float width_2  = width * 0.5f;
    float height_2 = height * 0.5f;

    Vec2f texcoords[4];
    texcoords[0] = Vec2f(0,width_2);


    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    glColor3f(0,0,1);
    glPushMatrix();
    glTranslatef(0, 30, 0);
    debugTexFontRef->drawString("TL " + toString(mSaPoints[0]),mSaPoints[0]);
    debugTexFontRef->drawString("TR " + toString(mSaPoints[1]),mSaPoints[1]);
    debugTexFontRef->drawString("BL " + toString(mSaPoints[2]),mSaPoints[2]);
    debugTexFontRef->drawString("BR " + toString(mSaPoints[3]),mSaPoints[3]);
    glPopMatrix();
    debugTexFontRef->drawString("TL" + toString(texcoords[0]),mSaPoints[0]);
    gl::disableAlphaBlending();
    gl::disableAlphaTest();

    glLineWidth(3);
    glColor3f(1,0,0);
    gl::drawLine(Vec2f(left,0), Vec2f(left,mWindowHeight));
    gl::drawLine(Vec2f(right,0), Vec2f(right,mWindowHeight));
    gl::drawLine(Vec2f(0,top), Vec2f(mWindowWidth,top));
    gl::drawLine(Vec2f(0,bottom), Vec2f(mWindowWidth,bottom));
    glLineWidth(1);




    gl::popMatrices();
    glEnable(GL_DEPTH_TEST);
}

CINDER_APP_NATIVE( SpeakerImageFilterApp, RendererGl )
