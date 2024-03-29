#include "stage/Stage.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "Resources.h"
#include "cinder/Plane.h"

#include "Config.h"

#include "util/GeomUtil.h"
#include "util/ShaderUtil.h"
#include "util/DrawUtil.h"
#include "util/SharedTextureFont.h"

#include "layout/quote/QuoteAlign.h"
#include "layout/quote/Quote.h"

#include "stage/theme/path/PathSurface.h"



using namespace boost::assign;

/*--------------------------------------------------------------------------------------------*/


Stage::Stage(const vector<QuoteJson>& quoteData){
    /*--------------------------------------------------------------------------------------------*/
    //  View
    /*--------------------------------------------------------------------------------------------*/

    mCameraAspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-mCameraAspectRatio * STAGE_MODEL_CAM_ZOOM, mCameraAspectRatio * STAGE_MODEL_CAM_ZOOM,
                     -STAGE_MODEL_CAM_ZOOM, STAGE_MODEL_CAM_ZOOM, STAGE_MODEL_CAM_NEAR_CLIP, STAGE_MODEL_CAM_FAR_CLIP);
    viewOrtho();
    
    mModelScale = STAGE_MODEL_SCALE;
    mTransform  = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    mFrustum.set(mCamera);
    
    mLantern0         = new Lantern(0);
    mLantern1         = new Lantern(1);
    mLantern0DebugDraw = false;
    mLantern1DebugDraw = false;
    loadLightProperties();

    
    /*--------------------------------------------------------------------------------------------*/
    //  Environment
    /*--------------------------------------------------------------------------------------------*/

    mGrid = new Grid(STAGE_GRID_NUM_CELLS_XY,STAGE_GRID_NUM_CELLS_XY);
    
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
    areaScaled *= Matrix44f::createScale(Vec3f(1.125f,1.0f,1.125f));
    
    /*--------------------------------------------------------------------------------------------*/
    //  Environment
    /*--------------------------------------------------------------------------------------------*/

    // viewport
    
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Typesetter init
    /*--------------------------------------------------------------------------------------------*/

    mTypesetter = new QuoteTypesetter(mGrid, area);
    mTypesetter->setFont(Font(app::loadResource(RES_FONT_TRANSCRIPT),STAGE_TYPESETTER_FONT_SIZE), STAGE_TYPESETTER_FONT_SCALE);
    mTypesetter->constrain(false);
    mTypesetter->manualLineBreak(true);
#ifdef DEBUG_STAGE_TYPESETTER_TEXCOORDS
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
    
    mOscillator   = new Oscillator();
    mBackground   = new Background(mGrid, areaScaled, mOscillator, windowSize.x, windowSize.y);
#ifndef STAGE_SKIP_THEME_VIEW
    mThemeView    = new ThemeView(mGrid, areaScaled, mOscillator, &mQuotes);
#endif
#ifndef STAGE_SKIP_SCHEDULE_VIEW
    mScheduleView = new ScheduleView(mGrid, areaScaled);
#endif
    
    /*--------------------------------------------------------------------------------------------*/
    //  Fbo + Post Process
    /*--------------------------------------------------------------------------------------------*/

    gl::Fbo::Format fboFormat_MSAA_4;
    fboFormat_MSAA_4.setSamples(4);
    
    gl::Fbo::Format fboFormat_MSAA_2;
    fboFormat_MSAA_2.setSamples(2);
    
    gl::Fbo::Format fboFormat_RGBA_MSAA_4;
    fboFormat_RGBA_MSAA_4.setSamples(4);
    fboFormat_RGBA_MSAA_4.setColorInternalFormat(GL_RGBA16F_ARB);
    
    mFboSize_1      = windowSize;
