//
//  Cell.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Cell_h
#define grid_00_Cell_h

#include "world/Index.h"

class Cell {
public:
    
    /*--------------------------------------------------------------------------------------------*/
    //  Index
    /*--------------------------------------------------------------------------------------------*/
    
    
    
    /*--------------------------------------------------------------------------------------------*/
    
private:
    Index mIndex;
    Vec3f         mPos;
    LayoutArea    mArea;
    
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
