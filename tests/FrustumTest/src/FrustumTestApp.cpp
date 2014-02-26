#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Frustum.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const int APP_WIDTH(1200),APP_HEIGHT(400);
static const int APP_WIDTH_2(APP_WIDTH / 2);
static const float APP_FPS(30);
static const float ASPECT_RATIO(float(APP_WIDTH_2) / float(APP_HEIGHT));

class FrustumTestApp : public AppNative {
private:
    enum { TOP, BOT, LEF, RIG, NEA, FARP };
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
    
    Vec3f mFrustumPlaneNormals[6];
	Vec3f mFrustumPlanePoints[6];
	float mFrustumPlaneDists[6];
    void calcFrustumPlane( Vec3f* fNormal, Vec3f* fPoint, float* fDist, const Vec3f &v1, const Vec3f &v2, const Vec3f &v3 );
	void calcNearAndFarClipCoordinates( const Camera &cam );
    bool isPointInFrustum(const Vec3f& loc);
    
    CameraOrtho   mCameraOrtho;
    CameraPersp   mCameraPersp;
    Camera*       mCameraCurrent;
    CameraOrtho   mCameraDebug;
    ci::Frustumf  mCameraFrustum;
    ci::Matrix44f mTransform;
    float         mScale;
    
    Rectf mRect;
};

void FrustumTestApp::calcFrustumPlane(Vec3f* fNormal, Vec3f* fPoint, float* fDist, const Vec3f &v1, const Vec3f &v2, const Vec3f &v3 )
{
	Vec3f aux1, aux2;
    
	aux1 = v1 - v2;
	aux2 = v3 - v2;
    
	*fNormal = aux2.cross( aux1 ).normalized();
	*fPoint  = v2;
	*fDist = -( fNormal->dot( *fPoint ) );
}

void FrustumTestApp::calcNearAndFarClipCoordinates( const Camera &cam )
{
	Vec3f ntl, ntr, nbl, nbr;
	cam.getNearClipCoordinates( &ntl, &ntr, &nbl, &nbr );
    
	Vec3f ftl, ftr, fbl, fbr;
	cam.getFarClipCoordinates( &ftl, &ftr, &fbl, &fbr );
    
    cout << "Near" << endl;
    cout << ntl << ", " << ntr << ", " << nbl << ", " << nbr << endl;
    cout << "Far" << endl;
    cout << ftl << ", " << ftr << ", " << fbl << ", " << fbr << endl;
    

	
	//if( ! mFrustumPlaneCached ){
		calcFrustumPlane( &mFrustumPlaneNormals[TOP],  &mFrustumPlanePoints[TOP],  &mFrustumPlaneDists[TOP], ntr, ntl, ftl );
		calcFrustumPlane( &mFrustumPlaneNormals[BOT],  &mFrustumPlanePoints[BOT],  &mFrustumPlaneDists[BOT], nbl, nbr, fbr );
		calcFrustumPlane( &mFrustumPlaneNormals[LEF],  &mFrustumPlanePoints[LEF],  &mFrustumPlaneDists[LEF], ntl, nbl, fbl );
		calcFrustumPlane( &mFrustumPlaneNormals[RIG],  &mFrustumPlanePoints[RIG],  &mFrustumPlaneDists[RIG], ftr, fbr, nbr );
		calcFrustumPlane( &mFrustumPlaneNormals[NEA],  &mFrustumPlanePoints[NEA],  &mFrustumPlaneDists[NEA], ntl, ntr, nbr );
		calcFrustumPlane( &mFrustumPlaneNormals[FARP], &mFrustumPlanePoints[FARP], &mFrustumPlaneDists[FARP], ftr, ftl, fbl );
	//}
	
	//mFrustumPlaneCached = true;
}

bool FrustumTestApp::isPointInFrustum( const Vec3f &loc )
{
	float d;
	bool result = true;
    
	for( int i=0; i<6; i++ ){
		d = mFrustumPlaneDists[i] + mFrustumPlaneNormals[i].dot( loc );
		if( d < 0 )
			return false;
	}
    
	return result;
}

void FrustumTestApp::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
    settings->setResizable(false);
}

