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

using namespace ci;
using namespace std;

class Cell {
    int        mId[2];
    Vec3f      mPos;
    LayoutArea mArea;
    
    
    
public:
    Cell(int* id, const Vec3f& pos) :
        mPos(pos),mArea(pos,1,1,true){
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
        
        static const float fontScale = 0.005f;
        
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
        
        const static gl::TextureFontRef debugFont = gl::TextureFont::create(Font("Apercu Mono",18));
        
        
        gl::enableAlphaTest();
        gl::enableAlphaBlending();
        glColor3f(0.65f,0.65f,0.65f);
        glPushMatrix();
        glMultMatrixf(mat);
        debugFont->drawString(toString(mId[0]) + "," + toString(mId[1]), Vec2f::zero());
        glColor3f(0.85f, 0, 0);
        debugFont->drawString(toString(mId[1] * GRID_NUM_XY + mId[0]), Vec2f(0,20));
        glPopMatrix();
        gl::disableAlphaBlending();
        gl::disableAlphaTest();
    }
    
    inline const LayoutArea& getArea() const{
        return mArea;
    }
    
    inline const Vec3f& getCenter() {
        return mPos;
    }
    
    inline const int (&getId())[2] {
        return mId;
    }
    
    inline const int (&getId() const)[2]  {
        return mId;
    }
};



#endif
