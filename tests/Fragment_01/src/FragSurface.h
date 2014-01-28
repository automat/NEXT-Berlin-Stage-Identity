//
//  FragmentSurface.h
//  Fragment_01
//
//  Created by Henryk Wollik on 26/01/14.
//
//

#ifndef __Fragment_01__FragmentSurface__
#define __Fragment_01__FragmentSurface__

#include <iostream>
#include <vector>
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Font.h"
#include "cinder/gl/TextureFont.h"

using namespace std;
using namespace ci;

class FragSurface {
    vector<Vec3f> mPoints;
    vector<Vec2f> mTexcoords;
    
    vector<Vec2f> mFragTriangleTexcoords;
    vector<Vec2f> mFragTriangleTexcoordsMultiplied;
    vector<Vec3f> mFragTriangleVertices;
    vector<Vec3f> mFragTriangleCenters;
    vector<uint>  mFragTriangleTangentIndex;
    
    
    vector<Vec3f> mFieldPoints;
    vector<Vec3f> mFieldTangents;
    int           mNumFieldTangentsX;
    int           mNumFieldTangentsY;
    int           mNumFieldTangentsX_1;
    int           mNumFieldTangentsY_1;
    
    void initSurface(int numPointsX, int numPointsY);
    void initTriangle(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2,
                      const Vec2f& t0, const Vec2f& t1, const Vec2f& t2);
    void initFbo();
    void resizeFbo();
    
   
    
    gl::Texture mQuoteTexture;
    
    int         mFboWidth;
    gl::Fbo     mFboRender;
    gl::Texture mTexture;
    
    
    
    
public:
    static bool ANIMATE;
    static bool DEBUG_DRAW_FIELD;
    static bool DEBUG_DRAW_TANGENTS;
    static bool DRAW_TRIANGLES;
    
    FragSurface();
   
    void drawFbo();
    void draw(float t);
    void update(float t);

};

#endif /* defined(__Fragment_01__FragmentSurface__) */
