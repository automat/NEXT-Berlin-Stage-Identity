#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/Ray.h"
#include "cinder/Vector.h"
#include <vector>

#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RayIntersectionApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    ci::CameraPersp mCamera;
    ci::Ray mRay;
    std::vector<ci::Vec3f> mTriangles;
    
    Vec3f mIntersection;
    int   mIntersectionCount;
    float mTime;
    float mTimeLast;
    float mTimeDelta;
    
};

void RayIntersectionApp::setup()
{
    mCamera.setPerspective(45.0f, getWindowAspectRatio(), 0.0001, 100.0);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mIntersectionCount = 0;
    
    
    mTriangles.push_back(ci::Vec3f( 2.0f,0.0f,0.0f));
    mTriangles.push_back(ci::Vec3f( 0.0f,0.0f,0.0f));
    mTriangles.push_back(ci::Vec3f( 0.0f,0.0f,2.0f));
}

void RayIntersectionApp::mouseDown( MouseEvent event )
{
}

void RayIntersectionApp::update()
{
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    float zoom   = 1.25f;// + sinf(mTime) * 1.25f;
    float pi     = (float)M_PI;
    float pizoom = pi * zoom;
    
    mCamera.setEyePoint(Vec3f(cosf(mTime) * pizoom,
                              zoom,
                              sinf(mTime) * pizoom));
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    mRay.setOrigin(Vec3f(cosf(mTime * 2) * pi * 0.25f,
                         1.0f,
                         sinf(mTime * 2) * pi * 0.25f));
    mRay.setDirection(Vec3f(0.0001f,-0.75f,0.0001f));
}


void RayIntersectionApp::draw()
{
    
    gl::enableDepthRead();
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::setMatrices(mCamera);
    glColor3f(1.0f, 0.0f, 0.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() * 2);
    glColor3f(0.0f, 1.0f, 0.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() * 2);
    glColor3f(0.0f, 0.0f, 1.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() * 2);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mTriangles[0].x);
    glDrawArrays(GL_TRIANGLES, 0, mTriangles.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glColor3f(1.0f, 0.0f, 1.0f);
    gl::drawSphere(mRay.getOrigin(), 0.0125f);
    gl::drawVector(mRay.getOrigin(), mRay.getOrigin() + mRay.getDirection());

    float distance;
    
    if (mRay.calcTriangleIntersection(mTriangles[0],
                                      mTriangles[2],
                                      mTriangles[1],
                                      &distance)) {
        mIntersectionCount++;
        cout << mIntersectionCount << endl;
        //glColor3f(1.0f,0.0f,0.0f);
        //gl::drawSphere(mIntersection, 0.25f);
    }
    
    
    mTimeLast = mTime;
}

CINDER_APP_NATIVE( RayIntersectionApp, RendererGl )