void FrustumTestApp::setup(){

    mCameraOrtho.setOrtho(-ASPECT_RATIO, ASPECT_RATIO  , -1, 1, 0.01f, 5);
    mCameraOrtho.lookAt(Vec3f(1,1,1), Vec3f::zero());
    
    
    mCameraPersp.setPerspective(45.0f, ASPECT_RATIO, 0.00001f, 10.0f);
    mCameraPersp.lookAt(Vec3f(1,1,1), Vec3f::zero());
    
    mCameraCurrent = &mCameraOrtho;
    
    calcNearAndFarClipCoordinates( *mCameraCurrent );

   
    float cameraDebugZoom = 1.0f;
    mCameraDebug.setOrtho(-ASPECT_RATIO * cameraDebugZoom, ASPECT_RATIO * cameraDebugZoom, -cameraDebugZoom, cameraDebugZoom, -10, 10);
    mCameraDebug.lookAt(Vec3f(0,1,0), Vec3f::zero());

    mRect.set(-0.5, -0.5, 0.5, 0.5);
    //
    
    mVertices[ 0] = mRect.x1;
    mVertices[ 1] = 0;
    mVertices[ 2] = mRect.y1;
    
    mVertices[ 3] = mRect.x2;
    mVertices[ 4] = 0;
    mVertices[ 5] = mRect.y1;
    
    mVertices[ 6] = mRect.x2;
    mVertices[ 7] = 0;
    mVertices[ 8] = mRect.y2;
    
    mVertices[ 9] = mRect.x1;
    mVertices[10] = 0;
    mVertices[11] = mRect.y2;
    
    mColors[ 0] = mColors[ 4] = mColors[ 8] = mColors[12] = 1.0f;
    mColors[ 1] = mColors[ 2] = mColors[ 5] = mColors[ 6] =
    mColors[ 9] = mColors[10] = mColors[13] = mColors[14] = 0.0f;
    mColors[ 3] = mColors[ 7] = mColors[11] = mColors[15] = 1.0f;
    
    mScale = 1.0f;
    
 
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
}

void FrustumTestApp::keyDown(cinder::app::KeyEvent event){
    switch (event.getCode()) {
        case KeyEvent::KEY_LEFT:
            break;
        case KeyEvent::KEY_RIGHT:
            break;
        case KeyEvent::KEY_0:
            mCameraCurrent = &mCameraOrtho;
            break;
        case KeyEvent::KEY_1:
            mCameraCurrent = &mCameraPersp;
            break;
        default:
            break;
    }
}


void FrustumTestApp::update(){
    float time = app::getElapsedSeconds();
    float cameraZoom = 0.25f + (0.5f + sinf(time) * 0.5f) * 1.75f;
    
    //mCamera.lookAt(Vec3f(sinf(time) * cameraZoom,cameraZoom,cosf(time)* cameraZoom), Vec3f::zero());
    
    mScale = 1.0f + (0.5f + sinf(time) * 0.5f) * 1.5f;
    mTransform = mTransform.identity();
    mTransform.scale(Vec3f(mScale,mScale,mScale));
    //mTransform.rotate(Vec3f(sinf(time)*(float)M_PI,0,0));
    
    calcNearAndFarClipCoordinates(*mCameraCurrent);
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
    if (isPointInFrustum(temp)) {
        setColor(1, 1, 1, 1, 0, mColors);
    } else {
        setColor(1, 0, 0, 1, 0, mColors);
    }
    
    temp.set(mVertices[3], mVertices[4], mVertices[5]);
    temp = mTransform.transformPoint(temp);
    if (isPointInFrustum(temp)) {
        setColor(1, 1, 1, 1, 1, mColors);
    } else {
        setColor(1, 0, 0, 1, 1, mColors);
    }
    temp.set(mVertices[6], mVertices[7], mVertices[8]);
    temp = mTransform.transformPoint(temp);
    if (isPointInFrustum(temp)) {
        setColor(1, 1, 1, 1, 2, mColors);
    } else {
        setColor(1, 0, 0, 1, 2, mColors);
    }
    temp.set(mVertices[9], mVertices[10], mVertices[11]);
    temp = mTransform.transformPoint(temp);
    if (isPointInFrustum(temp)) {
        setColor(1, 1, 1, 1, 3, mColors);
    } else {
        setColor(1, 0, 0, 1, 3, mColors);
    }
}

void FrustumTestApp::draw(){
    /*------------------------------------------------------------------------------*/
    //  Render View
    /*------------------------------------------------------------------------------*/
    
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, APP_WIDTH_2,  app::getWindowHeight()),
    glScissor(0, 0, APP_WIDTH_2, app::getWindowHeight());

    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(*mCameraCurrent);
    
    
    


   
    /*
    Matrix44f matrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
    cout << matrix << endl;
    */
    glPushMatrix();
    this->checkFrustum();
    this->drawObjects();
    glPopMatrix();
    glPopAttrib();
    
    /*------------------------------------------------------------------------------*/
    //  Debug View
    /*------------------------------------------------------------------------------*/

    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(APP_WIDTH_2, 0, APP_WIDTH_2,  app::getWindowHeight()),
    glScissor(APP_WIDTH_2, 0, APP_WIDTH_2, app::getWindowHeight());
	
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCameraDebug);

    glColor3f(0, 1, 0);
    gl::drawFrustum(*mCameraCurrent);

    glPushMatrix();
    this->drawObjects();
    glPopMatrix();
    
    glPopAttrib();
}

CINDER_APP_NATIVE( FrustumTestApp, RendererGl )
