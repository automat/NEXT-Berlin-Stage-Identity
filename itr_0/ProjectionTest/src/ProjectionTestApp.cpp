#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Utilities.h"

#include "cinder/gl/TextureFont.h"

#include "cinder/cairo/Cairo.h"

#include "FileWatcher.h"
#include "Config.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const std::string FONT_NAME("Transcript-Bold");

class ProjectionTestApp : public AppNative {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void update();
	void draw();
    void keyDown(KeyEvent event);
    
    gl::Fbo mFbo;
    
    void reloadFbo();
    
#ifdef CONFIGURABLE_SETUP
    FileWatcher mFileWatcher;
#endif
};

void ProjectionTestApp::setup(){
#ifdef CONFIGURABLE_SETUP
    mFileWatcher.addFile(PATH_CONFIG_JSON);
#endif
    reloadFbo();
}

void ProjectionTestApp::prepareSettings(Settings* settings){
#ifdef CONFIGURABLE_SETUP
    config::Load(PATH_CONFIG_JSON);
#else
    config::Load(app::getAppPath() + "/config.json");
#endif
    const std::vector<DisplayRef>& displays = Display::getDisplays();
    if(displays.size() != 1){
        settings->setDisplay(displays[WINDOW_DISPLAY_ID]);
        cout << "Display: " << WINDOW_DISPLAY_ID << " / " << (displays.size() - 1) << endl;
    }
    
    settings->setBorderless(WINDOW_BORDERLESS);
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setWindowPos(WINDOW_DISPLAY_POSITION);
    settings->setResizable(WINDOW_RESIZABLE);
    //settings->setFullScreen();
   settings->setAlwaysOnTop(true);
    
    settings->setFrameRate(APP_FPS);
}

