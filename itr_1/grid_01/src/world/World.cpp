#include "world/World.h"
#include "cinder/Plane.h"
#include "util/GeomUtil.h"
#include "world/board/path/PathSurface.h"
#include "layout/quote/QuoteAlign.h"
#include "layout/quote/Quote.h"

#include "util/ShaderUtil.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "Config.h"
#include "Resources.h"

using namespace boost::assign;

/*--------------------------------------------------------------------------------------------*/


World::World(const vector<QuoteJson>& quoteData){
    /*--------------------------------------------------------------------------------------------*/
    //  View
    /*--------------------------------------------------------------------------------------------*/

    mCameraAspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM, mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM,
                     -WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_NEAR_CLIP, WORLD_MODEL_CAM_FAR_CLIP);
    viewOrtho();
    
    mModelScale = WORLD_MODEL_SCALE;
    mTransform  = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    mFrustum.set(mCamera);
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Environment
    /*--------------------------------------------------------------------------------------------*/

    mGrid = new Grid(WORLD_GRID_NUM_CELLS_XY,WORLD_GRID_NUM_CELLS_XY);
    
    // get Area from initial frustum
    const vector<Vec3f>& frPlaneNear = mFrustum.getNearPlane();
    const vector<Vec3f>& frPlaneFar  = mFrustum.getFarPlane();
    Planef xzPlane(Vec3f::zero(), Vec3f::yAxis());
    Vec3f tl,tr,bl,br;
    utils::getIntersection(xzPlane, frPlaneNear[0], frPlaneFar[0], &tl);
    utils::getIntersection(xzPlane, frPlaneNear[1], frPlaneFar[1], &tr);
    utils::getIntersection(xzPlane, frPlaneNear[2], frPlaneFar[2], &br);
    utils::getIntersection(xzPlane, frPlaneNear[3], frPlaneFar[3], &bl);
    
    LayoutArea area(tl,tr,bl,br);
    LayoutArea areaScaled;
    
    area       *= mTransform.inverted();
    areaScaled  = area;
    areaScaled *= Matrix44f::createScale(Vec3f(1.125f,1.125f,1.125f));
    
    //  Typesetter init
    
    mTypesetter = new QuoteTypesetter(mGrid, area);
    mTypesetter->setFont(Font(app::loadResource(RES_FONT_TRANSCRIPT),WORLD_TYPESETTER_FONT_SIZE), WORLD_TYPESETTER_FONT_SCALE);
    mTypesetter->constrain(false);
    mTypesetter->manualLineBreak(true);
#ifdef DEBUG_WORLD_TYPESETTER_TEXCOORDS
    mTypesetter->debugTexture();
#endif
    
    for(auto& data : quoteData){
        const QuoteJson::Format& format = data.format;
        
        mTypesetter->balanceBaseline(format.balance);
        mTypesetter->setAlign(format.align);
        mTypesetter->setPadding(format.padding[0],format.padding[1],format.padding[2],format.padding[3]);
        mTypesetter->setFontScale(format.scale);
        mTypesetter->setString(data.str);
        mQuotes += *mTypesetter->getQuote();
    }
    
    Vec2i windowSize   = app::getWindowSize();
    Vec2i windowSize_2 = windowSize / 2;

    
    mOscillator = new Oscillator();
    mBackground = new Background(mGrid, areaScaled, mOscillator, windowSize.x, windowSize.y);
    mBoard      = new Board(mGrid, areaScaled, mOscillator, &mQuotes);

    
    /*--------------------------------------------------------------------------------------------*/
    //  Fbo + Post Process
    /*--------------------------------------------------------------------------------------------*/

    mTextureNoise  = loadImage(app::loadResource(RES_TEXTURE_NOISE));
    
    gl::Fbo::Format fboFormat;
    fboFormat.setSamples(4);
    
    
    mFboScene       = gl::Fbo(windowSize.x,   windowSize.y,   fboFormat);
    mFboSceneMix    = gl::Fbo(windowSize.x,   windowSize.y,   fboFormat);
    mFboNormalDepth = gl::Fbo(windowSize_2.x, windowSize_2.y, fboFormat);
    mFboBlurH       = gl::Fbo(windowSize_2.x, windowSize_2.y, fboFormat);
    mFboBlurV       = gl::Fbo(windowSize_2.x, windowSize_2.y, fboFormat);
    mFboSSAO        = gl::Fbo(windowSize_2.x, windowSize_2.y, fboFormat);
    mFboMix         = gl::Fbo(windowSize.x,   windowSize.y,   fboFormat);
    
    mFboBlurHRadial = gl::Fbo(windowSize.x,   windowSize.y, fboFormat);
    mFboBlurVRadial = gl::Fbo(windowSize.x,   windowSize.y, fboFormat);
    mFboMixRadial   = gl::Fbo(windowSize.x,   windowSize.y, fboFormat);
    
    
