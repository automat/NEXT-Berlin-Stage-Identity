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

using namespace boost::assign;

Background::Background(Grid* grid, const LayoutArea& area, Oscillator* osc, int width, int height){
    mTransform = Matrix44f::createTranslation(Vec3f::yAxis() * -0.05f);
    
    //
    //  Setup grid
    //
    vector<Vec3f>&    vertices = mMesh.getVertices();
    vector<uint32_t>& indices  = mMesh.getIndices();
    vector<Color>&    colors   = mMesh.getColorsRGB();
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

    //
    //  Setup material
    //
    mMaterial.setAmbient(COLOR_BLUE_0);
    mMaterial.setDiffuse(COLOR_BLUE_1);
    
    //
    //  Setup gradient
    //
    gl::Fbo::Format gradientFboFormat;
    gradientFboFormat.setSamples(4);
    gradientFboFormat.setWrapS(GL_CLAMP_TO_EDGE);
    gradientFboFormat.setWrapT(GL_CLAMP_TO_EDGE);

    mGradientFbo  = gl::Fbo(app::getWindowWidth(),app::getWindowHeight(),gradientFboFormat);
    
    
    
    //mNormalShader = gl::GlslProg(app::loadResource(RES_GLSL_NORMAL_VERT),app::loadResource(RES_GLSL_NORMAL_FRAG));
    
    
    
    
    
#ifdef BACKGROUND_LIVE_EDIT_SHADER
    mSharedFileWatcher = SharedFileWatcher::Get();
    mSharedFileWatcher->addFile(RES_ABS_GLSL_BG_GRADIENT_FRAG);
    mSharedFileWatcher->addFile(RES_ABS_GLSL_BG_GRADIENT_VERT);
    utils::loadShader(loadFile(RES_ABS_GLSL_BG_GRADIENT_VERT),loadFile(RES_ABS_GLSL_BG_GRADIENT_FRAG), &mGradientShader);
#else
    utils::loadShader(LoadResource(RES_GLSL_BG_GRADIENT_VERT),LoadResource(RES_GLSL_BG_GRADIENT_FRAG), &mGradientShader);
#endif
    
   

}

void Background::drawGradientFbo(){
    
}

void Background::draw(){
    const static Vec2f windowSize(app::getWindowSize());
    gl::disableDepthRead();
    gl::pushMatrices();
    gl::setMatricesWindow(windowSize.x, windowSize.y);
    
    gl::popMatrices();
    gl::enableDepthRead();
    
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    glColor3f(1, 1, 1);
    gl::draw(mMesh);
    glPopMatrix();
}

void Background::update(){
#ifdef BACKGROUND_LIVE_EDIT_SHADER
    if(mSharedFileWatcher->fileDidChange(RES_ABS_GLSL_BG_GRADIENT_FRAG) ||
       mSharedFileWatcher->fileDidChange(RES_ABS_GLSL_BG_GRADIENT_VERT)){
        if(utils::reloadShader(&mGradientShader, RES_ABS_GLSL_BG_GRADIENT_VERT, RES_ABS_GLSL_BG_GRADIENT_FRAG)){
            onGradientShaderChanged();
        };
        
    }
#endif
    
}

#ifdef BACKGROUND_LIVE_EDIT_SHADER
void Background::onGradientShaderChanged(){
    drawGradientFbo();
}
#endif