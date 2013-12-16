#include "Resources.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Font.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/params/Params.h"
#include <vector>
#include <string>

#define RENDER_SURFACE_OPENGL




using namespace ci;
using namespace ci::app;
using namespace std;

static const int   APP_WIDTH(1600),APP_HEIGHT(600);
static const float APP_FRAME_RATE(30);
static const float CAIRO_CANVAS_PADDING(0);

static bool  PRINT_FONT_NAME(true);
static float FONT_SIZE(125);

// trbl
static float TEXT_PADDING_RATIO[4] = {2,3,3,3};
static bool  SHOW_TEXT_BOUNDS(false), SHOW_SAFE_ZONE(false);

static float SAFE_ZONE_PADDING(0);
static bool  LAYOUT_SETUP_REPEAT(false);
static int   LAYOUT_SETUP_REPEAT_TICK(3);
static int   EPS_FILE_TICK_COUNT = -1;

/*-----------------------------------------------------------------------------------------------------------------------*/

static ci::Colorf bg_color(212.0f/255.0f,212.0f/255.0f,212.0f/255.0f);

static std::vector<ci::Colorf> quote_string_colors = {
    ci::Colorf(0,0,0)
};

static std::vector<ci::Colorf> quote_bg_colors = {
    ci::Colorf(255.0f /255.0f, 48.0f / 255.0f, 73.0f / 255.0f),
    ci::Colorf(1,1,1),
    //ci::Colorf(100.0f / 255.0f, 0.0f, 81.0f / 255.0f),
    //ci::Colorf(0,160.0f/255.0f,228.0f)
};

static std::vector<std::string> quote_strings = {
    "This is The New Normal.",
    "95% defenitive.",
    "Drones dancing. People cheering.",
    "Autonomous unmanned vehicles delivering parcels.",
    "You will be disrupted soon.",
    "Digital human senses.",
    "Replaced by bot.",
    "300 text messages sent per person per day.",
    "Marketing is dead . Long live marketing.",
    "Data-driven intuition.",
    "99.999999% reliable.",
    "Cash- and plastic- less.",
    "Orchestration, not control.",
    "Screenlight dinners.",
    "A prototype says more than 1000 images.",
    "10.500.500.500 daily video uploads.",
    "Dispute with your digital selfs." };

static const std::string FONT_NAME("Transcript-Bold");

/*-----------------------------------------------------------------------------------------------------------------------*/

class StringSurfaceTextureApp : public AppNative {
public:
	void setup();
    void prepareSettings(Settings* settings);
	
    void update();
	void draw();
    void resize();
    
    void setupLayout();
    void printFontNames();
    void cairoRenderScene(cairo::Context& ctx);
    void cairoDrawString(cairo::Context& ctx,
                         const std::string& str,
                         const ci::Vec2f& p,
                         const ci::Colorf& color_str,
                         const ci::Colorf& color_bg,
                         bool  drawFakeLight,
                         ci::Vec2f* offset);
    void cairoRenderToEPS();
    
    cairo::FontExtents m_font_extents;
    double             m_font_extents_line_height;
    float              m_text_padding[4];
    ci::Rectf          m_safe_zone;
    
    ci::params::InterfaceGlRef m_params;
#ifdef RENDER_SURFACE_OPENGL
    ci::gl::Texture    m_string_texture;
#else
    
#endif
};

void StringSurfaceTextureApp::prepareSettings(Settings *settings){
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FRAME_RATE);
}

/*-----------------------------------------------------------------------------------------------------------------------*/


void StringSurfaceTextureApp::setup(){
    m_params = ci::params::InterfaceGl::create(app::getWindow(), "Controls", ci::Vec2f(300,180));
    m_params->addParam("Font Size", &FONT_SIZE, "min=15 max=200 step=1");
    m_params->addParam("Text Padding T Ratio", &TEXT_PADDING_RATIO[0], "min=0 max=20 step=1");
    m_params->addParam("Text Padding R Ratio", &TEXT_PADDING_RATIO[1], "min=0 max=20 step=1");
    m_params->addParam("Text Padding B Ratio", &TEXT_PADDING_RATIO[2], "min=0 max=20 step=1");
    m_params->addParam("Text Padding L Ratio", &TEXT_PADDING_RATIO[3], "min=0 max=20 step=1");
    m_params->addButton("HIT ME!", std::bind(&StringSurfaceTextureApp::setupLayout, this));
    m_params->addSeparator();
    m_params->addParam("Disco Mode", &LAYOUT_SETUP_REPEAT);
    m_params->addParam("Disco Mode Tick Num", &LAYOUT_SETUP_REPEAT_TICK, "min=1 max=30 step=1");
    m_params->addSeparator();
    m_params->addButton("Render To EPS", std::bind(&StringSurfaceTextureApp::cairoRenderToEPS, this));
    
    if(PRINT_FONT_NAME)this->printFontNames();
    this->setupLayout();
}

