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
    
    float mLightBulbSizeOffLast;
    float mLightBulbSizeOnLast;
    
    int mNumLightBulbsXX;
    int mNumLightBulbsXY;
    int mNumLightBulbsXZ;
    int mNumLightBulbsXZY;
    
    
    int mNumLightBulbsYX;
    int mNumLightBulbsYY;
    int mNumLightBulbsYZ;
    int mNumLightBulbsYZX;
    
    
    int mNumLightBulbsZX;
    int mNumLightBulbsZY;
    int mNumLightBulbsZZ;
    int mNumLightBulbsZXY;
    
    
    
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
    
    void setDrawBulbsOff(bool b)      { mDrawBulbsOff = b; };
    void setDrawGridPoints(bool b)    { mDrawGridPoints = b; };
    void setGridPointsSize(float size){ mGridPointsSize = size; };
    void setLightBulbSizeOff(float size);
    void setLightBulbSizeOn(float size);
    
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
    
    
    //! Number of x bulbs on x axis 1 row
    int getNumLightBulbsXX() {return mNumLightBulbsYX;}
    //! Number of x bulbs on y axis 1 column
    int getNumLightBulbsXY() {return mNumLightBulbsYY;};
    //! Number of x bulbs on z axis 1 row
    int getNumLightBulbsXZ() {return mNumLightBulbsYX;}
    //! Number of x bulbs on x * z axis 1 row
    int getNumLightBulbsXZY(){return mNumLightBulbsXZY;}
    
    //! Number of y bulbs on x axis 1 row
    int getNumLightBulbsYX() {return mNumLightBulbsYX;}
    //! Number of y bulbs on y axis 1 column
    int getNumLightBulbsYY() {return mNumLightBulbsYY;};
    //! Number of y bulbs on z axis 1 row
    int getNumLightBulbsYZ() {return mNumLightBulbsYX;}
    //! Number of y bulbs on x * z axis 1 row
    int getNumLightBulbsYZX(){return mNumLightBulbsYZX;}
    
    //! Number of y bulbs on x axis 1 row
    int getNumLightBulbsZX() {return mNumLightBulbsZX;}
    //! Number of y bulbs on y axis 1 column
    int getNumLightBulbsZY() {return mNumLightBulbsZY;};
    //! Number of y bulbs on z axis 1 row
    int getNumLightBulbsZZ() {return mNumLightBulbsZZ;}
    //! Number of y bulbs on x * z axis 1 row
    int getNumLightBulbsZXY(){return mNumLightBulbsZXY;}
    
    void getLightBulbX(int indexBase, int indexAddX, int indexAddY, int indexAddZ, LightBulb* bulb);
    
    
   
    
    
    
    
};


#endif
