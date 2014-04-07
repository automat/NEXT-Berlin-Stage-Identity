#include "world/World.h"
#include "cinder/Plane.h"
#include "util/GeomUtil.h"
#include "world/board/path/PathSurface.h"
#include "layout/quote/QuoteAlign.h"
#include "layout/quote/Quote.h"

#include "util/ShaderUtil.h"
#include "util/DrawUtil.h"

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
    
    
    Vec2i windowSize = app::getWindowSize();
    
    mOscillator = new Oscillator();
    mBackground = new Background(mGrid, areaScaled, mOscillator, windowSize.x, windowSize.y);
    mBoard      = new Board(mGrid, areaScaled, mOscillator, &mQuotes);

    
    /*--------------------------------------------------------------------------------------------*/
    //  Fbo + Post Process
    /*--------------------------------------------------------------------------------------------*/

    gl::Fbo::Format fboFormat;
    fboFormat.setSamples(4);
    
    mFboSize_1      = windowSize;
#ifndef WORLD_SKIP_FX_SHADER
    mTextureNoise   = loadImage(app::loadResource(RES_TEXTURE_NOISE));
    
    mFboSize_1f     = Vec2f(windowSize.x,windowSize.y);
    mFboSize_2      = mFboSize_1 / 2;
    mFboBounds_1    = app::getWindowBounds();
    mFboBounds_2    = Area(Vec2i::zero(), mFboSize_2);
    mFboTexelSize_1 = Vec2f(1.0f / float(mFboSize_1.x), 1.0f / float(mFboSize_1.y));
    mFboTexelSize_2 = Vec2f(1.0f / float(mFboSize_2.x), 1.0f / float(mFboSize_2.y));
    
    mFboSceneSSAO   = gl::Fbo(mFboSize_1.x, mFboSize_1.y,   fboFormat);
    mFboPingPong_1  = PingPongFbo(mFboSize_1.x, mFboSize_1.y, fboFormat);
    mFboPingPong_2  = PingPongFbo(mFboSize_2.x, mFboSize_2.y, fboFormat);
    mFboSceneFinal  = gl::Fbo(mFboSize_1.x, mFboSize_1.y, fboFormat);
#endif
    
    mFboScene       = gl::Fbo(mFboSize_1.x, mFboSize_1.y,   fboFormat);
    
