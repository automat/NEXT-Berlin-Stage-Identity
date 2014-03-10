//
//  FrustumOrtho.h
//  FrustumTest
//
//  Created by Henryk Wollik on 28/02/14.
//
//

#ifndef FrustumTest_FrustumOrtho_h
#define FrustumTest_FrustumOrtho_h

#include "cinder/Camera.h"
#include "cinder/Vector.h"
#include "cinder/Quaternion.h"
#include <OpenGL/OpenGL.h>

using namespace ci;

class FrustumOrtho {
    Vec3f mPlaneNormals[6];
    Vec3f mPlanePoints[6];
    float mPlaneDists[6];
    
    Vec3f mEye;
    Vec3f mN[4]; //ntl,ntr,nbr,nbl
    Vec3f mF[4]; //ftl,ftr,fbr,fbl
    
    inline void calcPlane(Vec3f* normal, Vec3f* point, float* dist, const Vec3f& v0, const Vec3f& v1, const Vec3f& v2){
        Vec3f aux0 = v0 - v1;
        Vec3f aux1 = v2 - v1;
        
        *normal = aux1.cross( aux0 ).normalized();
        *point  = v1;
        *dist = -( normal->dot( *point ) );
    }
    
    
public:
    FrustumOrtho(){};
    
    FrustumOrtho(const CameraOrtho& cam){
        set(cam);
    }
  
    inline void set(const CameraOrtho& cam, float frustumScale = 1.0f){
        float frustumNear, frustumFar;
        float frustumTop, frustumBottom, frustumLeft, frustumRight;
        cam.getFrustum(&frustumLeft, &frustumTop, &frustumRight, &frustumBottom, &frustumNear, &frustumFar);
    
        mEye = cam.getEyePoint();
        const Vec3f& dir = cam.getViewDirection();
        Quatf orient = cam.getOrientation();
       
        frustumNear *= frustumScale;
        frustumFar  *= frustumScale;
        Vec3f u = orient * Vec3f::xAxis() * frustumScale;
        Vec3f v = orient * Vec3f::yAxis() * frustumScale;
        
        Vec3f frustumNearDir = frustumNear   * dir;
        Vec3f frustumFarDir  = frustumFar    * dir;
        Vec3f frustumTopV    = frustumTop    * v;
        Vec3f frustumBottomV = frustumBottom * v;
        Vec3f frustumLeftU   = frustumLeft   * u;
        Vec3f frustumRightU  = frustumRight  * u;
        
        // near
        mN[0] = mEye + frustumNearDir + frustumTopV    + frustumLeftU;  // ntl
        mN[1] = mEye + frustumNearDir + frustumTopV    + frustumRightU; // ntr
        mN[2] = mEye + frustumNearDir + frustumBottomV + frustumRightU; // nbr
        mN[3] = mEye + frustumNearDir + frustumBottomV + frustumLeftU;  // nbl
        
        // far
        mF[0] = mEye + frustumFarDir + frustumTopV    + frustumLeftU;   // ftl
        mF[1] = mEye + frustumFarDir + frustumTopV    + frustumRightU;  // ftr
        mF[2] = mEye + frustumFarDir + frustumBottomV + frustumRightU;  // fbr
        mF[3] = mEye + frustumFarDir + frustumBottomV + frustumLeftU;   // fbl
        
        calcPlane(&mPlaneNormals[0], &mPlanePoints[0], &mPlaneDists[0], mN[1], mN[0], mF[0]); // ntr, ntl, ftl
        calcPlane(&mPlaneNormals[1], &mPlanePoints[1], &mPlaneDists[1], mN[3], mN[2], mF[2]); // nbl, nbr, fbr
        calcPlane(&mPlaneNormals[2], &mPlanePoints[2], &mPlaneDists[2], mN[0], mN[3], mF[3]); // ntl, nbl, fbl
        calcPlane(&mPlaneNormals[3], &mPlanePoints[3], &mPlaneDists[3], mF[1], mF[2], mN[2]); // ftr, fbr, nbr
        calcPlane(&mPlaneNormals[4], &mPlanePoints[4], &mPlaneDists[4], mN[0], mN[1], mN[2]); // ntl, ntr, nbr
        calcPlane(&mPlaneNormals[5], &mPlanePoints[5], &mPlaneDists[5], mF[1], mF[0], mF[3]); // ftr, ftl, fbl
    }
    
    const inline bool contains(const Vec3f& point) const{
        int i = -1;
        while (++i < 6) {
            if (mPlaneDists[i] + mPlaneNormals[i].dot( point ) < 0) {
                return false;
            }
        }
        return true;
    };
    
    inline void draw(){
        float prevColor[4];
        float prevPointSize;
        
        glGetFloatv(GL_CURRENT_COLOR, prevColor);
        glGetFloatv(GL_CURRENT_POINT_SIZE_APPLE, &prevPointSize);
        glColor3f(0, 1, 0);
        
        Vec3f t[4] = {mN[0],mN[1],mF[1],mF[0]};
        Vec3f b[4] = {mN[2],mN[3],mF[3],mF[2]};
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mN[0].x);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        
        glVertexPointer(3, GL_FLOAT, 0, &mF[0].x);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(4, 0xAAAA);
        glVertexPointer(3, GL_FLOAT, 0, &t[0].x);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glVertexPointer(3, GL_FLOAT, 0, &b[0].x);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisable(GL_LINE_STIPPLE);
        
        glPointSize(10);
        glVertexPointer(3, GL_FLOAT, 0, &mEye.x);
        glDrawArrays(GL_POINTS, 0, 1);
        glPointSize(prevPointSize);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        
        glColor4f(prevColor[0],prevColor[1],prevColor[2],prevColor[3]);
    }
    
    
    
};



#endif
