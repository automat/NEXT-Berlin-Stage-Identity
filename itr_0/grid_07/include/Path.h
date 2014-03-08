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

#include "Config.h"

#include <math.h>

using namespace std;
using namespace ci;
using namespace config;


class Path {
    float         mLength;
    int           mNumPoints;
    vector<Vec3f> mPoints;
    
public:
    Path(Vec3f start, Vec3f end){
        mLength    = start.z - end.z;
        mNumPoints = mLength * PATH_NUM_POINTS;
        
        mPoints.push_back(start);
        
        int i = 0;
        float a;
        while (++i < mNumPoints - 1) {
            a = float(i)/float(mNumPoints-1);
            mPoints.push_back(start * (1.0f - a) + end * a);
        }

        mPoints.push_back(end);

        
    }
    
    inline void debugDraw(){
        glColor3f(0.25f,0.15,0.25f);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mPoints[0]);
        glDrawArrays(GL_LINE_STRIP, 0, mPoints.size());
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    inline const Vec3f& getStart() const{
        return mPoints[0];
    }
    
    inline const Vec3f& getEnd() const{
        return mPoints[PATH_NUM_POINTS - 1];
    }
    
    inline vector<Vec3f>& getPoints(){
        return mPoints;
    }
    
    inline int getNumPoints(){
        return mNumPoints;
    }
    
    inline float getLength(){
        return mLength;
    }
    
    inline void getPointOn(float intrpl, Vec3f* out){
        intrpl = max(0.0f, min(intrpl,1.0f));

        int size = mPoints.size();
        int len  = size - 1;
        
        uint  index  = (uint)floorf((float)len * intrpl),
              index1 = (uint)min((uint)index + 1, (uint)len);
        float localIntrpl    = fmodf(intrpl, 1.0f / (float)len) * (float)len,
              localIntrplInv = 1.0f - localIntrpl;
        
        out->set(mPoints[index] * localIntrplInv + mPoints[index1] * localIntrpl);
    }
};

#endif