#include "stage/Stage.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "Resources.h"
#include "cinder/Plane.h"

#include "Config.h"

#include "util/GeomUtil.h"
#include "util/gl/ShaderUtil.h"
#include "util/gl/DrawUtil.h"

#include "quote/QuoteAlign.h"
#include "quote/Quote.h"

#include "stage/theme/path/PathSurface.h"

namespace next{
    using namespace boost::assign;

    Stage::Stage(vector<QuoteJson>* quoteData, Session* sessionData){
        //
        //
        //
        //  View
        //
        //
        //

        mCameraAspectRatio = app::getWindowAspectRatio();
        mCamera.setOrtho(-mCameraAspectRatio * STAGE_MODEL_CAM_ZOOM, mCameraAspectRatio * STAGE_MODEL_CAM_ZOOM,
                -STAGE_MODEL_CAM_ZOOM, STAGE_MODEL_CAM_ZOOM, STAGE_MODEL_CAM_NEAR_CLIP, STAGE_MODEL_CAM_FAR_CLIP);
        viewOrtho();

        mModelScale = STAGE_MODEL_SCALE;
        mTransform  = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
        mFrustum.set(mCamera);

        mLantern0 = new Lantern(0);
        mLantern1 = new Lantern(1);
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
        util::getIntersection(xzPlane, frPlaneNear[0], frPlaneFar[0], &tl);
        util::getIntersection(xzPlane, frPlaneNear[1], frPlaneFar[1], &tr);
        util::getIntersection(xzPlane, frPlaneNear[2], frPlaneFar[2], &br);
        util::getIntersection(xzPlane, frPlaneNear[3], frPlaneFar[3], &bl);

        LayoutArea area(tl,tr,bl,br);
        LayoutArea areaScaled;

        area       *= mTransform.inverted();
        areaScaled  = area;
        areaScaled *= Matrix44f::createScale(Vec3f(1.125f,1.125f,1.125f));

        //  Typesetter init

        mTypesetter = new QuoteTypesetter(mGrid, area);
        mTypesetter->setFont(Font(app::loadResource(RES_TRANSCRIPT_BOLD),STAGE_TYPESETTER_FONT_SIZE), STAGE_TYPESETTER_FONT_SCALE);
        mTypesetter->constrain(false);
        mTypesetter->manualLineBreak(true);
#ifdef DEBUG_STAGE_TYPESETTER_TEXCOORDS
        mTypesetter->debugTexture();
#endif

        for(auto& data : *quoteData){
            const QuoteJson::Format& format = data.format;

            mTypesetter->balanceBaseline(format.balance);
            mTypesetter->setAlign(format.align);
            mTypesetter->setPadding(format.padding[0],format.padding[1],format.padding[2],format.padding[3]);
            mTypesetter->setFontScale(format.scale);
            mTypesetter->setString(data.str);
            mQuotes += mTypesetter->getQuote();
        }

        Vec2i windowSize = app::getWindowSize();

        mOscillator   = new Oscillator();
        mBackground   = new Background(mGrid, areaScaled, mOscillator, windowSize.x, windowSize.y);
        mThemeView    = new ThemeView(mGrid, areaScaled, mOscillator, &mQuotes);
        mSessionView  = new SessionView(sessionData);

#ifndef STAGE_SKIP_LOGO
        mLogoNEXT = new NEXTLogo();
#endif
        /*--------------------------------------------------------------------------------------------*/
        //  Fbo + Post Process
        /*--------------------------------------------------------------------------------------------*/

        gl::Fbo::Format fboFormat_MSAA_4;
        fboFormat_MSAA_4.setSamples(4);

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

        mFboPingPong_1     = PingPongFbo(mFboSize_1.x, mFboSize_1.y, fboFormat_MSAA_4);
        mFboPingPong_2     = PingPongFbo(mFboSize_2.x, mFboSize_2.y, fboFormat_MSAA_4);
        
        mShaderBlurHRef = FxResources::GetBlurH();
        mShaderBlurVRef = FxResources::GetBlurV();
#endif

        mFboThemeView    = gl::Fbo(mFboSize_1.x, mFboSize_1.y, fboFormat_MSAA_4);

#if defined(STAGE_LIVE_EDIT_FX_SHADER) && !defined(STAGE_SKIP_FX_SHADER)
        mFileWatcher = FileWatcher::Get();
        ::util::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_VERT),
                           loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG),
                           &mShaderNormalDepth);
        ::util::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_VERT),
                           loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_FRAG),
                           &mShaderSSAO);
        ::util::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_MIX_VERT),
                           loadFile(RES_ABS_GLSL_WORLD_FX_MIX_FRAG),
                           &mShaderMix);
        ::util::loadShader(loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_VERT),
                           loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_FRAG),
                           &mShaderMixRadial);