#ifdef WORLD_LIVE_EDIT_FX_SHADER
    mSharedFileWatcher = SharedFileWatcher::Get();
    utils::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_VERT),
                      loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG),
                      &mShaderNormalDepth);
    utils::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_VERT),
                      loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_H_FRAG),
                      &mShaderBlurH);
    utils::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_VERT),
                      loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_V_FRAG),
                      &mShaderBlurV);
    utils::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_VERT),
                      loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_FRAG),
                      &mShaderSSAO);
    utils::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_MIX_VERT),
                      loadFile(RES_ABS_GLSL_WORLD_FX_MIX_FRAG),
                      &mShaderMix);
    utils::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_VERT),
                      loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_FRAG),
                      &mShaderMixRadial);
#else
    utils::loadShader(LoadResource(RES_GLSL_WORLD_FX_NORAML_DEPTH_VERT),
                      LoadResource(RES_GLSL_WORLD_FX_NORAML_DEPTH_FRAG),
                      &mShaderNormalDepth);
    utils::loadShader(LoadResource(RES_GLSL_WORLD_NORMAL_VERT),
                      LoadResource(RES_GLSL_WORLD_NORMAL_FRAG),
                      &mShaderNormal);
    utils::loadShader(LoadResource(RES_GLSL_WORLD_FX_BLUR_H_VERT),
                      LoadResource(RES_GLSL_WORLD_FX_BLUR_H_FRAG),
                      &mShaderBlurH);
    utils::loadShader(LoadResource(RES_GLSL_WORLD_FX_BLUR_V_VERT),
                      LoadResource(RES_GLSL_WORLD_FX_BLUE_V_FRAG),
                      &mShaderBlurV);
    utils::loadShader(LoadResource(RES_GLSL_WORLD_FX_SSAO_VERT),
                      LoadResource(RES_GLSL_WORLD_FX_SSAO_FRAG),
                      &mShaderSSAO);
    utils::loadShader(LoadResource(RES_GLSL_WORLD_FX_MIX_VERT),
                      LoadResource(RES_GLSL_WORLD_FX_MIX_FRAG),
                      &mShaderMix);
#endif
    
}

/*--------------------------------------------------------------------------------------------*/
//  Destructor
/*--------------------------------------------------------------------------------------------*/

World::~World(){
    delete mBackground;
    delete mBoard;
    delete mOscillator;
    delete mTypesetter;
    delete mGrid;
    
    cout << "World destructed." << endl;
}

/*--------------------------------------------------------------------------------------------*/
//  Draw scene
/*--------------------------------------------------------------------------------------------*/

void World::drawScene(bool useMaterialShaders){
    gl::enableDepthRead();
    
    gl::clear(Color(0,0,0));
    gl::pushMatrices();
    gl::setMatrices(mCamera);
    
    mBackground->draw();
    
#ifdef DEBUG_WORLD_CAM_FRUSTUM
    mFrustum.draw();
#endif
    glMultMatrixf(&mTransform[0]);
    
#ifdef DEBUG_WORLD_COORDINATE_FRAME
    gl::drawCoordinateFrame();
#endif

    mBoard->draw(mCamera,useMaterialShaders);

#ifdef DEBUG_WORLD_GRID_DRAW_INDICES
    gl::disableDepthRead();
    mGrid->debugDrawIndices(mCamera);
    gl::enableDepthRead();
#endif
    gl::popMatrices();
    gl::disableDepthRead();
}

/*--------------------------------------------------------------------------------------------*/
//  Post process render
/*--------------------------------------------------------------------------------------------*/

//
//  Render scene
//
void World::renderFboScene(){
    mFboScene.bindFramebuffer();
    drawScene(true);
    mFboScene.unbindFramebuffer();
}

//
//  Render normal depth
//
void World::renderFboNormalDepth(){
    glPushAttrib(GL_VIEWPORT_BIT);
    gl::setViewport(mFboNormalDepth.getBounds());
    mFboNormalDepth.bindFramebuffer();
    mShaderNormalDepth.bind();
    drawScene(false);
    mShaderNormalDepth.unbind();
    mFboNormalDepth.unbindFramebuffer();
    glPopAttrib();
}

