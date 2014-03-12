//
//  QuoteTypesetter.h
//  QuoteLayout
//
//  Created by Henryk Wollik on 12/03/14.
//
//

#ifndef QuoteLayout_QuoteTypesetter_h
#define QuoteLayout_QuoteTypesetter_h
#include <boost/assign/std/vector.hpp>

#include "Config.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Rect.h"
#include "Cell.h"

#include <vector>
#include <string>

using namespace std;
using namespace ci;
using namespace boost::assign;

class QuoteLayoutArea {
    Vec3f mTl,mTr,
          mBl,mBr;
private:
    inline float area(const Vec3f& a, const Vec3f& b, const Vec3f& c){
        return (a-b).cross(a-c).length() * 0.5f;
    }
    
    inline float area(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& d){
        return area(a,b,c) + area(b,d,c);
    }
    
public:
    QuoteLayoutArea(const Vec3f& tl, const Vec3f& tr, const Vec3f& bl, const Vec3f& br) :
        mTl(tl),mTr(tr),
        mBl(bl),mBr(br){}
    
    //! assuming y = 0
    inline bool contains(const Vec3f& point){
        float areaABCD = area(mTl, mTr, mBl, mBr),
              areaAPD  = area(mTl, point, mBr),
              areaDPC  = area(mBr, point, mBl),
              areaCPB  = area(mBl, point, mTr),
              areaPBA  = area(point, mTr, mTl);
        
        return (areaAPD + areaDPC + areaCPB + areaPBA) <= areaABCD;
    }
    
    const Vec3f& getTL(){return mTl;}
    const Vec3f& getTR(){return mTr;}
    const Vec3f& getBL(){return mBl;}
    const Vec3f& getBR(){return mBr;}
};


class QuoteTypesetter {
    gl::TextureFontRef  mTexFontRef;
    vector<Cell*>*      mCells;         // ref cells
    QuoteLayoutArea     mArea;
    vector<int>         mCellsIndex;    // cells indices defined by area
    
    
public:
    QuoteTypesetter(vector<Cell*>* cells, const QuoteLayoutArea& area) :
        mCells(cells),
        mArea(area){
        mTexFontRef = gl::TextureFont::create(Font(FONT_NAME,FONT_SIZE));
            for(auto* cell : *mCells){
                if(mArea.contains(cell->getCenter())){
                    mCellsIndex += cell->getId()[0] * GRID_NUM_XY + cell->getId()[1];
                }
            }
    }
    
    inline void setString(const string& str){
        
        
    }
    
    inline void debugDrawArea(){
        static int indices[] = {0,1,3,2};
        Vec3f vertices[] = {mArea.getTL(),
                            mArea.getTR(),
                            mArea.getBL(),
                            mArea.getBR()};
        
        glColor3f(0, 1, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
        
        glColor3f(1, 1, 1);
        glPointSize(10);
        for (auto& index : mCellsIndex) {
            glVertexPointer(3, GL_FLOAT, 0, &(*mCells)[index]->getCenter().x);
            glDrawArrays(GL_POINTS, 0, 1);
        }
        glPointSize(1);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        
    }
    
    inline void draw(){
        
    }
    
    inline void update(){
        
    }
    
    
    
    
    
    
    
    
};



#endif