void drawRect(float x, float y, float width, float height){
    static float vertices[8];
    
    float xw = x + width;
    float yh = y + height;
    
    vertices[0] = x;
    vertices[1] = y;
    
    vertices[2] = xw;
    vertices[3] = y;
    
    vertices[4] = x;
    vertices[5] = yh;
    
    vertices[6] = xw;
    vertices[7] = yh;
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void drawString(cairo::Context& ctx, float x, float y, const string& str, double size){
    ctx.setFontSize(size);
    cairo::FontExtents extents = ctx.fontExtents();
    float lineHeight = extents.descent() - extents.ascent() + extents.height();
    
    cout << lineHeight << endl;
    
    ctx.save();
    cairo::Matrix prevMat;
    ctx.getMatrix(&prevMat);
    ctx.translate(x,y+lineHeight * 1.45f);
    ctx.showText(str);
    ctx.setMatrix(prevMat);
    ctx.restore();
}



void ProjectionTestApp::reloadFbo(){
    string transcriptBold = "Transcript-Bold";
    string akkurat        = "Akkurat";
    string akkuratBold    = "Akkurat-Bold";
    
    gl::TextureFontRef fontTranscript80 = gl::TextureFont::create(Font(transcriptBold,80));
    gl::TextureFontRef fontTranscript50 = gl::TextureFont::create(Font(transcriptBold,50));
    gl::TextureFontRef fontTranscript30 = gl::TextureFont::create(Font(transcriptBold,30));
    
    gl::TextureFontRef fontAkkurat14 = gl::TextureFont::create(Font(akkurat,14));
    gl::TextureFontRef fontAkkurat20 = gl::TextureFont::create(Font(akkurat,20));
    gl::TextureFontRef fontAkkurat30 = gl::TextureFont::create(Font(akkurat,30));
    
    gl::TextureFontRef fontAkkuratBold14 = gl::TextureFont::create(Font(akkuratBold,14));
    gl::TextureFontRef fontAkkuratBold20 = gl::TextureFont::create(Font(akkuratBold,20));
    gl::TextureFontRef fontAkkuratBold30 = gl::TextureFont::create(Font(akkuratBold,30));
    


    
    
    gl::Fbo::Format format;
    mFbo = gl::Fbo(app::getWindowWidth(), app::getWindowHeight(), format);
    
    mFbo.bindFramebuffer();
    glPushAttrib(GL_VIEWPORT_BIT);
    gl::clear(Colorf(1,0,0));
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    gl::setMatricesWindow(mFbo.getSize(),false);
    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    glPushMatrix();
    
    float width  = (float)mFbo.getWidth();
    float height = (float)mFbo.getHeight();
    
    int   num   = 10;
    int   rows  = 4;
    float stepX = width / (float)num;
    float stepY = height / (float)(rows);
    float r,g,b;
    
    int i,j;
    
    i = -1;
    while (++i < rows) {
        j = -1;
        
        glColor3f(0,0,0);
        while (++j < num) {
            if ((j % 2) == 0) {
                r = g = b = 0.84705f;
            } else {
                r = 1.0f; g = b = 0.0f;
            }
            glColor3f(r,g,b);
            drawRect(stepX * j, stepY * i, stepX, stepY);
        }
        num   *= 2;
        stepX *= 0.5;
    }
    static Vec2f zero(0,0);
    
    float marginTop = 20;
    
    glColor4f(0, 0, 0, 0.75f);
    glPushMatrix();
    glTranslatef(stepX * 10, height * 0.5f - 405 * 0.5f, 0 );
    drawRect(0, 0, 745, 405);
    glTranslatef(stepX, stepX, 0);
    
    glColor3f(1, 1, 1);
    glTranslatef(0, 80 * 0.75f,0);
    fontTranscript80->drawString("Transcript Headline", zero);
    glTranslatef(0, 50 * 0.75f + marginTop,0);
    fontTranscript50->drawString("Transcript Headline",zero);
    glTranslatef(0, 30 * 0.75f + marginTop,0);
    fontTranscript30->drawString("Transcript Headline",zero);
    
    glTranslatef(0, 30 * 0.75f + marginTop,0);
    fontAkkurat30->drawString("Akkurat Sub",zero);
    glTranslatef(0, 20 * 0.75f + marginTop,0);
    fontAkkurat20->drawString("Akkurat Sub",zero);
    glTranslatef(0, 14 * 0.75f + marginTop,0);
    fontAkkurat14->drawString("Akkurat Sub",zero);
    
    glTranslatef(0, 30 * 0.75f + marginTop,0);
    fontAkkuratBold30->drawString("Akkurat Bold Sub",zero);
    glTranslatef(0, 20 * 0.75f + marginTop,0);
    fontAkkuratBold20->drawString("Akkurat Bold Sub",zero);
    glTranslatef(0, 14 * 0.75f + marginTop,0);
    fontAkkuratBold14->drawString("Akkurat Bold Sub",zero);
    
    glPopMatrix();
   
    
    glColor4f(1,1,1, 0.75f);
    glPushMatrix();
    glTranslatef(width - 745 - stepX * 10, height * 0.5f - 405 * 0.5f, 0 );
    drawRect(0, 0, 745, 405);
    glTranslatef(stepX, stepX, 0);
    
    glColor3f(0,0,0);
    glTranslatef(0, 80 * 0.75f,0);
    fontTranscript80->drawString("Transcript Headline", zero);
    glTranslatef(0, 50 * 0.75f + marginTop,0);
    fontTranscript50->drawString("Transcript Headline",zero);
    glTranslatef(0, 30 * 0.75f + marginTop,0);
    fontTranscript30->drawString("Transcript Headline",zero);
    
    glTranslatef(0, 30 * 0.75f + marginTop,0);
    fontAkkurat30->drawString("Akkurat Sub",zero);
    glTranslatef(0, 20 * 0.75f + marginTop,0);
    fontAkkurat20->drawString("Akkurat Sub",zero);
    glTranslatef(0, 14 * 0.75f + marginTop,0);
    fontAkkurat14->drawString("Akkurat Sub",zero);
    
    glTranslatef(0, 30 * 0.75f + marginTop,0);
    fontAkkuratBold30->drawString("Akkurat Bold Sub",zero);
    glTranslatef(0, 20 * 0.75f + marginTop,0);
    fontAkkuratBold20->drawString("Akkurat Bold Sub",zero);
    glTranslatef(0, 14 * 0.75f + marginTop,0);
    fontAkkuratBold14->drawString("Akkurat Bold Sub",zero);
    
    glPopMatrix();
    
    
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    gl::enableAlphaBlending();
    gl::disableAlphaTest();
    glPopAttrib();
    mFbo.unbindFramebuffer();
    
}



void ProjectionTestApp::keyDown(KeyEvent event){
    if(event.getCode() == KeyEvent::KEY_ESCAPE){
        app::App::get()->quit();
    }
    
}

void ProjectionTestApp::update(){

}

void ProjectionTestApp::draw()
{
    
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatricesWindow(app::getWindowSize());
    glPushMatrix();
    glColor3f(1, 1, 1);
    gl::draw(mFbo.getTexture());
    glPopMatrix();
    
    
}

CINDER_APP_NATIVE( ProjectionTestApp, RendererGl )
