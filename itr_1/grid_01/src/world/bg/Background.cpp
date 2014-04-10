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
    mOsc = osc;
    mTransform = Matrix44f::createTranslation(Vec3f::yAxis() * -0.5f);
    
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
            indices += index_1_1, index_0_1, index_0_0;
        }
    }
    
    //
    //  A little noise
    //
    float scale  = 0.5f;
    float factor = 0.5f;
    float angle  = 0.0125f;
    
    i = -1;
    while(++i < vertices.size()){
        Vec3f& vertex = vertices[i];
        vertex.y = osc->getValue(vertex.x * scale, vertex.z * scale, angle) * factor;
    }
   
    utils::subdivide(vertices, indices, 2);

    i = -1;
    while(++i < vertices.size()){
        Vec3f& vertex = vertices[i];
        vertex.y = osc->getValue(vertex.x * scale, vertex.z * scale, angle) * factor;
    }

    
    //utils::genUniqueFaces(vertices, indices, normals);
    
    mMesh.recalculateNormals();
    
#ifdef BACKGROUND_LIVE_EDIT_SHADER
    mFileWatcher = FileWatcher::Get();
    utils::loadShader(loadFile(RES_ABS_GLSL_BG_MESH_VERT),
                      loadFile(RES_ABS_GLSL_BG_MESH_FRAG),
                      &mShaderMesh);
#else
    utils::loadShader(app::loadResource(RES_GLSL_BG_MESH_VERT),
                      app::loadResource(RES_GLSL_BG_MESH_FRAG),
                      &mShaderMesh);
#endif
}

void Background::draw(){
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);

    mShaderMesh.bind();
    gl::draw(mMesh);
    mShaderMesh.unbind();
    
    glPopMatrix();
}

void Background::update(Oscillator* osc, float t){
    float scale  = 0.5f;
    float factor = 0.5f;
    float angle  = 0.0125f;
    t *= 0.125f;
    
    vector<Vec3f>& vertices = mMesh.getVertices();
    int i = -1;
    i = -1;
    while(++i < vertices.size()){
        Vec3f& vertex = vertices[i];
        vertex.y = mOsc->getValue(vertex.x * scale, vertex.z * scale + t, angle) * factor;
    }
    mMesh.recalculateNormals();
    
#ifdef BACKGROUND_LIVE_EDIT_SHADER
     utils::watchShaderSource(mFileWatcher,
                                loadFile(RES_ABS_GLSL_BG_MESH_VERT),
                                loadFile(RES_ABS_GLSL_BG_MESH_FRAG),
                              &mShaderMesh);
#endif
}