void StringSurfaceTextureApp::printFontNames(){
	for( vector<string>::const_iterator fontName = Font::getNames().begin(); fontName != Font::getNames().end(); ++fontName )
		std::cout << *fontName << std::endl;
}

void StringSurfaceTextureApp::setupLayout(){

   // ci::Font    font(ci::Font(loadResource(FONT_TRANSCRIPT_BOLD),FONT_SIZE));
    m_safe_zone = ci::Rectf(SAFE_ZONE_PADDING,SAFE_ZONE_PADDING,
                            app::getWindowWidth()-SAFE_ZONE_PADDING,app::getWindowHeight()-SAFE_ZONE_PADDING);
    
    m_text_padding[0] = FONT_SIZE / TEXT_PADDING_RATIO[0];
    m_text_padding[1] = FONT_SIZE / TEXT_PADDING_RATIO[1];
    m_text_padding[2] = FONT_SIZE / TEXT_PADDING_RATIO[2];
    m_text_padding[3] = FONT_SIZE / TEXT_PADDING_RATIO[3];
    
    
    // Setup cairo
    cairo::SurfaceImage surface(app::getWindowWidth()  - CAIRO_CANVAS_PADDING * 2, app::getWindowHeight() - CAIRO_CANVAS_PADDING * 2);
    cairo::Context ctx(surface);
    
    this->cairoRenderScene(ctx);

#ifdef RENDER_SURFACE_OPENGL
    // Get cairo texture
    m_string_texture = ci::gl::Texture(surface.getSurface());
#else
    
#endif
}

void StringSurfaceTextureApp::cairoRenderScene(cairo::Context &ctx){
    ctx.selectFontFace(FONT_NAME, cairo::FONT_SLANT_NORMAL, cairo::FONT_WEIGHT_BOLD);
    //ctx.setFont(font);
    ctx.setFontSize(FONT_SIZE);
    
    m_font_extents = ctx.fontExtents();
    m_font_extents_line_height = m_font_extents.descent() - m_font_extents.ascent() + m_font_extents.height();
    
    // Begin paint
    ctx.setSourceRgb(bg_color.r, bg_color.g, bg_color.b);
    ctx.paint();
    
    
    //initial x pos after reset
    float quote_word_reset_x = -100;
    
    // line height step
    int word_offset_step_y = m_font_extents_line_height + m_text_padding[0] + m_text_padding[2];
    int line_index = 0;
    
    ci::Colorf main_quote_string_color(1,1,1);
    ci::Colorf main_quote_bg_color(0,0,0);
    
    std::vector<std::string> quote_words;
    ci::Vec2f quote_word_offset(quote_word_reset_x,0);
    
    int index_quote_strings = -1;
    int index_quote_strings_last = -1;
    
    int j;
    while (true) {
        
        if (quote_word_offset.y > m_safe_zone.getY2()) {
            break;
        }
        
        index_quote_strings = ci::randInt(0,quote_strings.size());
        std::string& quote_string = quote_strings[index_quote_strings];
        
        ci::Colorf& quote_string_color    = index_quote_strings == 0 ? main_quote_string_color : quote_string_colors[0] ;
        ci::Colorf& quote_string_color_bg = index_quote_strings == 0 ? main_quote_bg_color     : quote_bg_colors[ci::randInt(quote_bg_colors.size())];
        
        quote_words.clear();
        
        istringstream iss(quote_string);
        copy(std::istream_iterator<std::string>(iss),
             std::istream_iterator<std::string>(),
             std::back_inserter<std::vector<std::string>>(quote_words));
        
        if(!m_safe_zone.contains(quote_word_offset)){
            quote_word_offset.x = quote_word_reset_x;
            quote_word_offset.y = word_offset_step_y * line_index++;
        }
        
        if(index_quote_strings == 0){
            this->cairoDrawString(ctx,
                                  quote_string,
                                  quote_word_offset,
                                  quote_string_color,
                                  quote_string_color_bg,
                                  j!=0,
                                  &quote_word_offset);
        } else {
            j = -1;
            while (++j < quote_words.size()) {
                this->cairoDrawString(ctx,
                                      quote_words[j],
                                      quote_word_offset,
                                      quote_string_color,
                                      quote_string_color_bg,
                                      true,//index_quote_strings != 0 && j!=0,
                                      &quote_word_offset);
            }
        }
        
        index_quote_strings_last = index_quote_strings;
        
    }
    
    if (SHOW_SAFE_ZONE) {
        ctx.setSourceRgb(1, 1, 0);
        ctx.rectangle(m_safe_zone.getX1(), m_safe_zone.getY1(),
                      m_safe_zone.getWidth(), m_safe_zone.getHeight());
        ctx.stroke();
    }
}

