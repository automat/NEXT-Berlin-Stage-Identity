#include "stage/theme/ThemeView.h"

#include <iostream>
#include <vector>

#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assert.hpp>

#include "Resources.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Config.h"
#include "util/gl/ShaderUtil.h"
#include "util/ExcInfoPanel.h"
#include "data/quote/QuoteAlign.h"

namespace next{

    using namespace std;
    using namespace boost::assign;
    using namespace ci;

    /*--------------------------------------------------------------------------------------------*/
    // Constructor
    /*--------------------------------------------------------------------------------------------*/

    ThemeView::ThemeView(Grid* grid, const LayoutArea& area, Oscillator* oscillator, vector<Quote>* quotes) :
    AbstractView(),
    mGrid(grid), mArea(area),
    mOscillator(oscillator),
    mQuotes(quotes){

        const vector<Cell*>& gridCells = mGrid->getCells();
        // Create diverfields according to layoutarea
        for(vector<Cell*>::const_iterator itr = gridCells.begin(); itr != gridCells.end(); ++itr){
            const Cell* cell = *itr;
            const Vec3f& pos =cell->getCenter();
            if(area.contains(pos)){
                //const Index& index = cell->getIndex();
                mDiverFields += new DiverField(pos,Rand::randInt(DIVER_FIELD_NUM_DIVERS_MIN, DIVER_FIELD_NUM_DIVERS_MAX));
                //mIndexDiverFieldMap[index] = mDiverFields.back();
            }
        }

        loadMaterialProperties();
        
        mQuoteFieldManager = new QuoteFieldManager(mQuotes,mGrid);

#ifdef THEME_LIVE_EDIT_MATERIAL_SHADER
        mFileWatcher = FileWatcher::Get();
        ::util::loadShader(loadFile(RES_ABS_GLSL_BOARD_QUOTE_FIELD_VERT),
                loadFile(RES_ABS_GLSL_BOARD_QUOTE_FIELD_FRAG),
                &mShaderQuoteFields);
        ::util::loadShader(loadFile(RES_ABS_GLSL_BOARD_DIVER_FIELD_VERT),
                loadFile(RES_ABS_GLSL_BOARD_DIVER_FIELD_FRAG),
                &mShaderDiverFields);
#else
            util::loadShader(LoadResource(RES_GLSL_BOARD_QUOTE_FIELD_VERT),
                              LoadResource(RES_GLSL_BOARD_QUOTE_FIELD_FRAG),
                              &mShaderQuoteFields);
            util::loadShader(LoadResource(RES_GLSL_BOARD_DIVER_FIELD_VERT),
                              LoadResource(RES_GLSL_BOARD_DIVER_FIELD_FRAG),
                              &mShaderQuoteDivers);
#endif
    }

    /*--------------------------------------------------------------------------------------------*/
    // Destructor
    /*--------------------------------------------------------------------------------------------*/

    void ThemeView::deleteDiverFields(){
        while (!mDiverFields.empty()) delete mDiverFields.back(), mDiverFields.pop_back();
    }
    ThemeView::~ThemeView(){
        deleteDiverFields();
        delete mQuoteFieldManager;
    }


    /*--------------------------------------------------------------------------------------------*/
    // Load properties
    /*--------------------------------------------------------------------------------------------*/

    void ThemeView::loadMaterialProperties(){
        mMaterialDiverFields.setAmbient(  DIVER_FIELD_MATERIAL_AMBIENT);
        mMaterialDiverFields.setDiffuse(  DIVER_FIELD_MATERIAL_DIFFUSE);
        mMaterialDiverFields.setSpecular( DIVER_FIELD_MATERIAL_SPECULAR);
        mMaterialDiverFields.setShininess(DIVER_FIELD_MATERIAL_SHININESS);

        mMaterialQuoteFields.setAmbient(  QUOTE_FIELD_MATERIAL_AMBIENT);
        mMaterialQuoteFields.setDiffuse(  QUOTE_FIELD_MATERIAL_DIFFUSE);
        mMaterialQuoteFields.setSpecular( QUOTE_FIELD_MATERIAL_SPECULAR);
        mMaterialQuoteFields.setShininess(QUOTE_FIELD_MATERIAL_SHININESS);
    }
    

