//
//  AbstractField.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_AbstractField_h
#define grid_00_AbstractField_h

#include <boost/assign/std/vector.hpp>
#include <OpenGL/OpenGL.h>

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Matrix44.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"

#include "Config.h"

#include "world/grid/Cell.h"
#include "world/board/path/PathSurface.h"
#include "world/board/diver/Diver.h"


using namespace ci;
using namespace std;

class Oscillator;


class AbstractField{
protected:
    
    /*--------------------------------------------------------------------------------------------*/
    //  General
    /*--------------------------------------------------------------------------------------------*/
    
    bool        mActive;
    Vec3f       mPos;
    int         mSize;
    
    Matrix44f   mTransform;
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Path Surface
    /*--------------------------------------------------------------------------------------------*/
    
    int         mSurfaceNumSlices;
    Vec3f       mSurfaceDisplacement;   //  field random pos offset
    float       mSurfaceDensity;        //  field density, low/high freq
    float       mSurfaceAmplitude;      //  field amplitude scale
    float       mSurfaceOffset;
    float       mSurfaceOffsetSpeed;
    
    PathSurface mPathSurface;
    
    void updatePathSurface(Oscillator* osc,float t);
    void debugDrawArea_Internal();
    
    /*--------------------------------------------------------------------------------------------*/
    //  Diver
    /*--------------------------------------------------------------------------------------------*/
    
    vector<Diver*> mDivers;
    int            mNumDivers;
    int            mNumDiversMin;
    int            mNumDiversMax;
    int            mDiverUnitNumPoints;
    float          mDiverOffsetMin;
    float          mDiverOffsetMax;
    float          mDiverSpeedMin;
    float          mDiverSpeedMax;
    float          mDiverLengthMin;
    float          mDiverLengthMax;
    float          mDiverHeightMin;
    float          mDiverHeightMax;
    
    void updateDivers();
    
    /*--------------------------------------------------------------------------------------------*/
    //  Geometry
    /*--------------------------------------------------------------------------------------------*/
    
    int mDiverVerticesBodyLen;
    int mDiverVerticesCapLen;
    int mDiverVerticesLen;
    int mDiverIndicesLen;
    
    int mMeshVerticesLen;
    int mMeshIndicesLen;
    
    gl::VboMesh::Layout mMeshLayout;
    gl::VboMesh         mMesh;
    
    vector<Vec3f>    mMeshVertexBuffer;     // keep copy of vertices
    vector<Vec3f>    mMeshNormalBuffer;     // buffer for normals
    vector<Vec3f>    mMeshNormalTopBuffer;  // buffer for all top normals
    vector<uint32_t> mMeshIndexScheme;      // indices order, used for normal calculation
    vector<uint32_t> mMeshIndexBuffer;      // indices order, send to buffer object concatenated
    
    vector<uint32_t> mDiverIndicesFolded;   // indices order, when folded
    vector<uint32_t> mDiverIndicesUnfolded; // indices order, when unfolded
    vector<uint32_t> mDiverIndicesBuffer;   // indices order, when translated to specific diver
    
    //
    // collapse all vertices of a diver into one vertex by setting
    // all its indices to its first index
    //
    void fold(int index);
    
    //
    // reorder all indices to their vertices,
    // recreating the orignal shape
    //
    void unfold(int index);

    
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Internal
    /*--------------------------------------------------------------------------------------------*/
    
    // free all allocated data
    void freeData();
    // init all
    void reset_Internal();
    
public:
    AbstractField(const Vec3f& pos, int size, int numPathSlices);
    ~AbstractField();
    
    virtual void debugDrawArea() = 0;
    
    void debugDrawPathSurface();
    void debugDrawDivers();

    virtual void draw() = 0;
    virtual void update(Oscillator* osc, float t)= 0;
    
};

#endif