#elif !defined(STAGE_SKIP_FX_SHADER)
        ::util::loadShader(app::loadResource(RES_GLSL_WORLD_FX_NORMAL_DEPTH_VERT),
                           app::loadResource(RES_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG),
                           &mShaderNormalDepth);
        ::util::loadShader(app::loadResource(RES_GLSL_WORLD_FX_SSAO_VERT),
                           app::loadResource(RES_GLSL_WORLD_FX_SSAO_FRAG),
                           &mShaderSSAO);
        ::util::loadShader(app::loadResource(RES_GLSL_WORLD_FX_MIX_VERT),
                           app::loadResource(RES_GLSL_WORLD_FX_MIX_FRAG),
                           &mShaderMix);
        ::util::loadShader(app::loadResource(RES_GLSL_WORLD_FX_RADIAL_MIX_VERT),
                           app::loadResource(RES_GLSL_WORLD_FX_RADIAL_MIX_FRAG),
                           &mShaderMixRadial);
#endif
        mThemeView->play(100, NULL);
        //mSessionView->start();
    }

/*--------------------------------------------------------------------------------------------*/
//  Destructor
/*--------------------------------------------------------------------------------------------*/

    Stage::~Stage(){
        delete mBackground;
        delete mThemeView;
        delete mSessionView;
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

    void Stage::drawThemeScene(bool useMaterialShaders){
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

        gl::popMatrices();
        gl::disableDepthRead();
    }

/*--------------------------------------------------------------------------------------------*/
//  Post process theme view
/*--------------------------------------------------------------------------------------------*/

    void Stage::processThemeScene(){

        ////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //  Render View
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////

        mFboThemeView.bindFramebuffer();
        drawThemeScene(true);
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
        drawThemeScene(false);
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
        util::drawClearedScreenRect(mFboSize_2);

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
        mShaderBlurHRef->bind();
        mShaderBlurHRef->uniform("uTexture", 0);
        mShaderBlurHRef->uniform("uTexelSize", mFboTexelSize_2.x);
        mShaderBlurHRef->uniform("uScale", STAGE_FX_SHADER_BLUR_SCALE);
        util::drawClearedScreenRect(mFboSize_2);
        mShaderBlurHRef->unbind();
        mFboPingPong_2.unbindSourceTexture(0);
        mFboPingPong_2.unbindFramebuffer();
        mFboPingPong_2.swap();


        /*--------------------------------------------------------------------------------------------*/
        //  Blur ssao vertical
        /*--------------------------------------------------------------------------------------------*/

        mFboPingPong_2.bindFramebuffer();
        mFboPingPong_2.bindSourceTexture(0);
        mShaderBlurVRef->bind();
        mShaderBlurVRef->uniform("uTexture", 0);
        mShaderBlurVRef->uniform("uTexelSize", mFboTexelSize_2.y);
        mShaderBlurVRef->uniform("uScale", STAGE_FX_SHADER_BLUR_SCALE);
        util::drawClearedScreenRect(mFboSize_2);
        mShaderBlurVRef->unbind();
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
        util::drawClearedScreenRect(mFboSize_1,false);
        mShaderMix.unbind();

        mFboThemeView.getTexture().unbind(1);
        mFboPingPong_2.unbindSourceTexture(0);
        mFboThemeViewSSAO.unbindFramebuffer();


        /*--------------------------------------------------------------------------------------------*/
        //  Render mix horizontal blur
        /*--------------------------------------------------------------------------------------------*/

        mFboPingPong_1.bindFramebuffer();
        mFboThemeViewSSAO.getTexture().bind(0);

        mShaderBlurHRef->bind();
        mShaderBlurHRef->uniform("uTexture", 0);
        mShaderBlurHRef->uniform("uTexelSize", mFboTexelSize_1.x);
        mShaderBlurHRef->uniform("uScale", STAGE_FX_SHADER_BLUR_RADIAL_SCALE);
        util::drawClearedScreenRect(mFboSize_1);
        mShaderBlurHRef->unbind();

        mFboThemeViewSSAO.getTexture().unbind(0);
        mFboPingPong_1.unbindFramebuffer();
        mFboPingPong_1.swap();


        /*--------------------------------------------------------------------------------------------*/
        //  Render blur vertical blur
        /*--------------------------------------------------------------------------------------------*/

        mFboPingPong_1.bindFramebuffer();
        mFboPingPong_1.bindSourceTexture(0);

        mShaderBlurVRef->bind();
        mShaderBlurVRef->uniform("uTexture", 0);
        mShaderBlurVRef->uniform("uTexelSize", mFboTexelSize_1.x);
        mShaderBlurVRef->uniform("uScale", STAGE_FX_SHADER_BLUR_RADIAL_SCALE);
        util::drawClearedScreenRect(mFboPingPong_1.getSize(),false);
        mShaderBlurVRef->unbind();

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
        util::drawClearedScreenRect(mFboThemeViewFinal.getSize());
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


/*--------------------------------------------------------------------------------------------*/
//  Update
/*--------------------------------------------------------------------------------------------*/

    void Stage::update(){
#if defined(STAGE_LIVE_EDIT_FX_SHADER) && !defined(STAGE_SKIP_FX_SHADER)
        ::util::watchShaderSource(mFileWatcher,
                loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_VERT),
                loadFile(RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG),
                &mShaderNormalDepth);
        ::util::watchShaderSource(mFileWatcher,
                loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_VERT),
                loadFile(RES_ABS_GLSL_WORLD_FX_SSAO_FRAG),
                &mShaderSSAO);
        ::util::watchShaderSource(mFileWatcher,
                loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_VERT),
                loadFile(RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_FRAG),
                &mShaderMixRadial);
