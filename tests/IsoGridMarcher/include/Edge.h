//
//  GridEdge.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 28/11/13.
//
//

#ifndef IsoGridMarcher_GridEdge_h
#define IsoGridMarcher_GridEdge_h

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/Matrix44.h"
#include "cinder/Quaternion.h"
#include "cinder/Rand.h"
#include <OpenGL/OpenGL.h>
#include <vector>
#include "cinder/TriMesh.h"

class Edge : public ci::TriMesh{
private:
    bool mOn;
    bool mActive;
    ci::Vec2f mSizeOff;
    ci::Vec2f mSizeOn;
    
    ci::Vec3f* mP0;
    ci::Vec3f* mP1;
    
    float         mDistance;
    ci::Matrix44f mMatrix;
    
    void init();
    
    
public:
    Edge();
    Edge(ci::Vec3f* p0, ci::Vec3f* p1, bool isOn = false);
    
    void setSizeOn(const ci::Vec2f& size) { mSizeOn = size; }
    void setSizeOff(const ci::Vec2f& size){ mSizeOff = size; }
    
    void drawOcclusive();
    void drawEmissive();
    
    void updateGeometry();
    
    void switchOn(){if(!mActive)return;mOn = true;}
    void switchOff(){mOn = false;}
    
    void switchRandom(float triggerTolerance = 0.0015f);
    void getMid(ci::Vec3f* vec);
    
    bool isOn()    { return mOn; }
    bool isActive(){ return mActive; }
    
};

#endif
