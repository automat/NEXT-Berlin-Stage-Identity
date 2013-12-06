//
//  LightCubeMatrix.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 03/12/13.
//
//

#ifndef IsoGridMarcher_LightCubeMatrix_h
#define IsoGridMarcher_LightCubeMatrix_h

#include <vector>
#include "cinder/Vector.h"

#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Utilities.h"

#include "LightBulb.h"
#include "LightCube.h"

#include "TriMeshBatch.h"

class LightCubeMatrix {
    int mNumPointsX;
    int mNumPointsY;
    int mNumPointsZ;
    int mNumPoints;
    
    int mNumPointsX_1;
    int mNumPointsY_1;
    int mNumPointsZ_1;
    
    int mNumPointsY_1Z_1;
    int mNumPointsYZ;
    int mNumPointsYZ_1;
    int mNumPointsY_1Z;
    
    int mNumCubesX;
    int mNumCubesY;
    int mNumCubesZ;
    int mNumCubes;
    
    bool  mDrawBulbsOff;
    bool  mDrawGridPoints;
    float mGridPointsSize;
    
    float mBulbSizeOffLast;
    float mBulbSizeOnLast;
    
    std::vector<ci::Vec3f> mGridPoints;
    std::vector<LightBulb> mBulbsX; // X axis
    std::vector<LightBulb> mBulbsY; // Y axis
    std::vector<LightBulb> mBulbsZ; // Z axis
    std::vector<LightBulb> mBulbsXD; // diagonal
    std::vector<LightBulb> mBulbsYD; // diagonal
    std::vector<LightBulb> mBulbsZD; // diagonal
    std::vector<LightCube> mCubes;
    
    void build();
    void clear();
    
    ci::Font               mDebugFont;
    ci::gl::TextureFontRef mTextureDebugFont;
    float                  mDebugFontScale;
    
    TriMeshBatch* mBatchBulbsOffX;
    TriMeshBatch* mBatchBulbsOffY;
    TriMeshBatch* mBatchBulbsOffZ;
    TriMeshBatch* mBatchBulbsOffXD;
    TriMeshBatch* mBatchBulbsOffYD;
    TriMeshBatch* mBatchBulbsOffZD;
    
    void initDebugFont();
    void initBatchesOff();
    
    void setNumPoints_Internal(int numX, int numY, int numZ);
    
    void drawBatch(TriMeshBatch* batch);
    void putBatch(std::vector<LightBulb>& bulbs, TriMeshBatch* batch);
    
    void switchOnBulb(std::vector<LightBulb>& bulbs);
    void switchOffBulbs(std::vector<LightBulb>& bulbs);
    void switchRandomBulbs(std::vector<LightBulb>& bulbs, float triggerTolerance);
    
    void drawDebugBulb(std::vector<LightBulb> &bulbs);
    
public:
    LightCubeMatrix();
    ~LightCubeMatrix();
    
    //! Set initial grid size
    void setPointsNum(int numXYZ);
    void setPointsNum(int numX, int numY, int numZ);
    
    //! Update time component of all edges
    void update();
    
    //! Draw occlusive bulb parts and joints
    void drawOcclusive();
    //! Draw emmisive bulb parts
    void drawEmissive();
    
    //! Switch all bulbs on
    void switchOn();
    //! Switch all bulbs off
    void switchOff();
    //! Switch bulbs on/off randomly
    void switchRandom(float triggerTolerance = 0.0015f);
    
    void setDrawBulbsOff(bool b)      { mDrawBulbsOff = b; };
    void setDrawGridPoints(bool b)    { mDrawGridPoints = b; };
    void setGridPointsSize(float size){ mGridPointsSize = size; };
    void setBulbSizeOff(float size);
    void setBulbSizeOn(float size);
    
    
    //! Get pointer to cube at index x y z
    LightCube* getLightCube(int x, int y, int z);
    //! Get pointer to cube at index x y z, ax ay az added
    LightCube* getLightCube(int x, int y, int z, int ax, int ay, int az);
    //! Get pointer to cube index ax ay az added
    LightCube* getLightCube(LightCube& baseCube, int ax, int ay, int az);
    
    //! Get points of grid bulb edges are connected to
    const std::vector<ci::Vec3f>& getGridPoints() const{ return mGridPoints;}
    //! Get bulbs all perpendicular axes
    const std::vector<LightBulb>& getBulbsX() const{ return mBulbsX; };
    const std::vector<LightBulb>& getBulbsY() const{ return mBulbsY; };
    const std::vector<LightBulb>& getBulbsZ() const{ return mBulbsZ; };
    std::vector<LightBulb>& getBulbsX(){ return mBulbsX; }
    std::vector<LightBulb>& getBulbsY(){ return mBulbsY; }
    std::vector<LightBulb>& getBulbsZ(){ return mBulbsZ; }
    //! Get bulbs all diagonal axes
    const std::vector<LightBulb>& getBulbsXD() const{ return mBulbsXD; }
    const std::vector<LightBulb>& getBulbsYD() const{ return mBulbsYD; }
    const std::vector<LightBulb>& getBulbsZD() const{ return mBulbsZD; }
    std::vector<LightBulb>& getBulbsXD(){ return mBulbsXD; }
    std::vector<LightBulb>& getBulbsYD(){ return mBulbsYD; }
    std::vector<LightBulb>& getBulbsZD(){ return mBulbsZD; }
    
    //! Get cubes formed defined by bulb edges
    const std::vector<LightCube>& getCubes() const{return mCubes; }
    std::vector<LightCube>& getCubes(){ return mCubes; }
    
    //! Get nums
    int getNumCubes(){ return mNumCubes;};
    int getNumCubesX(){return mNumCubesX;};
    int getNumCubesY(){return mNumCubesY;};
    int getNumCubesZ(){return mNumCubesZ;};
    
    
    int getNumBulbsX() {return mBulbsX.size();};
    int getNumBulbsY() {return mBulbsY.size();};
    int getNumBulbsZ() {return mBulbsZ.size();};
    int getNumBulbsXD(){return mBulbsXD.size();};
    int getNumBulbsYD(){return mBulbsYD.size();};
    int getNumBulbsZD(){return mBulbsZD.size();};
    
    
    //! Debug view grid points
    void drawDebugPoints();
    //! Debug view all perpendicular axes
    void drawDebugBulbsX(){ this->drawDebugBulb(mBulbsX);};
    void drawDebugBulbsY(){ this->drawDebugBulb(mBulbsY);};
    void drawDebugBulbsZ(){ this->drawDebugBulb(mBulbsZ);};
    //! Debug view all diagonal axes
    void drawDebugBulbsXD(){ this->drawDebugBulb(mBulbsXD);};
    void drawDebugBulbsYD(){ this->drawDebugBulb(mBulbsYD);};
    void drawDebugBulbsZD(){ this->drawDebugBulb(mBulbsZD);};
    //! Debug view all cubes
    void drawDebugCubes();


    
    
   
    
    
    
    
};


#endif
