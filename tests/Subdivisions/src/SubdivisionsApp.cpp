#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Camera.h"

#include <vector>


using namespace ci;
using namespace ci::app;
using namespace std;


static const ci::Vec2f WINDOW_SIZE(600,480);
static const float FPS(30);

class SubdivisionsApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    
    void subdivideTri(std::vector<ci::Vec3f>& tri, int level = 1);

    ci::CameraPersp mCamera;
    std::vector<ci::Vec3f> mTriangles;
};

void SubdivisionsApp::setup(){
    mCamera.setPerspective(45.0f, ci::app::getWindowAspectRatio(), 0.00001f, 5.0f);
    mCamera.setEyePoint(ci::Vec3f(0,2,0));
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    mTriangles = {ci::Vec3f(-0.5f,0,0.25f),
                  ci::Vec3f(0,0,-0.5f),
                  ci::Vec3f( 0.5f,0,0.25f)};
    
    this->subdivideTri(mTriangles, 4);
}

void SubdivisionsApp::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_SIZE);
    settings->setFrameRate(FPS);
}

void SubdivisionsApp::mouseDown( MouseEvent event ){
}

void SubdivisionsApp::update(){
}

void SubdivisionsApp::draw(){
    
    static const ci::Vec3f zero(0,0,0);
    
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);
    
   // gl::drawStrokedCube(zero, ci::Vec3f(0.25f,0.25f,0.25f));
    
    glColor3f(1, 1, 1);
    int i = 0;
    while (i < mTriangles.size()) {
        gl::drawLine(mTriangles[i],   mTriangles[i+1]);
        gl::drawLine(mTriangles[i+1], mTriangles[i+2]);
        gl::drawLine(mTriangles[i+2], mTriangles[i]);
        
        i+=3;
    }
    
    
}


void SubdivisionsApp::subdivideTri(std::vector<ci::Vec3f>& tri, int level){
    std::cout << level << std::endl;
    if(level == 0)return;
    
    ci::Vec3f p0;
    ci::Vec3f p1;
    ci::Vec3f p2;
    ci::Vec3f center;
    
    
    
    int i = 0;
    while (i < tri.size()) {
        p0 = tri[i+0];
        p1 = tri[i+1];
        p2 = tri[i+2];
        
        center = (p0 + p1 + p2) / 3;
    
        
        tri[i  ] = p0;
        tri[i+1] = p1;
        tri[i+2] = center;
    
        /*
      
        tri.push_back(p1);
        tri.push_back(p2);
        tri.push_back(center);
        
        tri.push_back(p2);
        tri.push_back(center);
        tri.push_back(p0);
        */
        i+=3;
    }
    
    
    
    
    subdivideTri(tri, --level);
}



CINDER_APP_NATIVE( SubdivisionsApp, RendererGl )
