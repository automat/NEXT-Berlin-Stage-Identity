#include "world/board/field/QuoteField.h"
#include "Resources.h"

#include "cinder/gl/TextureFont.h"
#include "cinder/Utilities.h"

QuoteField::QuoteField(const Vec3f& pos, int numPathSlices, QuoteLine* quoteLine) :
AbstractField(pos,numPathSlices,quoteLine->getIndices().size()){
    reset(pos, numPathSlices, quoteLine);
    
    //set mesh layout texcoords here
}

void QuoteField::reset(const Vec3f &pos, int numPathSlices, QuoteLine* quoteLine){
    mPos = pos;
    mSurfaceNumSlices = numPathSlices;
    
    mTransform.setToIdentity();
    mTransform.translate(pos);
    
    reset_Internal();
    
    
    
    // parse quote here
}


void QuoteField::draw(){
    
}

void QuoteField::update(Oscillator *osc, float t){
    
}


void QuoteField::debugDrawIndices(const cinder::CameraOrtho &camera){
    static const float fontScale = 0.005f;
    
    Vec3f v;
    Vec3f w;
    Vec3f u;
    
    camera.getBillboardVectors(&w, &u);
    v = w.cross(u);
    
    const static gl::TextureFontRef debugFont = gl::TextureFont::create(ci::Font(app::loadResource(RES_FONT_APERCU_MONO),18));
    const static Vec2f zero;

    Matrix44f mat;
    Matrix44f rot = Matrix44f::createRotationOnb(u,w,v);
    rot*= Matrix44f::createRotation(Vec3f::zAxis(), M_PI_2);
    rot*= Matrix44f::createScale(Vec3f(fontScale,fontScale,fontScale));
    
    gl::VboMesh::VertexIter vbItr = mMesh.mapVertexBuffer();

    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    
    int i = -1;
    while (++i < mMeshVerticesLen) {
        mat.setToIdentity();
        mat *= Matrix44f::createTranslation(*(vbItr.getPositionPointer()));
        
        glPushMatrix();
        glMultMatrixf(&mat[0]);
        debugFont->drawString(toString(i), zero);
        glPopMatrix();
        
    }
    
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
}


