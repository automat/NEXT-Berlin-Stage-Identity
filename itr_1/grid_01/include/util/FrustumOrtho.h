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
    std::vector<Vec3f> mN; //ntl,ntr,nbr,nbl
    std::vector<Vec3f> mF; //ftl,ftr,fbr,fbl

    void calcPlane(Vec3f* normal, Vec3f* point, float* dist, const Vec3f& v0, const Vec3f& v1, const Vec3f& v2);

public:
    FrustumOrtho();
    FrustumOrtho(const CameraOrtho& cam);

    void set(const CameraOrtho& cam, float frustumScale = 1.0f);
    bool contains(const Vec3f& point) const;
    bool contains(const Vec3f& point);

    inline const std::vector<Vec3f>& getNearPlane() const{
        return mN;
    }

    inline const std::vector<Vec3f>& getFarPlane() const{
        return mF;
    }

    inline const std::vector<Vec3f>& getNearPlane(){
        return mN;
    }

    inline const std::vector<Vec3f>& getFarPlane(){
        return mF;
    }

    void draw();
};

#endif