#ifndef STAGE_SKIP_FX_SHADER
    mTextureNoise   = loadImage(app::loadResource(RES_TEXTURE_NOISE));
    
    mFboSize_1f     = Vec2f(windowSize.x,windowSize.y);
    mFboSize_2      = mFboSize_1 / 2;
    mFboBounds_1    = app::getWindowBounds();
    mFboBounds_2    = Area(Vec2i::zero(), mFboSize_2);
    mFboTexelSize_1 = Vec2f(1.0f / float(mFboSize_1.x), 1.0f / float(mFboSize_1.y));
    mFboTexelSize_2 = Vec2f(1.0f / float(mFboSize_2.x), 1.0f / float(mFboSize_2.y));
    
    mFboThemeViewSSAO     = gl::Fbo(mFboSize_1.x, mFboSize_1.y, fboFormat_MSAA_4);
    mFboThemeViewFinal    = gl::Fbo(mFboSize_1.x, mFboSize_1.y, fboFormat_MSAA_4);
    mFboScheduleViewSSAO  = gl::Fbo(mFboSize_1.x, mFboSize_1.y, fboFormat_MSAA_4);
    mFboScheduleViewFinal = gl::Fbo(mFboSize_1.x, mFboSize_1.y, fboFormat_MSAA_4);
    
    mFboPingPong_1     = PingPongFbo(mFboSize_1.x, mFboSize_1.y, fboFormat_MSAA_4);
    mFboPingPong_2     = PingPongFbo(mFboSize_2.x, mFboSize_2.y, fboFormat_MSAA_4);

#endif
    
    mFboThemeView    = gl::Fbo(mFboSize_1.x, mFboSize_1.y, fboFormat_MSAA_4);
    mFboScheduleView = gl::Fbo(mFboSize_1.x, mFboSize_1.y, fboFormat_RGBA_MSAA_4);
    
