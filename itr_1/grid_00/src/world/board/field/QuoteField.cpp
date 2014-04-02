#include "world/board/field/QuoteField.h"
#include "Resources.h"

#include "cinder/app/App.h"
#include "cinder/gl/Texture.h"
#include "util/SharedTextureFont.h"
#include "cinder/Utilities.h"

#include "Config.h"


QuoteField::QuoteField(const Vec3f& pos, int numPathSlices, QuoteLine* quoteLine) :
AbstractField(pos,numPathSlices,quoteLine->getIndices().size()){
    mSurfaceDisplacement = Vec3f(Rand::randInt(0, WORLD_GRID_NUM_CELLS_XY),0,Rand::randInt(0, WORLD_GRID_NUM_CELLS_XY));
    mSurfaceAmplitude    = QUOTE_FIELD_SURFACE_PATH_AMPLITUDE;
    mSurfaceDensity      = QUOTE_FIELD_SURFACE_PATH_DENSITY;
    mSurfaceOffsetSpeed  = QUOTE_FIELD_SURFACE_PATH_OFFSET_SPEED;
    
    mDiverUnitNumPoints = QUOTE_FIELD_DIVER_NUM_POINTS;
    mDiverHeightMin     = QUOTE_FIELD_DIVER_MIN_HEIGHT;
    mDiverHeightMax     = QUOTE_FIELD_DIVER_MAX_HEIGHT;
    mDiverOffsetMin     = QUOTE_FIELD_DIVER_MIN_OFFSET;
    mDiverOffsetMax     = QUOTE_FIELD_DIVER_MAX_OFFSET;
    mDiverSpeedMin      = QUOTE_FIELD_DIVER_MIN_SPEED;
    mDiverSpeedMax      = QUOTE_FIELD_DIVER_MAX_SPEED;
    mDiverLengthMin     = QUOTE_FIELD_DIVER_MIN_LENGTH;
    mDiverLengthMax     = QUOTE_FIELD_DIVER_MAX_LENGTH;
    
    mMeshLayout.setDynamicTexCoords2d();
    
    reset(pos, numPathSlices, quoteLine);

}

/*--------------------------------------------------------------------------------------------*/
//  Reset
/*--------------------------------------------------------------------------------------------*/

void QuoteField::reset(const Vec3f &pos, int numPathSlices, QuoteLine* quoteLine){
    mPos              = pos;
    mSurfaceNumSlices = numPathSlices;
    mSize             = quoteLine->getIndices().size();
    
    mTransform.setToIdentity();
    mTransform.translate(pos);
    
    reset_Internal();
    
    
    
    // parse quote here
}

/*--------------------------------------------------------------------------------------------*/
// Draw / Update
/*--------------------------------------------------------------------------------------------*/


void QuoteField::updateDivers(){
    for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
        (*itr)->update();
        (*itr)->updateTexcoords();
    }
}

void QuoteField::draw(){
    static const gl::Texture testTex2x2 =  loadImage( app::loadResource( "test_texture_1024x1024.jpg" ) );
    
    testTex2x2.enableAndBind();
    drawMesh();
    testTex2x2.unbind();
}

void QuoteField::update(Oscillator *osc, float t){
    mSurfaceOffset += mSurfaceOffsetSpeed;
    updatePathSurface(osc, mSurfaceOffset);
    updateDivers();
    updateMeshTexcoords();
    updateMesh();
}

void QuoteField::updateMeshTexcoords(){
    const static Vec2f zero;
    Vec2f tex_0(0,0);   //upper tex coord
    Vec2f tex_1(0,1);   //lower tex coord
    
    int i;
    
    gl::VboMesh::VertexIter vbItr = mMesh.mapVertexBuffer();
    for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
        const vector<float>& texcoords = (*itr)->getTexcoords();
        
        i = -1;
        while (++i < mDiverNumPoints) {
            tex_0.x = tex_1.x = texcoords[i];       //  get sliced hotizontal
            
            ++vbItr; ++vbItr;                       //  skip bottom
            vbItr.setTexCoord2d0(tex_0); ++vbItr;   //  top
            vbItr.setTexCoord2d0(tex_1); ++vbItr;
            ++vbItr; ++vbItr; ++vbItr; ++vbItr;     //  skip top / bottom left / right
        }
        
        ++vbItr; ++vbItr; ++vbItr; ++vbItr;         //  skip front
        ++vbItr; ++vbItr; ++vbItr; ++vbItr;         //  skip back
    }
}


/*--------------------------------------------------------------------------------------------*/
//  Debug draw
/*--------------------------------------------------------------------------------------------*/

void QuoteField::debugDrawArea(){
    glColor3f(1,0,1);
    debugDrawArea_Internal();
}

void QuoteField::debugDrawDiverIndices(const cinder::CameraOrtho &camera){
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
        (*itr)->debugDrawIndices(camera);
    }
    glPopMatrix();
}

void QuoteField::debugDrawIndices(const cinder::CameraOrtho &camera){
}