#if defined(WORLD_LIVE_EDIT_FX_SHADER) && !defined(WORLD_SKIP_FX_SHADER)
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
#elif !defined(WORLD_SKIP_FX_SHADER)
    utils::loadShader(app::loadResource(RES_GLSL_WORLD_FX_NORMAL_DEPTH_VERT),
                      app::loadResource(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG),
                      &mShaderNormalDepth);
    utils::loadShader(app::loadResource(RES_GLSL_WORLD_FX_BLUR_VERT),
                      app::loadResource(RES_GLSL_WORLD_FX_BLUR_H_FRAG),
                      &mShaderBlurH);
    utils::loadShader(app::loadResource(RES_GLSL_WORLD_FX_BLUR_V_VERT),
                      app::loadResource(RES_GLSL_WORLD_FX_BLUE_V_FRAG),
                      &mShaderBlurV);
    utils::loadShader(app::loadResource(RES_GLSL_WORLD_FX_SSAO_VERT),
                      app::loadResource(RES_GLSL_WORLD_FX_SSAO_FRAG),
                      &mShaderSSAO);
    utils::loadShader(app::loadResource(RES_GLSL_WORLD_FX_MIX_VERT),
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



void World::processScene(){
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //  Render Scene
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    mFboScene.bindFramebuffer();
    drawScene(true);
    mFboScene.unbindFramebuffer();
    
#ifndef WORLD_SKIP_FX_SHADER
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //  Render normalDepthMap + ssao + blur ssao + both merged
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    /*--------------------------------------------------------------------------------------------*/
    //  Render normal depth map
    /*--------------------------------------------------------------------------------------------*/
    
    glPushAttrib(GL_VIEWPORT_BIT);
    gl::setViewport(mFboBounds_2);
    
    mFboPingPong_2.bindFramebuffer();
   
    mShaderNormalDepth.bind();
    drawScene(false);
    mShaderNormalDepth.unbind();
    
    mFboPingPong_2.unbindFramebuffer();
    mFboPingPong_2.swap();
   
    
    /*--------------------------------------------------------------------------------------------*/
    //  Render ssao
    /*--------------------------------------------------------------------------------------------*/
    
    mFboPingPong_2.bindFramebuffer();
    mFboPingPong_2.bindSourceTexture(2);
    
    mTextureNoise.bind(1);
    mShaderSSAO.bind();
    mShaderSSAO.uniform("uTextureRandom", 1);
    mShaderSSAO.uniform("uTextureNormalMap", 2);
    utils::drawClearedScreenRect(mFboSize_2);
    
    mShaderSSAO.unbind();
    mFboPingPong_2.unbindSourceTexture(2);
    mTextureNoise.unbind(1);
    mFboPingPong_2.unbindFramebuffer();
    mFboPingPong_2.swap();
   
    
    /*--------------------------------------------------------------------------------------------*/
    //  Blur ssao horizontal
    /*--------------------------------------------------------------------------------------------*/
    
    mFboPingPong_2.bindFramebuffer();
    mFboPingPong_2.bindSourceTexture(0);
	mShaderBlurH.bind();
	mShaderBlurH.uniform("uTexture", 0);
    mShaderBlurH.uniform("uTexelSize", mFboTexelSize_2.x);
    mShaderBlurH.uniform("uScale", WORLD_FX_SHADER_BLUR_SCALE);
    utils::drawClearedScreenRect(mFboSize_2);
	mShaderBlurH.unbind();
	mFboPingPong_2.unbindSourceTexture(0);
	mFboPingPong_2.unbindFramebuffer();
    mFboPingPong_2.swap();
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Blur ssao vertical
    /*--------------------------------------------------------------------------------------------*/
    
    mFboPingPong_2.bindFramebuffer();
	mFboPingPong_2.bindSourceTexture(0);
	mShaderBlurV.bind();
	mShaderBlurV.uniform("uTexture", 0);
    mShaderBlurV.uniform("uTexelSize", mFboTexelSize_2.y);
    mShaderBlurV.uniform("uScale", WORLD_FX_SHADER_BLUR_SCALE);
    utils::drawClearedScreenRect(mFboSize_2);
	mShaderBlurV.unbind();
	mFboPingPong_2.unbindSourceTexture(0);
	mFboPingPong_2.unbindFramebuffer();
    mFboPingPong_2.swap();
    
    glPopAttrib();

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //  Mix ssao / scene + result blurred + mix / result merged
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    gl::pushMatrices();
    gl::setMatricesWindow(mFboSize_1);
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Render scene/ssao mix
    /*--------------------------------------------------------------------------------------------*/
    
    mFboSceneSSAO.bindFramebuffer();
    mFboPingPong_2.bindSourceTexture(0);
    mFboScene.getTexture().bind(1);
    
    mShaderMix.bind();
    mShaderMix.uniform("uTextureSSAO", 0);
    mShaderMix.uniform("uTextureBase", 1);
    utils::drawClearedScreenRect(mFboSize_1,false);
    mShaderMix.unbind();
    
    mFboScene.getTexture().unbind(1);
    mFboPingPong_2.unbindSourceTexture(0);
    mFboSceneSSAO.unbindFramebuffer();
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Render mix horizontal blur
    /*--------------------------------------------------------------------------------------------*/
    
    mFboPingPong_1.bindFramebuffer();
    mFboSceneSSAO.getTexture().bind(0);
	
    mShaderBlurH.bind();
	mShaderBlurH.uniform("uTexture", 0);
    mShaderBlurH.uniform("uTexelSize", mFboTexelSize_1.x);
    mShaderBlurH.uniform("uScale", WORLD_FX_SHADER_BLUR_RADIAL_SCALE);
    utils::drawClearedScreenRect(mFboSize_1);
	mShaderBlurH.unbind();
	
    mFboSceneSSAO.getTexture().unbind(0);
	mFboPingPong_1.unbindFramebuffer();
    mFboPingPong_1.swap();
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Render blur vertical blur
    /*--------------------------------------------------------------------------------------------*/
    
    mFboPingPong_1.bindFramebuffer();
    mFboPingPong_1.bindSourceTexture(0);
   
    mShaderBlurV.bind();
    mShaderBlurV.uniform("uTexture", 0);
    mShaderBlurV.uniform("uTexelSize", mFboTexelSize_1.x);
    mShaderBlurV.uniform("uScale", WORLD_FX_SHADER_BLUR_RADIAL_SCALE);
    utils::drawClearedScreenRect(mFboPingPong_1.getSize(),false);
	mShaderBlurV.unbind();
    
    mFboPingPong_1.unbindSourceTexture(0);
    mFboPingPong_1.unbindFramebuffer();
    mFboPingPong_1.swap();
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Render mix result blur radial
    /*--------------------------------------------------------------------------------------------*/
    
    mFboSceneFinal.bindFramebuffer();
    mFboSceneSSAO.getTexture().bind(0);
    mFboPingPong_1.bindSourceTexture(1);
    
    mShaderMixRadial.bind();
    mShaderMixRadial.uniform("uTexture0", 0);
    mShaderMixRadial.uniform("uTexture1", 1);
    mShaderMixRadial.uniform("uScreenSize", Vec2f(app::getWindowWidth(),app::getWindowHeight()));
    utils::drawClearedScreenRect(mFboSceneFinal.getSize());
    mShaderMixRadial.unbind();
    
    mFboPingPong_1.unbindSourceTexture(1);
    mFboSceneSSAO.getTexture().bind(0);
    mFboSceneFinal.unbindFramebuffer();

    
    gl::popMatrices();
#endif
}

/*--------------------------------------------------------------------------------------------*/
//  Update / Draw
/*--------------------------------------------------------------------------------------------*/

void World::update(){
#if defined(WORLD_LIVE_EDIT_FX_SHADER) && !defined(WORLD_SKIP_FX_SHADER)
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
    //  Process scene fx pipe
    processScene();
    
    //  Draw Scene
    gl::pushMatrices();
    gl::setMatricesWindow(app::getWindowSize(),false);
    
#ifndef WORLD_SKIP_FX_SHADER
    gl::draw(mFboSceneFinal.getTexture(),mFboSceneSSAO.getBounds());
#else
    gl::draw(mFboScene.getTexture(), mFboScene.getBounds());
#endif
    
    
    const static Vec2i windowSize(app::getWindowSize());
    
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