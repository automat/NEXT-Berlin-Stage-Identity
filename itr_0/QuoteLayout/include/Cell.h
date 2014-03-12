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
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include "cinder/Utilities.h"
#include "cinder/Camera.h"

using namespace ci;

class Cell {
    int   mId[2];
    Vec3f mPos;
    Rectf mRect;
    
    
    
public:
    Cell(int* id, const Vec3f& pos) :
        mPos(pos),
        mRect(pos.x-0.5f, pos.y-0.5f, pos.x+0.5f, pos.y+0.5f){
            mId[0] = id[0];
            mId[1] = id[1];
    
    }
    
    inline void debugDrawArea(const CameraOrtho& camera){
        static const float zero[3] = {
            0,0,0
        };
        static const float verticesArea[12] = {
            -0.5f,0,-0.5f,
             0.5f,0,-0.5f,
             0.5f,0, 0.5f,
            -0.5f,0, 0.5f
        };
        
        glColor3f(0, 0, 1);
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
        
        static const float fontScale = 0.0125f;
        
        Vec3f v;
        Vec3f w;
        Vec3f u;
        
        camera.getBillboardVectors(&w, &u);
        v = w.cross(u);
        
        
        Matrix44f mat;
        mat *= Matrix44f::createTranslation(mPos);
        mat *= Matrix44f::createRotationOnb(u, w, v);
        mat *= Matrix44f::createRotation(Vec3f::zAxis(), M_PI_2);
        mat *= Matrix44f::createScale(Vec3f(fontScale,fontScale,fontScale));
        
        gl::enableAlphaTest();
        gl::enableAlphaBlending();
        glColor3f(1, 1, 1);
        glPushMatrix();
        glMultMatrixf(mat);
        gl::drawString(toString(mId[0]) + " , " + toString(mId[1]), Vec2f::zero());
        glPopMatrix();
        gl::disableAlphaBlending();
        gl::disableAlphaTest();
    }
    
    inline const Rectf& getBounds() const{
        return mRect;
    }
    
    inline const Vec3f& getCenter() {
        return mPos;
    }
    
    inline const int (&getId())[2] {
        return mId;
    }
};



#endif