    /*--------------------------------------------------------------------------------------------*/
    // Draw
    /*--------------------------------------------------------------------------------------------*/

    void ThemeView::draw(const CameraOrtho& camera, bool useMaterialShaders){
        gl::disableDepthRead();
        gl::enableAlphaTest();
        gl::enableAlphaBlending();
        for(vector<DiverField*>::const_iterator itr =mDiverFields.begin(); itr != mDiverFields.end(); ++itr){
            (*itr)->drawSurface();
        }
        gl::disableAlphaBlending();
        gl::disableAlphaTest();
        gl::enableDepthRead();

        if(useMaterialShaders){
            mShaderDiverFields.bind();
            mMaterialDiverFields.apply();
        }

        for(vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr){
            (*itr)->draw();
        }

        if(useMaterialShaders){
            mShaderDiverFields.unbind();
        }

        if(mQuoteFieldManager->isActive()){
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(0.0, 0.9);
            
            if(useMaterialShaders){
                mShaderQuoteFields.bind();
                mMaterialQuoteFields.apply();
            }
            const gl::Texture& quoteTexture = mQuoteFieldManager->getTexture();
            
            if(useMaterialShaders){
                quoteTexture.bind();
                mShaderQuoteFields.uniform("uTexture", 0);
            }
            
            mQuoteFieldManager->draw(camera);
            
            if(useMaterialShaders){
                quoteTexture.unbind();
                quoteTexture.disable();
            }
            
            if(useMaterialShaders){
                mShaderQuoteFields.unbind();
            }
            
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
    }

    void ThemeView::debugDrawQuoteManager(){
        glAlphaFunc(GL_GREATER, 0.0);
        glEnable(GL_ALPHA_TEST);
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        mQuoteFieldManager->debugDraw();
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5); // clearStates what seems to be cinders default
    }
    

    /*--------------------------------------------------------------------------------------------*/
    // Update
    /*--------------------------------------------------------------------------------------------*/

    void ThemeView::update(){
#ifdef THEME_LIVE_EDIT_MATERIAL_SHADER
        ::util::watchShaderSource(mFileWatcher,
                loadFile(RES_ABS_GLSL_BOARD_QUOTE_FIELD_VERT),
                loadFile(RES_ABS_GLSL_BOARD_QUOTE_FIELD_FRAG),
                &mShaderQuoteFields);
        ::util::watchShaderSource(mFileWatcher,
                loadFile(RES_ABS_GLSL_BOARD_DIVER_FIELD_VERT),
                loadFile(RES_ABS_GLSL_BOARD_DIVER_FIELD_FRAG),
                &mShaderDiverFields);
#endif
        float t = app::getElapsedSeconds();
        for (vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr) {
            (*itr)->update(mOscillator,t);
        }

#ifndef BOARD_SKIP_DRAW_FIELD_QUOTE
        mQuoteFieldManager->update(mOscillator, t);
#endif
        
    }

    /*--------------------------------------------------------------------------------------------*/
    // Config Change Handling
    /*--------------------------------------------------------------------------------------------*/

    void ThemeView::onConfigDidChange(){
        loadMaterialProperties();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // get
    /*--------------------------------------------------------------------------------------------*/

    void ThemeView::play(int num, const std::function<void()>& callbackUpdate, const std::function<void()>& callbackFinish){
        mQuoteFieldManager->play(num, callbackUpdate, callbackFinish);
    }
    
    int ThemeView::getQuoteIndex(){
        return mQuoteFieldManager->getQuoteIndex();
    }
    
    size_t ThemeView::getNumQuotes(){
        return mQuoteFieldManager->getNumQuotes();
    }
}
