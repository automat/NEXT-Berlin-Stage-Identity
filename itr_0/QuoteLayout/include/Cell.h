//
//  Cell.h
//  QuoteLayout
//
//  Created by Henryk Wollik on 12/03/14.
//
//

#ifndef QuoteLayout_Cell_h
#define QuoteLayout_Cell_h

#include <OpenGL/OpenGL.h>
#include "Config.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include "cinder/Utilities.h"
#include "cinder/Camera.h"
#include "cinder/gl/TextureFont.h"
#include "LayoutArea.h"

#include <cassert>

using namespace ci;
using namespace std;

class Cell {
public:
    /*--------------------------------------------------------------------------------------------*/
    // Cell Index
    /*--------------------------------------------------------------------------------------------*/

    class Index{
    private:
        int x,y;
    public:
        Index(const Index& index) : x(index.x), y(index.y){}
        Index(int x = 0,int y = 0) : x(x), y(y){}
        
        inline const int& operator[](int n) const {
            assert( n >= 0 && n <= 1);
            return (&x)[n];
        }
    };
    
    /*--------------------------------------------------------------------------------------------*/

private:
    Index      mIndex;
    Vec3f      mPos;
    LayoutArea mArea;
    
public:
    Cell(const Index& index, const Vec3f& pos) :
        mIndex(index), mPos(pos),mArea(pos,1,1,true){}
    
    inline void debugDrawArea(){
        static const float zero[3] = {
            0,0,0
        };
        static const float verticesArea[12] = {
            -0.5f,0,-0.5f,
             0.5f,0,-0.5f,
             0.5f,0, 0.5f,
            -0.5f,0, 0.5f
        };
        
        glColor3f(0, 0, 0.25f);
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, mPos.z);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &verticesArea[0]);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glVertexPointer(3, GL_FLOAT, 0, &zero[0]);
        glPointSize(10);
        //glDrawArrays(GL_POINTS, 0, 1);
        glPointSize(1);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
    }
    
    inline const LayoutArea& getArea() const{
        return mArea;
    }
    
    inline const Vec3f& getCenter() const{
        return mPos;
    }
    
    inline const Index& getIndex() const {
        return mIndex;
    }
};



#endif