//
//  Render blur
//
void World::renderFboBlur(){
    glPushAttrib(GL_VIEWPORT_BIT);
    gl::setViewport(mFboBlurH.getBounds());
    
    //  Blur horizontal
    mFboBlurH.bindFramebuffer();
    glClearColor( 0.5f, 0.5f, 0.5f, 1 );
	glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    gl::setMatricesWindow(mFboBlurH.getSize());
    mFboSSAO.getTexture().bind(0);
	mShaderBlurH.bind();
	mShaderBlurH.uniform("uTexture", 0);
    mShaderBlurH.uniform("uTexelSize", 1.0f / float(mFboBlurH.getWidth()));
    mShaderBlurH.uniform("uScale", WORLD_FX_SHADER_BLUR_SCALE);
    gl::drawSolidRect( Rectf( 0, 0, app::getWindowWidth(), app::getWindowHeight()) );
	mShaderBlurH.unbind();
	mFboSSAO.getTexture().unbind(0);
	mFboBlurH.unbindFramebuffer();
    
    //  Blur vertical
	mFboBlurV.bindFramebuffer();
	glClearColor( 0.5f, 0.5f, 0.5f, 1 );
	glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	gl::setMatricesWindow( mFboBlurV.getSize() );
	mFboBlurH.getTexture().bind(0);
	mShaderBlurV.bind();
	mShaderBlurV.uniform("uTexture", 0);
    mShaderBlurV.uniform("uTexelSize", 1.0f / float(mFboBlurH.getHeight()));
    mShaderBlurV.uniform("uScale", WORLD_FX_SHADER_BLUR_SCALE);
	gl::drawSolidRect( Rectf( 0, 0, app::getWindowWidth(), app::getWindowHeight()) );
	mShaderBlurV.unbind();
	mFboBlurH.getTexture().unbind(0);
	
	mFboBlurV.unbindFramebuffer();
    glPopAttrib();
}

//
//  Render SSAO
//
void World::renderFboSSAO(){
    glPushAttrib(GL_VIEWPORT_BIT);
    gl::setViewport(mFboSSAO.getBounds());
    mFboSSAO.bindFramebuffer();
    glClearDepth(1.0f);
    gl::clear(Color(0.5f,0.5f,0.5f));
    gl::setMatricesWindow(mFboSSAO.getSize());
    
    mTextureNoise.bind(1);
    mFboNormalDepth.getTexture().bind(2);
    mShaderSSAO.bind();
    
    mShaderSSAO.uniform("uTextureRandom", 1);
    mShaderSSAO.uniform("uTextureNormalMap", 2);
    
    gl::drawSolidRect(Rectf(0,0,app::getWindowWidth(),app::getWindowHeight()));
    
    mShaderSSAO.unbind();
    
    mFboNormalDepth.getTexture().unbind(2);
    mTextureNoise.unbind(1);
    
    mFboSSAO.unbindFramebuffer();
    glPopAttrib();
}

