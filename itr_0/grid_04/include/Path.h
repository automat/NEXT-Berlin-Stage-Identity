//
//  GridDiverPath.h
//  grid_02
//
//  Created by Henryk Wollik on 25/02/14.
//
//

#ifndef grid_02_GridDiverPath_h
#define grid_02_GridDiverPath_h

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>

#include <math.h>

using namespace std;
using namespace ci;


class Path {
    vector<Vec3f> mPoints;
    
public:
    static const int PATH_RES = 20;
    
    Path(Vec3f start, Vec3f end){
        mPoints.push_back(start);
        int i = 0;
        float a;
        while (++i < PATH_RES - 1) {
            a = float(i)/float(PATH_RES-1);
            mPoints.push_back(start * (1.0f - a) + end * a);
        }
        
        mPoints.push_back(end);
        
    
    }
    
    inline void debugDraw(){
        glColor3f(0,0,1);
        gl::drawLine(getStart(),getEnd());
        
        glColor3f(1,1,1);
        glPointSize(3);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mPoints[0]);
        glDrawArrays(GL_POINTS, 0, mPoints.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        glPointSize(1);
       
        
    }
    
    inline const Vec3f& getStart() const{
        return mPoints[0];
    }
    
    inline const Vec3f& getEnd() const{
        return mPoints[PATH_RES - 1];
    }
    
    inline vector<Vec3f>& getPoints(){
        return mPoints;
    }
    
    
    
    
    
    
};



#endif