#if defined(STAGE_LIVE_EDIT_FX_SHADER) && !defined(STAGE_SKIP_FX_SHADER)
    mFileWatcher = FileWatcher::Get();
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
#elif !defined(STAGE_SKIP_FX_SHADER)
    utils::loadShader(app::loadResource(RES_GLSL_WORLD_FX_NORMAL_DEPTH_VERT),
                      app::loadResource(RES_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG),
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

Stage::~Stage(){
    delete mBackground;
#ifndef STAGE_SKIP_THEME_VIEW
    delete mThemeView;
#endif
#ifndef STAGE_SKIP_SCHEDULE_VIEW
    delete mScheduleView;
#endif
    delete mOscillator;
    delete mTypesetter;
    delete mGrid;
    
    cout << "World destructed." << endl;
}

/*--------------------------------------------------------------------------------------------*/
// Load properties
/*--------------------------------------------------------------------------------------------*/

void Stage::loadLightProperties(){
    //
    //  Lantern 0
    //
    mLantern0->setDirection(STAGE_LANTERN_0_DIRECTION);
    
    mLantern0->setAmbient(STAGE_LANTERN_0_COLOR_AMBIENT);
    mLantern0->setDiffuse(STAGE_LANTERN_0_COLOR_DIFFUSE);
    mLantern0->setSpecular(STAGE_LANTERN_0_COLOR_SPECULAR);
    
    mLantern0->setAttenuation(STAGE_LANTERN_0_ATTENUATION);
    mLantern0->setConstantAttenuation(STAGE_LANTERN_0_CONSTANT_ATTENUATION);
    mLantern0->setLinearAttenuation(STAGE_LANTERN_0_LINEAR_ATTENUATION);
    mLantern0->setQuadraticAttenuation(STAGE_LANTERN_0_QUADRIC_ATTENUATION);
    
    mLantern0DebugDraw = STAGE_LANTERN_0_DEBUG_DRAW;
    
    //
    //  Lantern 1
    //
    mLantern1->setDirection(STAGE_LANTERN_1_DIRECTION);
    
    mLantern1->setAmbient(STAGE_LANTERN_1_COLOR_AMBIENT);
    mLantern1->setDiffuse(STAGE_LANTERN_1_COLOR_DIFFUSE);
    mLantern1->setSpecular(STAGE_LANTERN_1_COLOR_SPECULAR);
    
    mLantern1->setAttenuation(STAGE_LANTERN_1_ATTENUATION);
    mLantern1->setConstantAttenuation(STAGE_LANTERN_1_CONSTANT_ATTENUATION);
    mLantern1->setLinearAttenuation(STAGE_LANTERN_1_LINEAR_ATTENUATION);
    mLantern1->setQuadraticAttenuation(STAGE_LANTERN_1_QUADRIC_ATTENUATION);
    
    mLantern1DebugDraw = STAGE_LANTERN_1_DEBUG_DRAW;

}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Theme view
//
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef STAGE_SKIP_THEME_VIEW
/*--------------------------------------------------------------------------------------------*/
//  Draw theme view
/*--------------------------------------------------------------------------------------------*/

void Stage::drawThemeView(bool useMaterialShaders){
    gl::enableDepthRead();
    
    gl::clear(Color(1,1,1));
    gl::pushMatrices();
    gl::setMatrices(mCamera);
    
    
#ifdef DEBUG_STAGE_CAM_FRUSTUM
    mFrustum.draw();
#endif
    glMultMatrixf(&mTransform[0]);
    
#ifdef DEBUG_STAGE_COORDINATE_FRAME
    gl::drawCoordinateFrame();
#endif
    
    if(useMaterialShaders){
        mLantern0->enable();
        mLantern0->update(mCamera);
        mLantern1->enable();
        mLantern1->update(mCamera);
    } else {
        mLantern0->disable();
        mLantern1->disable();
    }
    
    mBackground->draw();
    mThemeView->draw(mCamera,useMaterialShaders);
    
    if(useMaterialShaders){
        mLantern0->disable();
        mLantern1->disable();
    }
#ifdef DEBUG_STAGE_GRID_DRAW_INDICES
    gl::disableDepthRead();
    mGrid->debugDrawIndices(mCamera);
    gl::enableDepthRead();
#endif
    
    if(mLantern0DebugDraw){
        mLantern0->debugDraw();
    }
    
    if(mLantern1DebugDraw){
        mLantern1->debugDraw();
    }
    
    gl::popMatrices();
    gl::disableDepthRead();
}

/*--------------------------------------------------------------------------------------------*/
//  Post process theme view
/*--------------------------------------------------------------------------------------------*/

void Stage::processThemeView(){
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //  Render View
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    mFboThemeView.bindFramebuffer();
    drawThemeView(true);
    mFboThemeView.unbindFramebuffer();
    
#ifndef STAGE_SKIP_FX_SHADER
    
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
    drawThemeView(false);
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
    mShaderBlurH.uniform("uScale", STAGE_FX_SHADER_BLUR_SCALE);
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
    mShaderBlurV.uniform("uScale", STAGE_FX_SHADER_BLUR_SCALE);
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
    
    mFboThemeViewSSAO.bindFramebuffer();
    mFboPingPong_2.bindSourceTexture(0);
    mFboThemeView.getTexture().bind(1);
    
    mShaderMix.bind();
    mShaderMix.uniform("uTextureSSAO", 0);
    mShaderMix.uniform("uTextureBase", 1);
    utils::drawClearedScreenRect(mFboSize_1,false);
    mShaderMix.unbind();
    
    mFboThemeView.getTexture().unbind(1);
    mFboPingPong_2.unbindSourceTexture(0);
    mFboThemeViewSSAO.unbindFramebuffer();
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Render mix horizontal blur
    /*--------------------------------------------------------------------------------------------*/
    
    mFboPingPong_1.bindFramebuffer();
    mFboThemeViewSSAO.getTexture().bind(0);
	
    mShaderBlurH.bind();
	mShaderBlurH.uniform("uTexture", 0);
    mShaderBlurH.uniform("uTexelSize", mFboTexelSize_1.x);
    mShaderBlurH.uniform("uScale", STAGE_FX_SHADER_BLUR_RADIAL_SCALE);
    utils::drawClearedScreenRect(mFboSize_1);
	mShaderBlurH.unbind();
	
    mFboThemeViewSSAO.getTexture().unbind(0);
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
    mShaderBlurV.uniform("uScale", STAGE_FX_SHADER_BLUR_RADIAL_SCALE);
    utils::drawClearedScreenRect(mFboPingPong_1.getSize(),false);
	mShaderBlurV.unbind();
    
    mFboPingPong_1.unbindSourceTexture(0);
    mFboPingPong_1.unbindFramebuffer();
    mFboPingPong_1.swap();
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Render mix result blur radial
    /*--------------------------------------------------------------------------------------------*/
    
    mFboThemeViewFinal.bindFramebuffer();
    mFboThemeViewSSAO.getTexture().bind(0);
    mFboPingPong_1.bindSourceTexture(1);
    
    mShaderMixRadial.bind();
    mShaderMixRadial.uniform("uTexture0", 0);
    mShaderMixRadial.uniform("uTexture1", 1);
    mShaderMixRadial.uniform("uScreenSize", Vec2f(app::getWindowWidth(),app::getWindowHeight()));
    mShaderMixRadial.uniform("uScaleGradient", STAGE_FX_SHADER_BLUR_RADIAL_RADIUS_SCALE);
    utils::drawClearedScreenRect(mFboThemeViewFinal.getSize());
    mShaderMixRadial.unbind();
    
    mFboPingPong_1.unbindSourceTexture(1);
    mFboThemeViewSSAO.getTexture().bind(0);
    mFboThemeViewFinal.unbindFramebuffer();

    
    gl::popMatrices();
#endif
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Schedule view
//
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef STAGE_SKIP_SCHEDULE_VIEW
/*--------------------------------------------------------------------------------------------*/
//  Draw schedule view
/*--------------------------------------------------------------------------------------------*/

void Stage::drawScheduleView(bool useMaterialShaders){
    gl::enableDepthRead();
    gl::clear(ColorAf(0,0,0,0));
    
    gl::pushMatrices();
    glMultMatrixf(&mTransform[0]);
    mScheduleView->draw(mCamera, useMaterialShaders);
    gl::popMatrices();
    
    gl::disableDepthRead();
}

/*--------------------------------------------------------------------------------------------*/
//  Post process schedule view
/*--------------------------------------------------------------------------------------------*/

void Stage::processScheduleView(){
    
    mFboScheduleView.bindFramebuffer();

    drawScheduleView(true);
    mFboScheduleView.unbindFramebuffer();
    
}
#endif



/*--------------------------------------------------------------------------------------------*/
//  Update
/*--------------------------------------------------------------------------------------------*/

void Stage::update(){
#if defined(STAGE_LIVE_EDIT_FX_SHADER) && !defined(STAGE_SKIP_FX_SHADER)
    utils::watchShaderSource(mFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG),
                             &mShaderNormalDepth);
    utils::watchShaderSource(mFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_FRAG),
                             &mShaderSSAO);
    utils::watchShaderSource(mFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_H_FRAG),
                             &mShaderBlurH);
    utils::watchShaderSource(mFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_BLUR_V_FRAG),
                             &mShaderBlurV);
    utils::watchShaderSource(mFileWatcher,
                             loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_VERT),
                             loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_FRAG),
                             &mShaderMixRadial);
