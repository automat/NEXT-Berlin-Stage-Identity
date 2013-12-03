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

class LightCubeMatrix {
    int mNumPointsX;
    int mNumPointsY;
    int mNumPointsZ;
    int mNumPoints;
    
    int mNumPointsX_1;
    int mNumPointsY_1;
    int mNumPointsZ_1;
    
    int mNumCubesX;
    int mNumCubesY;
    int mNumCubesZ;
    int mNumCubes;
    
    std::vector<ci::Vec3f> mGridPoints;
    std::vector<LightBulb> mLightBulbsX; // X axis
    std::vector<LightBulb> mLightBulbsY; // Y axis
    std::vector<LightBulb> mLightBulbsZ; // Z axis
    std::vector<LightBulb> mLightBulbsXD; // diagonal
    std::vector<LightBulb> mLightBulbsYD; // diagonal
    std::vector<LightBulb> mLightBulbsZD; // diagonal
    std::vector<LightCube> mLightCubes;
    
    void build();
    void clear();
    
    ci::Font               mDebugFont;
    ci::gl::TextureFontRef mTextureDebugFont;
    float                  mDebugFontScale;

    void initDebugFont();
    
    void setNumPoints_Internal(int numX, int numY, int numZ);
    
    
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
    
    //! Debug view grid points
    void drawDebugPoints();
    //! Debug view all perpendicular axes
    void drawDebugLightBulbsX();
    void drawDebugLightBulbsY();
    void drawDebugLightBulbsZ();
    //! Debug view all diagonal axes
    void drawDebugLightBulbsXD();
    void drawDebugLightBulbsYD();
    void drawDebugLightBulbsZD();
    
    //! Get points of grid bulb edges are connected to
    const std::vector<ci::Vec3f>& getGridPoints() const{ return mGridPoints;}
    //! Get bulbs all perpendicular axes
    const std::vector<LightBulb>& getLightBulbsX() const{ return mLightBulbsX; };
    const std::vector<LightBulb>& getLightBulbsY() const{ return mLightBulbsY; };
    const std::vector<LightBulb>& getLightBulbsZ() const{ return mLightBulbsZ; };
    std::vector<LightBulb>& getLightBulbsX(){ return mLightBulbsX; }
    std::vector<LightBulb>& getLightBulbsY(){ return mLightBulbsY; }
    std::vector<LightBulb>& getLightBulbsZ(){ return mLightBulbsZ; }
    //! Get bulbs all diagonal axes
    const std::vector<LightBulb>& getLightBulbsXD() const{ return mLightBulbsXD; }
    const std::vector<LightBulb>& getLightBulbsYD() const{ return mLightBulbsYD; }
    const std::vector<LightBulb>& getLightBulbsZD() const{ return mLightBulbsZD; }
    std::vector<LightBulb>& getLightBulbsXD(){ return mLightBulbsXD; }
    std::vector<LightBulb>& getLightBulbsYD(){ return mLightBulbsYD; }
    std::vector<LightBulb>& getLightBulbsZD(){ return mLightBulbsZD; }
    
    //! Get cubes formed defined by bulb edges
    const std::vector<LightCube>& getLightCubes() const{return mLightCubes; }
    std::vector<LightCube>& getLightCubes(){ return mLightCubes; }
    
    //! Get nums
    int getNumLightCubes(){return mNumCubes;};
    int getNumLightBulbsX() {return mLightBulbsX.size();};
    int getNumLightBulbsY() {return mLightBulbsY.size();};
    int getNumLightBulbsZ() {return mLightBulbsZ.size();};
    int getNumLightBulbsXD(){return mLightBulbsXD.size();};
    int getNumLightBulbsYD(){return mLightBulbsYD.size();};
    int getNumLightBulbsZD(){return mLightBulbsZD.size();};
   
    
    
    
    
};


#endif
