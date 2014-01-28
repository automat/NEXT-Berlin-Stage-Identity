//
//  FragmentSurface.h
//  Fragment_00
//
//  Created by Henryk Wollik on 23/01/14.
//
//

#ifndef Fragment_00_FragmentSurface_h
#define Fragment_00_FragmentSurface_h

#include <vector>
#include <math.h>
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"





class FragmentSurface {
    size_t mNumPointsX;
    size_t mNumPointsY;
    size_t mNumPoints;
    
    std::vector<ci::Vec3f> mPoints;
    std::vector<ci::Vec2f> mTexcoords;
    std::vector<ci::Vec3f> mTrianglePoints;
    std::vector<ci::Vec3f> mTriangleCenters;
    std::vector<ci::Vec2f> mTriangleTexcoords;
    
    
    
    
    size_t mNumFieldPointsX;
    size_t mNumFieldPointsY;
    size_t mNumFieldPoints;
    std::vector<ci::Vec3f> mFieldPoints;
    std::vector<ci::Vec3f> mFieldTangents; //tangent bundle
    std::vector<uint>      mFieldTangentIndex;

    ci::gl::Texture mTexture;
    
    
    void init(size_t resolutionXY = 5);
    
    
public:
    FragmentSurface();
    
    void draw();
    void update(float t);
};


#endif