#endif
    mBackground->update(mOscillator,app::getElapsedSeconds());
#ifndef STAGE_SKIP_THEME_VIEW
    mThemeView->update();
#endif
#ifndef STAGE_SKIP_SCHEDULE_VIEW
    mScheduleView->update();
#endif
}


/*--------------------------------------------------------------------------------------------*/
//  Draw
/*--------------------------------------------------------------------------------------------*/

void Stage::draw(){
#ifndef STAGE_SKIP_THEME_VIEW
    //  Process theme fx pipe
    processThemeView();
#endif
#ifndef STAGE_SKIP_SCHEDULE_VIEW
    //  Process schedule fx pipe
    processScheduleView();
#endif
    
    //  Draw Scene
    gl::pushMatrices();
    gl::setMatricesWindow(app::getWindowSize(),false);
    
#ifndef STAGE_SKIP_THEME_VIEW
#ifndef STAGE_SKIP_FX_SHADER
    gl::draw(mFboThemeViewFinal.getTexture(), mFboThemeViewSSAO.getBounds());
#else
    gl::draw(mFboThemeView.getTexture(), mFboThemeView.getBounds());
#endif
#endif

#ifndef STAGE_SKIP_SCHEDULE_VIEW
    
    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_ALPHA_TEST);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    gl::draw(mFboScheduleView.getTexture(), mFboScheduleView.getBounds());
    
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5); // reset what seems to be cinders default
  
