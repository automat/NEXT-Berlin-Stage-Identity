#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/Ray.h"
#include "cinder/Color.h"
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
    std::vector<ci::Vec3f>  mVertices;
    std::vector<ci::Colorf> mColors;
    
    Vec3f                  mIntersection;

    
    int                    mIntersectionTick;
    int                    mIntersectionTickFreq;
    bool                   mIntersectionLastTick;
    
    int                    mIntersectionMemoryCount;
    int                    mIntersectionMemoryActive;
    size_t                 mIntersectionMemoryMax;
    std::vector<ci::Vec3f> mIntersectionMemory;
    std::vector<ci::Colorf>mIntersectionMemoryColor;
    
    float mTime;
    float mTimeLast;
    float mTimeDelta;
    
};

void RayIntersectionApp::setup()
{
    mCamera.setPerspective(45.0f, getWindowAspectRatio(), 0.0001, 100.0);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    
    mIntersectionTick         = 0;
    mIntersectionTickFreq     = 3;
    mIntersectionMemoryActive = 0;
    mIntersectionMemoryMax    = 300;
    mIntersectionMemoryCount  = -1;
    mIntersectionLastTick     = false;
    
    int i = -1;
    while (++i < mIntersectionMemoryMax) {
        mIntersectionMemory.push_back(Vec3f::zero());
        mIntersectionMemoryColor.push_back(Colorf());
    }
    
    
    mVertices.push_back(ci::Vec3f( 2.0f,  0.0f, 0.0f));
    mVertices.push_back(ci::Vec3f( 0.0f,  0.0f, 0.0f));
    mVertices.push_back(ci::Vec3f( 0.0f,  0.0f, 2.0f));
    
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    
    mVertices.push_back(ci::Vec3f( 2.0f,  0.0f, 0.0f));
    mVertices.push_back(ci::Vec3f( 0.0f,  0.0f, 0.0f));
    mVertices.push_back(ci::Vec3f( 0.0f,  1.0f,-2.0f));
    
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    
    mVertices.push_back(ci::Vec3f(-2.0f,  0.0f, 0.0f));
    mVertices.push_back(ci::Vec3f( 0.0f,  0.0f, 0.0f));
    mVertices.push_back(ci::Vec3f( 0.0f,  0.0f, 2.0f));
    
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    
    mVertices.push_back(ci::Vec3f(-2.0f, 0.0f, 0.0f));
    mVertices.push_back(ci::Vec3f( 0.0f, 0.0f, 0.0f));
    mVertices.push_back(ci::Vec3f( 0.0f, 1.0f,-2.0f));
    
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    mColors.push_back(ci::Colorf(1.0f,1.0f,1.0f));
    
    
}

void RayIntersectionApp::mouseDown( MouseEvent event )
{
}

void RayIntersectionApp::update()
{
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mIntersectionTick++;
    
    float zoom   = 1.25f;// + sinf(mTime) * 1.25f;
    float pi     = (float)M_PI;
    float pizoom = pi * zoom;
    
    mCamera.setEyePoint(Vec3f(cosf(mTime) * pizoom,
                              zoom,
                              sinf(mTime) * pizoom));
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    float radius = 0.25f * sinf(mTime);
    
    mRay.setOrigin(Vec3f(cosf(mTime * 2) * pi * radius,
                         1.0f,
                         sinf(mTime * 2) * pi * radius));
    mRay.setDirection(Vec3f(cosf(mTime * 2) * pi * 0.25f,
                            -1.0f,
                            sinf(mTime * 2) * pi * 0.25f));
}


void RayIntersectionApp::draw()
{
    
    gl::enableDepthRead();
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::setMatrices(mCamera);
    
    glPushMatrix();
    glTranslatef(0.0f, -0.005, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() * 0.25f,0.05f,0.025f );
    glColor3f(0.0f, 1.0f, 0.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() * 0.25f,0.05f,0.025f);
    glColor3f(0.0f, 0.0f, 1.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() * 0.25f,0.05f,0.025f);
    glPopMatrix();
    
    glColor3f(0.15f,0.15f,0.15f);
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mVertices[0].x);
    //glColorPointer(3, GL_FLOAT, 0, &mColors[0].r);
    glDrawArrays(GL_LINE_LOOP, 0, mVertices.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    

    float distance = 1.0f;
    
    if (mIntersectionTick % mIntersectionTickFreq == 0) {
        int i = 0;
        bool intersects = false;
        
        while (i < mVertices.size()) {
            if (mRay.calcTriangleIntersection(mVertices[i  ],
                                              mVertices[i+1],
                                              mVertices[i+2],
                                              &distance)) {
            
                mIntersectionMemoryCount++;
                if (mIntersectionMemoryCount < mIntersectionMemory.size()) {
                    
                    Vec3f intersection = mRay.calcPosition(distance);
                    
                    mIntersectionMemory[mIntersectionMemoryCount].set(intersection);
                    mIntersectionMemoryActive = mIntersectionMemoryCount;
                    
                    glColor3f(1.0f, 0.0f, 0.0f);
                    gl::drawSphere(intersection, 0.025f);
                }
                
                intersects = true;
                break;
            }
            i+=3;
        }
        
        
        
        mIntersectionLastTick = intersects;
        if(!mIntersectionLastTick)mIntersectionMemoryCount = -1;
    }
    
    if(mIntersectionLastTick){
     
        int i = -1;
        float n;
        while (++i < mIntersectionMemoryActive) {
            n = (float)i / (float)mIntersectionMemoryActive;
            mIntersectionMemoryColor[i].r = n;
            mIntersectionMemoryColor[i].g = n * 0.125f;
            mIntersectionMemoryColor[i].b = n * 0.25f;
        }
        
        glLineWidth(4.0f);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mIntersectionMemory[0].x);
        glColorPointer(3, GL_FLOAT, 0, &mIntersectionMemoryColor[0].r);
        glPushMatrix();
       
        glDrawArrays(GL_LINE_STRIP, 0, mIntersectionMemoryActive);
        
        glPopMatrix();
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glLineWidth(1.0f);
    }
    
    
    glColor3f(0.525f, 0.0f, 0.0f);
    gl::drawSphere(mRay.getOrigin(), 0.0125f);
    gl::drawVector(mRay.getOrigin(), mRay.calcPosition(1.0f));
    
    
    
    
    mTimeLast = mTime;
}

CINDER_APP_NATIVE( RayIntersectionApp, RendererGl )
