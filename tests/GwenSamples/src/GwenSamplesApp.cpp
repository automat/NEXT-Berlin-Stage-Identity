#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cigwen/GwenRendererGl.h"
#include "cigwen/GwenInput.h"
#include "cigwen/CinderGwen.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/Dark.h"
#include "Gwen/Skins/TexturedBase.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/MenuStrip.h"

#include "ControlPanel.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define SKIN_ASSET_PATH "/Users/automat/Projects/Cinder/blocks/Cinder-Gwen/assets"

class GwenSamplesApp : public AppNative {
  public:
    void prepareSettings(Settings* settings);
	void setup();
	void mouseDown( MouseEvent event );	
	void keyDown( KeyEvent event );
    void update();
	void draw();
    
    void initGwen();
    
    cigwen::GwenRendererGl* mGwenRenderer;
    cigwen::GwenInputRef    mGwenInput;
    Gwen::Controls::Canvas* mGwenCanvas;
    
    ControlPanel*  mControlPanel;
};

void GwenSamplesApp::prepareSettings(Settings* settings){
    settings->setWindowSize(800, 600);
    settings->setFrameRate(32.0f);
}

void GwenSamplesApp::setup(){
    initGwen();
}

void GwenSamplesApp::initGwen(){
    addAssetDirectory(SKIN_ASSET_PATH);
    
    mGwenRenderer = new cigwen::GwenRendererGl();
    mGwenRenderer->Init();
    
    
    
    Gwen::Skin::Dark* skin = new Gwen::Skin::Dark(mGwenRenderer);
    skin->Init("DefaultSkin_Dark.png");
    //skin->Init("DefaultSkin_Dark.png ");
   // skin->Init("DefaultSkin_Dark.png");
   // skin->SetDefaultFont(L"Apercu Mono",10);
    
    
    
    float outerPadding = 0;
    
    mGwenCanvas = new Gwen::Controls::Canvas( skin );
	mGwenCanvas->SetSize( getWindowWidth(), getWindowHeight() );
    
    
    Gwen::Controls::MenuStrip* menu = new MenuStrip(mGwenCanvas);
    menu->Dock(Gwen::Pos::Top);
    menu->AddItem("Close");
    menu->AddItem("View");
    menu->AddItem("Panels");
    
	//mGwenCanvas->SetDrawBackground( true );
	//mGwenCanvas->SetBackgroundColor( cigwen::toGwen( Color::gray( 0.2 ) ) );
    
	mGwenInput = cigwen::GwenInput::create( mGwenCanvas );
    //
    
    
    
    
    mControlPanel = new ControlPanel(mGwenCanvas,Vec2f(20,40));
    
    
}

void GwenSamplesApp::mouseDown( MouseEvent event ){
}

void GwenSamplesApp::keyDown( KeyEvent event ){
    
    switch (event.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
            
        default:
            break;
    }
}

void GwenSamplesApp::update()
{
}

void GwenSamplesApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mGwenCanvas->RenderCanvas();
}

CINDER_APP_NATIVE( GwenSamplesApp, RendererGl )
