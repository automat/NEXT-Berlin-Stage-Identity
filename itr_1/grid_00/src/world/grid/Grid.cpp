#include "world/grid/Grid.h"

#include <boost/assign/std/vector.hpp>
#include <cassert>

#include "Resources.h"
#include "util/SharedTextureFont.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace boost::assign;

Grid::Grid(int numCellsX, int numCellsY) :
mSize(numCellsX,numCellsY){
    int size_2 = mSize.x / 2;
    int i,j;
    
    
    i = -1;
    while (++i < mSize.y) {
        j = -1;
        while(++j < mSize.x){
            mCells += new Cell(Index(j,i),Vec3f(-size_2 + j, 0, -size_2 + i));
        }
    }
    /*
    i = mSize.y;
    while(--i > -1){
        j = mSize.x;
        while(--j > -1){
            mCells += new Cell(Index(j,i),Vec3f(-size_2 + j, 0, -size_2 + i));
        }
    }*/
}

Grid::~Grid(){
    while (!mCells.empty()) {
        delete mCells.back();
        mCells.pop_back();
    }
}

Cell* Grid::getCell(int n){
    assert( n > -1 && n < mCells.size());
    return mCells[n];
}

const Cell* Grid::getCell(int n) const{
    return getCell(n);
}

Cell* Grid::getCell(int x, int y){
    assert( x > -1 && x < mSize.x && y > -1 && y < mSize.y);
    return mCells[ y * mSize.x + x ];
}

Cell* Grid::getCell(const Index& index){
    return getCell(index[0], index[1]);
}


void Grid::debugDrawIndices(const CameraOrtho& camera){
    static const float fontScale = 0.005f;
    
    Vec3f v;
    Vec3f w;
    Vec3f u;
    
    camera.getBillboardVectors(&w, &u);
    v = w.cross(u);
    
   const static Vec2f zero;
    
    Matrix44f mat;
    Matrix44f rot = Matrix44f::createRotationOnb(u,w,v);
    rot*= Matrix44f::createRotation(Vec3f::zAxis(), M_PI_2);
    rot*= Matrix44f::createScale(Vec3f(fontScale,fontScale,fontScale));
    
    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    
    for(auto* cell : mCells){
        const Index& index = cell->getIndex();
        
        mat.setToIdentity();
        mat *= Matrix44f::createTranslation(cell->getCenter());
        mat *= rot;
        
        glColor3f(0.65f,0.65f,0.65f);
        glPushMatrix();
        glMultMatrixf(mat);
        SharedTextureFont::Get()->drawString(toString(index[0]) + "," + toString(index[1]), zero);
        glColor3f(0.85f, 0, 0);
        SharedTextureFont::Get()->drawString(toString(index[1] * mSize.x + index[0]), Vec2f(0,20));
        glPopMatrix();
    }
    
    gl::disableAlphaBlending();
    gl::disableAlphaTest();

}
