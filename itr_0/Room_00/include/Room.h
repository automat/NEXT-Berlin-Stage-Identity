//
//  Room.h
//  Room_00
//
//  Created by Henryk Wollik on 01/02/14.
//
//

#ifndef __Room_00__Room__
#define __Room_00__Room__

#include <iostream>
#include "cinder/Vector.h"
#include "cinder/TriMesh.h"

#include "cinder/gl/Material.h"
#include "cinder/gl/Light.h"

#include "cinder/gl/GlslProg.h"

#include "cinder/Camera.h"

using namespace ci;

class Room {
    Vec3f mDim;
    float mTime;
    float mTimeStart;
    float mFrame;
    CameraPersp* mCameraRef;
    
    TriMesh mMeshDome;
    TriMesh mMeshFloor;
    
    gl::Light*    mLightDebug;
    gl::Material* mMaterialDebug;
    
    gl::GlslProg mNormalDebugShader;
    
    
public:
    Room();
    Room(float dim, CameraPersp* camera);
    
    void update();
    void draw();
    
    const Vec3f& getDim();
};

#endif /* defined(__Room_00__Room__) */