void StringSurfaceTextureApp::cairoDrawString(cairo::Context& ctx,
                                              const std::string& str,
                                              const ci::Vec2f& p,
                                              const ci::Colorf& color_str,
                                              const ci::Colorf& color_bg,
                                              bool  draw_fake_light,
                                              ci::Vec2f* offset){
    ctx.save();
    // preserve previous transformations
    cairo::Matrix prevMat;
    ctx.getMatrix(&prevMat);
    ctx.translate(p.x, p.y);

    // get text extents
    cairo::TextExtents text_extends = ctx.textExtents(str);

    double text_extents_width = text_extends.width();
    int    text_bounds_width  = text_extents_width         + m_text_padding[1] + m_text_padding[3];
    int    text_bounds_height = m_font_extents_line_height + m_text_padding[0] + m_text_padding[2];
    
    static const int gradient_width = 8;
    
    // draw quote bg
    ctx.setSourceRgb(color_bg.r, color_bg.g, color_bg.b);
    ctx.rectangle(0, 0, text_bounds_width, text_bounds_height);
    ctx.fill();
    
    // draw quote bg gradient
    cairo::GradientLinear gradient_bg(0, 0, text_bounds_width, 0);
    gradient_bg.addColorStopRgba(0, 0, 0, 0, 0.15f);
    gradient_bg.addColorStopRgba(1, 0, 0, 0, 0);
    ctx.setSource(gradient_bg);
    ctx.rectangle(0, 0, text_bounds_width, text_bounds_height);
    ctx.fill();
    
    // draw fake light gradient
    if(draw_fake_light){
        cairo::GradientLinear gradient_shadow(0, 0, gradient_width, 0);
        gradient_shadow.addColorStopRgba(0, 0, 0, 0, 0.25f);
        gradient_shadow.addColorStopRgba(1, 0, 0, 0, 0);
        ctx.setSource(gradient_shadow);
        ctx.rectangle(0, 0, gradient_width, text_bounds_height);
        ctx.fill();
    }
   
    /*
    // draw edge gradient
    cairo::GradientLinear gradient_edge(text_bounds_width - gradient_width, 0, text_bounds_width, 0);
    gradient_edge.addColorStopRgba(0, 1, 1, 1, 0.125f);
    gradient_edge.addColorStopRgba(1, 1, 1, 1, 0);
    ctx.setSource(gradient_edge);
    ctx.rectangle(text_bounds_width - gradient_width, 0, gradient_width, text_bounds_height);
    ctx.fill();
    */
    
    ctx.translate(int(m_text_padding[3]), int(m_text_padding[0]));
    // Draw text bounds
    if(SHOW_TEXT_BOUNDS){
        ctx.setSourceRgb(0, 1, 0);
        ctx.rectangle(0, 0, text_extents_width, m_font_extents_line_height);
        ctx.stroke();
    }
    
    // draw quote string
    ctx.translate(0, int(m_font_extents_line_height));
    ctx.setSourceRgb(color_str.r, color_str.g, color_str.b);
    ctx.showText(str);
    ctx.fill();
    
    ctx.restore();
    ctx.setMatrix(prevMat);
    
    offset->x += text_bounds_width;
}

void StringSurfaceTextureApp::cairoRenderToEPS(){
    EPS_FILE_TICK_COUNT++;
    std::stringstream sstream;
    sstream << "StringSurfaceTexture" << EPS_FILE_TICK_COUNT << ".eps";
    std::string filename = sstream.str();
    
    cairo::Context ctx( cairo::SurfaceEps(getHomeDirectory() / filename,
                                          app::getWindowWidth()  - CAIRO_CANVAS_PADDING * 2, app::getWindowHeight() - CAIRO_CANVAS_PADDING * 2));
    cairoRenderScene(ctx);
}



void StringSurfaceTextureApp::update(){
    if(LAYOUT_SETUP_REPEAT)if(app::getElapsedFrames()%LAYOUT_SETUP_REPEAT_TICK==0)this->setupLayout();
}

/*-----------------------------------------------------------------------------------------------------------------------*/

void StringSurfaceTextureApp::draw(){
#ifdef RENDER_SURFACE_OPENGL
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    glClear(GL_COLOR_BUFFER_BIT);
    gl::setMatricesWindow(app::getWindowSize());
    gl::enableAlphaBlending(false);
    
    ci::gl::color(ci::Color::white());
    ci::gl::draw(m_string_texture, ci::Vec2f(CAIRO_CANVAS_PADDING,CAIRO_CANVAS_PADDING));
    
    m_params->draw();
#else
#endif
}

void StringSurfaceTextureApp::resize(){this->setupLayout();}

/*-----------------------------------------------------------------------------------------------------------------------*/
#ifdef RENDER_SURFACE_OPENGL
CINDER_APP_NATIVE( StringSurfaceTextureApp, RendererGl )
#else
#endif