//
//  Render radial blur
//
void World::renderFboBlurRadial(){
    gl::pushMatrices();
    gl::setMatricesWindow(mFboScene.getSize());
    
    mFboSceneMix.bindFramebuffer();
    mFboBlurV.getTexture().bind(0);
    mFboScene.getTexture().bind(1);
    
    mShaderMix.bind();
    mShaderMix.uniform("uTextureSSAO", 0);
    mShaderMix.uniform("uTextureBase", 1);
    gl::drawSolidRect( Rectf( 0, app::getWindowHeight(), app::getWindowWidth(), 0) );
    mShaderMix.unbind();
    
    mFboScene.getTexture().unbind(1);
    mFboBlurV.getTexture().unbind(0);
    mFboSceneMix.unbindFramebuffer();
    
    
    mFboBlurHRadial.bindFramebuffer();    // render result back to scene buffer
    glClearColor( 0.5f, 0.5f, 0.5f, 1 );
	glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    mFboSceneMix.getTexture().bind(0);
	mShaderBlurH.bind();
	mShaderBlurH.uniform("uTexture", 0);
    mShaderBlurH.uniform("uTexelSize", 1.0f / float(mFboBlurHRadial.getWidth()));
    mShaderBlurH.uniform("uScale", WORLD_FX_SHADER_BLUR_RADIAL_SCALE);
    gl::drawSolidRect( Rectf( 0, 0, app::getWindowWidth(), app::getWindowHeight()) );
	mShaderBlurH.unbind();
	mFboSceneMix.getTexture().unbind(0);
	mFboBlurHRadial.unbindFramebuffer();
    
    mFboBlurVRadial.bindFramebuffer();
    glClearColor( 0.5f, 0.5f, 0.5f, 1 );
	glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    mFboBlurHRadial.getTexture().bind(0);
    mShaderBlurV.bind();
    mShaderBlurV.uniform("uTexture", 0);
    mShaderBlurV.uniform("uTexelSize", 1.0f / float(mFboBlurHRadial.getHeight()));
    mShaderBlurV.uniform("uScale", WORLD_FX_SHADER_BLUR_RADIAL_SCALE);
    gl::drawSolidRect( Rectf( 0, 0, app::getWindowWidth(), app::getWindowHeight()) );
	mShaderBlurV.unbind();
    mFboBlurHRadial.getTexture().unbind(0);
    mFboBlurVRadial.unbindFramebuffer();
    
    mFboMixRadial.bindFramebuffer();
    glClearColor( 0.5f, 0.5f, 0.5f, 1 );
	glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    mFboSceneMix.getTexture().bind(0);
    mFboBlurVRadial.getTexture().bind(1);
    mShaderMixRadial.bind();
    mShaderMixRadial.uniform("uTexture0", 0);
    mShaderMixRadial.uniform("uTexture1", 1);
    mShaderMixRadial.uniform("uScreenSize", Vec2f(app::getWindowWidth(),app::getWindowHeight()));
    gl::drawSolidRect( Rectf( 0, 0, app::getWindowWidth(), app::getWindowHeight()) );
    mFboBlurVRadial.getTexture().bind(1);
    mFboSceneMix.getTexture().bind(0);
    mShaderMixRadial.unbind();
    mFboMixRadial.unbindFramebuffer();
    /*
    
    mFboBlurV.getTexture().bind(0);
    mFboSceneMix.getTexture().bind(1);
    mShaderBlurH.bind();
    gl::drawSolidRect( Rectf( 0, app::getWindowHeight(), app::getWindowWidth(), 0) );
    mFboSceneMix.getTexture().bind(1);
    mFboBlurV.getTexture().unbind(0);
    */
    
   // mFboScene.unbindFramebuffer();
    
    gl::popMatrices();
}

void World::drawSceneFinal(){
    gl::pushMatrices();
    gl::setMatricesWindow(mFboSceneMix.getSize(),false);
    gl::draw(mFboMixRadial.getTexture(),mFboSceneMix.getBounds());
    gl::popMatrices();
}

/*--------------------------------------------------------------------------------------------*/
//  Update / Draw
/*--------------------------------------------------------------------------------------------*/

void World::update(){
#ifdef WORLD_LIVE_EDIT_FX_SHADER
    utils::watchShaderSource(mSharedFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG),
                             &mShaderNormalDepth);
    utils::watchShaderSource(mSharedFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_FRAG),
                             &mShaderSSAO);
    utils::watchShaderSource(mSharedFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_H_FRAG),
                             &mShaderBlurH);
    utils::watchShaderSource(mSharedFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_V_FRAG),
                             &mShaderBlurV);
    utils::watchShaderSource(mSharedFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_FRAG),
                             &mShaderMixRadial);
#endif
    mBoard->update();
}

void World::draw(){
    renderFboScene();
    renderFboNormalDepth();
    renderFboSSAO();
    renderFboBlur();
    renderFboBlurRadial();
    
    drawSceneFinal();

    
    const static Vec2i windowSize(app::getWindowSize());
    
    gl::pushMatrices();
    gl::setMatricesWindow(windowSize, false);
    gl::disableDepthRead();
    glColor3f(1,1,1);
    //gl::draw(mFboBlurV.getTexture(),       mFboScene.getBounds());
    //gl::draw(mS.getTexture(), mFboNormalDepth.getBounds());
    //gl::draw(mFboNormal.getTexture(),      mFboNormal.getBounds());
#ifdef DEBUG_WORLD_TYPESETTER_TEXTURE
    gl::setMatricesWindow(windowSize);
    const Quote* quote = mBoard->getCurrentQuote();
    if(quote != nullptr){
        gl::draw(quote->getTexture(),Rectf(windowSize.x - 256, 0, windowSize.x, 256));
    }
#endif
    
    gl::popMatrices();
}


/*--------------------------------------------------------------------------------------------*/
//  View
/*--------------------------------------------------------------------------------------------*/

void World::zoomModelIn(){
    mModelScale = MIN(WORLD_MODEL_SCALE_MIN, mModelScale * 2);
    mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
}

void World::zoomModelOut(){
    mModelScale = MIN(mModelScale / 2, WORLD_MODEL_SCALE_MAX);
    mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
}

void World::viewTop(){
    mCamera.lookAt(Vec3f(0,1,0), Vec3f::zero());
}

void World::viewOrtho(){
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
}