#include "world/bg/Background.h"

#include <OpenGL/OpenGL.h>
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "cinder/app/App.h"

#include "cinder/Vector.h"

#include "Resources.h"
#include "Colors.h"

#include "util/GeomUtil.h"
#include "util/ShaderUtil.h"
#include "util/DrawUtil.h"

using namespace boost::assign;

Background::Background(Grid* grid, const LayoutArea& area, Oscillator* osc, int width, int height){
    mTransform = Matrix44f::createTranslation(Vec3f::yAxis() * -0.05f);
    
    /*--------------------------------------------------------------------------------------------*/
    // setup mesh
    /*--------------------------------------------------------------------------------------------*/

    vector<Vec3f>&    vertices = mMesh.getVertices();
    vector<uint32_t>& indices  = mMesh.getIndices();
    vector<Vec3f>&    normals  = mMesh.getNormals();
    
    const vector<Cell*>& gridCells = grid->getCells();
    for(vector<Cell*>::const_iterator itr = gridCells.begin(); itr != gridCells.end(); ++itr){
        vertices += (*itr)->getCenter();
        normals  += Vec3f();
    }
    
    int gridSizeX = grid->getNumCellsX();
    int gridSizeY = grid->getNumCellsY();
    int j, i = -1;
    int index_0_0, index_1_0, index_0_1, index_1_1;
    while (++i < gridSizeY - 1) {
        j = -1;
        while(++j < gridSizeX - 1 ){
            index_0_0 = i * gridSizeX + j;          // 0
            index_1_0 = index_0_0 + 1;              // 1
            index_1_1 = index_0_0 + gridSizeX + 1;  // 2
            index_0_1 = index_0_0 + gridSizeX;      // 3
            
            
            indices += index_0_0, index_1_0, index_1_1;
            //indices += index_0_0, index_1_1, index_0_1;
            indices += index_1_1, index_0_1, index_0_0;
            //indices += index_0_1, index_1_1, index_0_0;
        }
    }
    //
    //  A little noise
    //
    float scale  = 0.5f;
    float factor = 0.25f;
    float angle  = 0.0125f;
    
    utils::subdivide(vertices, indices, 1);
    
    i = -1;
    while(++i < vertices.size()){
        Vec3f& vertex = vertices[i];
        vertex.y = osc->getValue(vertex.x * scale, vertex.z * scale, angle) * factor;
    }
    
    utils::genUniqueFaces(vertices, indices, normals);
    
    mMesh.recalculateNormals();
    
    /*--------------------------------------------------------------------------------------------*/
    // setup shaders
    /*--------------------------------------------------------------------------------------------*/

#ifdef BACKGROUND_LIVE_EDIT_SHADER
    mSharedFileWatcher = SharedFileWatcher::Get();
    utils::loadShader(loadFile(RES_ABS_GLSL_PASS_THRU_VERT),
                      loadFile(RES_ABS_GLSL_BG_GRADIENT_FRAG),
                      &mShaderGradient);
    utils::loadShader(loadFile(RES_ABS_GLSL_BG_MESH_VERT),
                      loadFile(RES_ABS_GLSL_BG_MESH_FRAG),
                      &mShaderMesh);
    utils::loadShader(loadFile(RES_ABS_GLSL_PASS_THRU_VERT),
                      loadFile(RES_ABS_GLSL_BG_MIX_FRAG),
                      &mShaderMix);
#else
    utils::loadShader(LoadResource(RES_GLSL_PASS_THRU_VERT),
                      LoadResource(RES_GLSL_BG_GRADIENT_FRAG),
                      &mShaderGradient);
    utils::loadShader(LoadResource(RES_GLSL_BG_MESH_VERT),
                      LoadResource(RES_GLSL_BG_MESH_FRAG),
                      &mShaderMesh);
    utils::loadShader(LoadResource(RES_GLSL_PASS_THRU_VERT),
                      LoadResource(RES_GLSL_BG_MIX_FRAG),
                      &mShaderMix);
#endif
    
    gl::Fbo::Format format;
    format.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    format.setSamples(4);
    
    Vec2f windowSize(app::getWindowSize());
    mFboGradient = gl::Fbo(windowSize.x, windowSize.y, format);
    mFboMesh     = gl::Fbo(windowSize.x, windowSize.y, format);
    mFboMix      = gl::Fbo(windowSize.x, windowSize.y, format);
    
    mTextureIsDirty = true;
    renderGradient();
    
}

/*--------------------------------------------------------------------------------------------*/
// draw / update
/*--------------------------------------------------------------------------------------------*/

void Background::draw(){
    if(mTextureIsDirty){
        //renderGradient();
        renderMesh();
        renderGradient();
        renderTexture();
        
        mTextureIsDirty = false;
    }
    glColor3f(1,1,1);
    
    const static Vec2f windowSize(app::getWindowSize());
    gl::disableDepthRead();
    
    gl::pushMatrices();
    gl::setMatricesWindow(windowSize.x, windowSize.y, true);
    gl::draw(mTexture, app::getWindowBounds());
    gl::popMatrices();
    
    gl::enableDepthRead();
}

void Background::update(){
#ifdef BACKGROUND_LIVE_EDIT_SHADER
    
    if(utils::shaderDidChange(mSharedFileWatcher,
                              loadFile(RES_ABS_GLSL_PASS_THRU_VERT),
                              loadFile(RES_ABS_GLSL_BG_GRADIENT_FRAG),
                              &mShaderGradient) ||
       utils::shaderDidChange(mSharedFileWatcher,
                              loadFile(RES_ABS_GLSL_BG_MESH_VERT),
                              loadFile(RES_ABS_GLSL_BG_MESH_FRAG),
                              &mShaderMesh) ||
       utils::shaderDidChange(mSharedFileWatcher,
                              loadFile(RES_ABS_GLSL_PASS_THRU_VERT),
                              loadFile(RES_ABS_GLSL_BG_MIX_FRAG),
                              &mShaderMix)){
        mTextureIsDirty = true;
    }
#endif
}


/*--------------------------------------------------------------------------------------------*/
// Texture
/*--------------------------------------------------------------------------------------------*/


void Background::renderGradient(){
    mFboGradient.bindFramebuffer();
    mFboMesh.bindTexture(1);
    mShaderGradient.bind();
    mShaderGradient.uniform("uScreenSize", Vec2f(app::getWindowWidth(),app::getWindowHeight()));
    mShaderGradient.uniform("uColor0", COLOR_BLUE_0);
    mShaderGradient.uniform("uColor1", COLOR_BLUE_1);
    mShaderGradient.uniform("uTexture",1);;
    gl::pushMatrices();
    gl::setMatricesWindow(1,1,true);
    gl::clear(Color::white());
    utils::drawUnitQuad();
    gl::popMatrices();
    mShaderGradient.unbind();
    mFboMesh.unbindTexture();
    mFboGradient.unbindFramebuffer();
}

void Background::renderMesh(){
    mFboMesh.bindFramebuffer();
    mShaderMesh.bind();
    mShaderMesh.uniform("uColor0", COLOR_BLUE_0);
    mShaderMesh.uniform("uColor1", COLOR_BLUE_1);
    mShaderMesh.uniform("uColor2", COLOR_BLUE_2);
    gl::clear(Color::black());
    gl::draw(mMesh);
    mShaderMesh.unbind();
    mFboMesh.unbindFramebuffer();
}

void Background::renderTexture(){
    mTexture = mFboGradient.getTexture();
}
