//
//  Grid.h
//  QuoteLayout
//
//  Created by Henryk Wollik on 17/03/14.
//
//

#ifndef QuoteLayout_Grid_h
#define QuoteLayout_Grid_h

#include <vector>
#include <boost/assign/std/vector.hpp>
#include <cassert>
#include "Cell.h"
#include "cinder/Vector.h"

using namespace std;
using namespace boost::assign;
using namespace ci;

class Grid {
private:
    Vec2f mSize;
    vector<Cell*> mCells;
    vector<int*> mTest;
    
    // No copy
    Grid();
    Grid(const Grid& other);
    Grid& operator=(const Grid& rhs);
    
public:
    Grid(int numCellsX, int numCellsY) :
        mSize(numCellsX,numCellsY){
            int size_2 = mSize.x / 2;
            int i,j;
            i = -1;
            while (++i < mSize.y) {
                j = -1;
                while(++j < mSize.x){
                    mCells.push_back(new Cell(Cell::Index(j,i),Vec3f(-size_2 + j, 0, -size_2 + i)));
                }
            }
    }
    
    ~Grid(){
        while (!mCells.empty()) {
            delete mCells.back();
            mCells.pop_back();
        }
    }
    
    inline const Cell* getCell(int n) const{
        assert( n > -1 && n < mCells.size());
        return mCells[n];
    }
    
    inline Cell* getCell(int n){
        assert( n > -1 && n < mCells.size());
        return mCells[n];
    }
    
    inline Cell* getCell(int x, int y){
        assert( x > -1 && x < mSize.x && y > -1 && y < mSize.y);
        return mCells[ y * mSize.x + x ];
    }
    
    inline Cell* getCell(const Cell::Index& index){
        return getCell(index[0], index[1]);
    }
    
    inline const vector<Cell*>& getCells() const{
        return mCells;
    }
    
    inline int getNumCellsX() const{
        return mSize.x;
    }
    
    inline int getNumCellsX(){
        return mSize.x;
    }
    
    inline int getNumCellsY() const{
        return mSize.y;
    }
    
    inline int getNumCellsY(){
        return mSize.y;
    }
    
    
    inline void debugDrawIndices(const CameraOrtho& camera){
        static const float fontScale = 0.005f;
        
        Vec3f v;
        Vec3f w;
        Vec3f u;
        
        camera.getBillboardVectors(&w, &u);
        v = w.cross(u);
        
        const static gl::TextureFontRef debugFont = gl::TextureFont::create(ci::Font("Apercu Mono",18));
        const static Vec2f zero;
        
        Matrix44f mat;
        Matrix44f rot = Matrix44f::createRotationOnb(u,w,v);
                  rot*= Matrix44f::createRotation(Vec3f::zAxis(), M_PI_2);
                  rot*= Matrix44f::createScale(Vec3f(fontScale,fontScale,fontScale));
        
        gl::enableAlphaTest();
        gl::enableAlphaBlending();
        
        for(auto* cell : mCells){
            const Cell::Index& index = cell->getIndex();
            
            mat.setToIdentity();
            mat *= Matrix44f::createTranslation(cell->getCenter());
            mat *= rot;

            glColor3f(0.65f,0.65f,0.65f);
            glPushMatrix();
            glMultMatrixf(mat);
            debugFont->drawString(toString(index[0]) + "," + toString(index[1]), zero);
            glColor3f(0.85f, 0, 0);
            debugFont->drawString(toString(index[1] * mSize.x + index[0]), Vec2f(0,20));
            glPopMatrix();
        }
        
        gl::disableAlphaBlending();
        gl::disableAlphaTest();
    }
};



#endif