#endif

        mBackground->update(mOscillator,app::getElapsedSeconds());
        mThemeView->update();
        mSessionView->update();
    }


/*--------------------------------------------------------------------------------------------*/
//  Draw
/*--------------------------------------------------------------------------------------------*/

    void Stage::draw(){

        //  Process theme fx pipe
        processThemeScene();
        //  Draw Scene
        gl::pushMatrices();
        gl::setMatricesWindow(app::getWindowSize(),false);

        gl::draw(mFboThemeViewFinal.getTexture(), mFboBounds_1);

        gl::disableDepthRead();
        gl::setMatricesWindow(app::getWindowSize(), true);

#ifdef DEBUG_THEME_FIELD_QUOTE_MANAGER
        mThemeView->debugDrawQuoteManager();
#endif

        glAlphaFunc(GL_GREATER, 0.0);
        glEnable(GL_ALPHA_TEST);
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        gl::enableAlphaBlending();
        gl::disableDepthRead();
        gl::pushMatrices();
        gl::setMatricesWindow(app::getWindowSize());
        mSessionView->drawLabels();
        mLogoNEXT->draw();
        gl::popMatrices();
        gl::enableDepthRead();
        
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5);;
    
        
        
        gl::enableDepthRead();
        
        gl::popMatrices();
    }


    /*--------------------------------------------------------------------------------------------*/
    //  View
    /*--------------------------------------------------------------------------------------------*/

    void Stage::zoomModelIn(){
        mModelScale = MIN(STAGE_MODEL_SCALE_MIN, mModelScale * 2);
        mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    }

    void Stage::zoomModelOut(){
        mModelScale = MIN(mModelScale / 2, STAGE_MODEL_SCALE_MAX);
        mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
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
    
    /*--------------------------------------------------------------------------------------------*/
    //  get
    /*--------------------------------------------------------------------------------------------*/

}

