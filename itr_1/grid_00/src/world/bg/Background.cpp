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
            index_0_0 = i * gridSizeX + j;
            index_1_0 = index_0_0 + 1;
            index_0_1 = index_0_0 + gridSizeX;
            index_1_1 = index_0_1 + 1;
            
            indices += index_0_0, index_1_0, index_1_1;
            indices += index_0_0, index_1_1, index_0_1;
        }
    }
    
    //
    //  A little noise
    //
    float scale  = 0.5f;
    float factor = 0.25f;
    float angle  = 0.0125f;
    i = -1;
    while(++i < vertices.size()){
        Vec3f& vertex = vertices[i];
        vertex.y = osc->getValue(vertex.x * scale, vertex.z * scale, angle) * factor;
    }
    utils::randomSubdivide(vertices, indices, 4, 0.15f);
    mMesh.recalculateNormals();
    
    
    /*--------------------------------------------------------------------------------------------*/
    // setup shaders
    /*--------------------------------------------------------------------------------------------*/

#ifdef BACKGROUND_LIVE_EDIT_SHADER
    mSharedFileWatcher = SharedFileWatcher::Get();
    utils::loadShader(loadFile(RES_ABS_GLSL_PASS_THRU_VERT),
                      loadFile(RES_ABS_GLSL_BG_GRADIENT_FRAG),
                      &mShaderGradient);
#else
    utils::loadShader(LoadResource(RES_GLSL_PASS_THRU_VERT),
                      LoadResource(RES_GLSL_BG_GRADIENT_FRAG),
                      &mShaderGradient);
#endif
    
    gl::Fbo::Format format;
    format.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    format.setSamples(4);
    
    Vec2f windowSize(app::getWindowSize());
    mFboGradient = gl::Fbo(windowSize.x, windowSize.y, format);
    
    renderTexture();
}

/*--------------------------------------------------------------------------------------------*/
// draw / update
/*--------------------------------------------------------------------------------------------*/

void Background::draw(){
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
    //onShaderChanged();
#endif
}


/*--------------------------------------------------------------------------------------------*/
// Texture
/*--------------------------------------------------------------------------------------------*/


void Background::renderTexture(){
    Vec2f windowSize(app::getWindowSize());
    mFboGradient.bindFramebuffer();
    mShaderGradient.bind();
    mShaderGradient.uniform("uScreenWidth", windowSize.x);
    mShaderGradient.uniform("uScreenHeight", windowSize.y);
    mShaderGradient.uniform("uColor0", COLOR_BLUE_0);
    mShaderGradient.uniform("uColor1", COLOR_BLUE_1);
    
    gl::pushMatrices();
    gl::setMatricesWindow(1,1,true);
    gl::clear(Color(0,1,1));
    utils::drawUnitQuad();
    gl::popMatrices();
    
    mShaderGradient.unbind();
    mFboGradient.unbindFramebuffer();
    
    mTexture = mFboGradient.getTexture();
}

#ifdef BACKGROUND_LIVE_EDIT_SHADER
void Background::onShaderChanged(){
    if(utils::shaderDidChange(*mSharedFileWatcher.get(),
                              loadFile(RES_ABS_GLSL_PASS_THRU_VERT),
                              loadFile(RES_ABS_GLSL_BG_GRADIENT_FRAG),
                              &mShaderGradient)){
        renderTexture();
    }
}
#endif