#endif
    
#ifdef DEBUG_STAGE_TYPESETTER_TEXTURE
    gl::disableDepthRead();
    glColor3f(1, 1, 1);
    const Quote* quote = mThemeView->getCurrentQuote();
    if(quote != nullptr){
        gl::draw(quote->getTexture(),Rectf(mFboSize_1.x - 256,mFboSize_1.y,mFboSize_1.x,mFboSize_1.y - 256));
    }
    gl::enableDepthRead();
#endif
    
    drawExportViewports();
    
    gl::popMatrices();
}

/*--------------------------------------------------------------------------------------------*/
//  Draw reference export viewports
/*--------------------------------------------------------------------------------------------*/


void Stage::drawExportLayoutArea(const LayoutArea &area){
    const gl::TextureFontRef& debugFont = SharedTextureFont::Get();
    static const float fontScale = 0.005f;
    
    Vec3f v;
    Vec3f w;
    Vec3f u;
    
    mCamera.getBillboardVectors(&w, &u);
    v = w.cross(u);
    
    Matrix44f mat = Matrix44f::createTranslation(area.getTL());
    Matrix44f rot = Matrix44f::createRotationOnb(u,w,v);
    rot*= Matrix44f::createRotation(Vec3f::zAxis(), M_PI_2);
    rot*= Matrix44f::createScale(Vec3f(fontScale,fontScale,fontScale));
    mat*= rot;
    
    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    glColor3f(1, 1, 1);
    
    glPushMatrix();
    glMultMatrixf(&mat[0]);
    
    debugFont->drawString(toString(area.getSize()), Vec2f(0,debugFont->getAscent()));
    glPopMatrix();
    
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
    
    static int indices[] = {0,1,3,2};
    
   
    Vec3f vertices[] = {area.getTL(), area.getTR(),area.getBL(),area.getBR()};
    
    glLineWidth(3);
    glColor3f(1,0,0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
    glLineWidth(1);


}

void Stage::drawExportViewports(){

    gl::pushMatrices();
    gl::setMatrices(mCamera);
    glMultMatrixf(&mTransform[0]);
    

    //drawExportLayoutArea(mArea_1920_1080);
    
    
    gl::popMatrices();
}

/*--------------------------------------------------------------------------------------------*/
//  View
/*--------------------------------------------------------------------------------------------*/

static float zoomStep = 1.125f;

void Stage::zoomModelIn(){
    mModelScale = MIN(STAGE_MODEL_SCALE_MIN, mModelScale * zoomStep);
    mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    cout << "Model scale: " << mModelScale <<  endl;
}

void Stage::zoomModelOut(){
    mModelScale = MIN(mModelScale / zoomStep, STAGE_MODEL_SCALE_MAX);
    mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    cout << "Model scale: " << mModelScale <<  endl;
}

void Stage::viewTop(){
    mCamera.lookAt(Vec3f(0,1,0), Vec3f::zero());
}

void Stage::viewOrtho(){
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
}


/*--------------------------------------------------------------------------------------------*/
//  Control
/*--------------------------------------------------------------------------------------------*/

void Stage::onConfigDidChange(){
    loadLightProperties();
    mThemeView->onConfigDidChange();
}

void Stage::wakeUp(){
    
}

void Stage::tearDown(){
    
}
