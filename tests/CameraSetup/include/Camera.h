//
//  Camera.h
//  CameraSetup
//
//  Created by Henryk Wollik on 18/11/13.
//
//

#ifndef CameraSetup_Camera_h
#define CameraSetup_Camera_h

#include "cinder/Vector.h"
#include "cinder/Arcball.h"
#include "cinder/Camera.h"

class OrbitCam {
private:
    ci::Arcball     mArcball;
    ci::Vec2f       mMousePos;
    ci::Vec2f       mArcballPos;
    ci::CameraPersp mCamera;
    
public:
    
    OrbitCam(float distance = 1.0f);
    
    
    void mouseMove(const ci::Vec2f& pos);
    void mouseDrag(const ci::Vec2f& pos);
    void mouseDown(const ci::Vec2f& pos);
    
    const ci::CameraPersp& getCamera();
    const ci::Vec3f& getEye();
    const ci::Vec3f& getTarget();
    float getDistance();
};



#endif
