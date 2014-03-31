#include "world/board/field/AbstractField.h"

AbstractField::AbstractField(const Vec3f& pos, int size, int numPathSlices) :
    mActive(true),mPos(pos),mSize(size),mSurfaceOffset(0){
        mPathSurface.set(pos,numPathSlices,size);
        
}

AbstractField::~AbstractField(){
    
}

void AbstractField::updatePathSurface(Oscillator* osc, float t){
    mPathSurface.update(osc, mPos + mSurfaceDisplacement, mSurfaceDensity, mSurfaceAmplitude, mSurfaceOffset);
}

void AbstractField::debugDrawArea_Internal(){
    static const Vec3f zero;
    
    float verticesArea[12] = {
        -0.5f, 0,-0.5f,
        0.5f,  0,-0.5f,
        0.5f,  0, mSize - 0.5f,
        -0.5f, 0, mSize - 0.5f
    };
    
    glPushMatrix();
    glTranslatef(mPos.x, mPos.y, mPos.z);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &verticesArea[0]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glVertexPointer(3, GL_FLOAT, 0, &zero.x);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void AbstractField::debugDrawPathSurface(){
    mPathSurface.debugDraw();
}