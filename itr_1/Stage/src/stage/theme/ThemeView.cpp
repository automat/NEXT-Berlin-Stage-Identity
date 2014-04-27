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
#include "quote/QuoteAlign.h"

namespace next{

    using namespace std;
    using namespace boost::assign;
    using namespace ci;

    /*--------------------------------------------------------------------------------------------*/
    // Constructor
    /*--------------------------------------------------------------------------------------------*/

    ThemeView::ThemeView(Grid* grid, const LayoutArea& area, Oscillator* oscillator, vector<Quote>* quotes) :
    AbstractView(grid,area),
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
        
        mQuoteFieldManager = new QuoteFieldManager(mQuotes,mQuoteFields,mGrid);

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
        //mIndexDiverFieldMap.clear();
        while (!mDiverFields.empty()) delete mDiverFields.back(), mDiverFields.pop_back();
    }

    void ThemeView::deleteQuoteFields(){
        //mIndexQuoteFieldMap.clear();
        vector<QuoteField*>& quoteFields = mQuoteFields[0];
        while (!quoteFields.empty()) delete quoteFields.back(), quoteFields.pop_back();
        quoteFields = mQuoteFields[1];
        while (!quoteFields.empty()) delete quoteFields.back(), quoteFields.pop_back();
    }

    ThemeView::~ThemeView(){
        deleteDiverFields();
        deleteQuoteFields();
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
#ifndef THEME_SKIP_DRAW_FIELD_DIVER
        for(vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr){
#ifdef DEBUG_THEME_FIELD_DIVER
            (*itr)->debugDrawArea();
            (*itr)->debugDrawDivers();
            (*itr)->debugDrawPathSurface();
#endif
            (*itr)->draw();
        }
#endif
        if(useMaterialShaders){
            mShaderDiverFields.unbind();
        }

#ifndef THEME_SKIP_DRAW_QUOTE_DIVER
        
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0.0, 0.9);
        
        if(useMaterialShaders){
            mShaderQuoteFields.bind();
            mMaterialQuoteFields.apply();
        }
        
        int i = -1;
        while (++i < 2) {
            const vector<QuoteField*>& quoteFields = mQuoteFields[i];
            //const gl::Texture& quoteTexture = mQuoteFieldManager->getSelectedQuote(i)->getTexture();
            
            if(useMaterialShaders){
              //  quoteTexture.bind();
                mShaderQuoteFields.uniform("uTexture", 0);
            }
            
            for(vector<QuoteField*>::const_iterator itr = quoteFields.begin(); itr != quoteFields.end(); ++itr){
#ifdef DEBUG_THEME_FIELD_QUOTE
                (*itr)->debugDrawArea();
                (*itr)->debugDrawPathSurface();
                (*itr)->debugDrawDivers();
#endif
#ifdef DEBUG_THEME_FIELD_QUOTE_TEXCOORDS
                
                (*itr)->debugDrawDiverIndices(camera);
#endif
                (*itr)->draw();
            }
            if(useMaterialShaders){
              //  quoteTexture.unbind();
              //  quoteTexture.disable();
            }
        }
        
        if(useMaterialShaders){
            mShaderQuoteFields.unbind();
        }
        
        glDisable(GL_POLYGON_OFFSET_FILL);

#endif
    }
#ifdef DEBUG_THEME_FIELD_QUOTE_MANAGER
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
#endif
    

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
#ifndef THEME_SKIP_DRAW_FIELD_DIVER
        for (vector<DiverField*>::const_iterator itr = mDiverFields.begin(); itr != mDiverFields.end(); ++itr) {
            (*itr)->update(mOscillator,t);
        }
#endif
#ifndef BOARD_SKIP_DRAW_FIELD_QUOTE
        int i = -1;
        while (++i < 2) {
            for (vector<QuoteField*>::const_iterator itr = mQuoteFields[i].begin(); itr != mQuoteFields[i].end(); ++itr) {
                (*itr)->update(mOscillator,t);
            }
        }
        mQuoteFieldManager->update();
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

}
