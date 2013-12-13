#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "ConcurrentQueue.h"
#include "CinderVideoStreamServer.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>


using namespace ci;
using namespace ci::app;
using namespace std;

typedef CinderVideoStreamServer<uint8_t> CinderVideoStreamServerUint8;
static const int WIDTH(1280),
                 HEIGHT(720);

class SceneStreamServerApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
    void shutdown();
	void update();
	void draw();
private:
    void threadLoop();
    bool running;
    std::string mStatus;
    std::shared_ptr<std::thread> mServerThreadRef;
    ph::ConcurrentQueue<uint8_t*>* queueToServer;
    
    int mCurrFrame;
    int mLastFrame;
    
};

void SceneStreamServerApp::threadLoop()
{
    while (running) {
        try {
            boost::shared_ptr<CinderVideoStreamServerUint8> server = boost::shared_ptr<CinderVideoStreamServerUint8>(new CinderVideoStreamServerUint8(3333,queueToServer, WIDTH, HEIGHT));
            server.get()->run();
        }
        catch (std::exception& e) {
            std::cerr << "Exception: " << e.what() << "\n";
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }
}


void SceneStreamServerApp::setup()
{
    
    queueToServer = new ph::ConcurrentQueue<uint8_t*>();
    mServerThreadRef = std::shared_ptr<std::thread>(new std::thread(boost::bind(&SceneStreamServerApp::threadLoop, this)));
    if (!running) running = true;
    
    
}

void SceneStreamServerApp::mouseDown( MouseEvent event )
{
}

void SceneStreamServerApp::update()
{
}

void SceneStreamServerApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::drawColorCube(ci::Vec3f::zero(), ci::Vec3f(0.25f,0.25f,0.25f));
}

void SceneStreamServerApp::shutdown(){
    running = false;
    
    if (queueToServer) delete queueToServer;
}

CINDER_APP_NATIVE( SceneStreamServerApp, RendererGl )
