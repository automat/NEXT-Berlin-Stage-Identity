//
//  NLogo.h
//  Logo_00
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#ifndef Logo_00_NLogo_h
#define Logo_00_NLogo_h

#include <math.h>
#include <vector>

#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include "cinder/TriMesh.h"


#include "GeomHelper.h"

using namespace ci;

class NLogo {
    float       mUnit;
    TriMesh     mMesh;
    
    
    
public:
    NLogo();
    
    void draw();
    void update(float t);
};



#endif
