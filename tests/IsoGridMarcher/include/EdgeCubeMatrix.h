//
//  EdgeCubeMatrix.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 03/12/13.
//
//

#ifndef IsoGridMarcher_EdgeCubeMatrix_h
#define IsoGridMarcher_EdgeCubeMatrix_h

#include <vector>
#include "cinder/Vector.h"

#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Utilities.h"

#include "TriMeshBatch.h"

#include "Edge.h"
#include "EdgeCube.h"

#include "EdgePattern.h"
#include "EdgePatternSequence.h"


class EdgeCubeMatrix {
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
    
    bool  mDrawEdgesOff;
    bool  mDrawPoints;
    ci::Vec2f mEdgesSizeOffLast;
    ci::Vec2f mEdgesSizeOnLast;
    float mPointSize;
    
    std::vector<ci::Vec3f> mPoints;
    std::vector<Edge> mEdgesX; // X axis
    std::vector<Edge> mEdgesY; // Y axis
    std::vector<Edge> mEdgesZ; // Z axis
    std::vector<Edge> mEdgesXD; // diagonal
    std::vector<Edge> mEdgesYD; // diagonal
    std::vector<Edge> mEdgesZD; // diagonal
    std::vector<EdgeCube> mCubes;
    
    void build();
    void clear();
    
    ci::Font               mDebugFont;
    ci::gl::TextureFontRef mTextureDebugFont;
    float                  mDebugFontScale;
    
    TriMeshBatch* mBatchEdgesOffX;
    TriMeshBatch* mBatchEdgesOffY;
    TriMeshBatch* mBatchEdgesOffZ;
    TriMeshBatch* mBatchEdgesOffXD;
    TriMeshBatch* mBatchEdgesOffYD;
    TriMeshBatch* mBatchEdgesOffZD;
    
    void initDebugFont();
    void initBatchesOff();
    
    void setSize_Internal(int sizeX, int sizeY, int sizeZ);
    
    void drawBatch(TriMeshBatch* batch);
    void putBatch(std::vector<Edge>& edges, TriMeshBatch* batch);
    
    void switchOnEdges(std::vector<Edge>& edges);
    void switchOffEdges(std::vector<Edge>& edges);
    void switchRandomEdges(std::vector<Edge>& edges, float triggerTolerance);
    
    void setSizeOnEdges(std::vector<Edge> &edges, const ci::Vec2f& size);
    void setSizeOffEdges(std::vector<Edge> &edges, const ci::Vec2f& size);
    
    void drawDebugBulb(std::vector<Edge> &edges);
    
public:
    EdgeCubeMatrix();
    ~EdgeCubeMatrix();
    
    //! Set initial grid size
    void setSize(int sizeXYZ);
    void setSize(int sizeX, int sizeY, int sizeZ);
    
    //! Update time component of all edges
    void update();
    
    //! Draw occlusive bulb parts and joints
    void drawOcclusive();
    //! Draw emmisive bulb parts
    void drawEmissive();
    //! switch edges based on pattern & pattern sequences
    void applyPattern(const EdgePattern& pattern);
    void applyPatternSeq(const EdgePatternSequence& seq);
    void applyPatternSeqs(const std::vector<EdgePatternSequence>& seqs);
    
    //! Switch all edges on
    void switchOn();
    //! Switch all edges off
    void switchOff();
    //! Switch edges on/off randomly
    void switchRandom(float triggerTolerance = 0.0015f);
    
    void setDrawEdgesOff(bool b){ mDrawEdgesOff = b; };
    void setDrawPoints(bool b){ mDrawPoints = b; };
    void setPointsSize(float size){ mPointSize = size; };
    void setEdgeSizeOff(const ci::Vec2f& size);
    void setEdgeSizeOn(const ci::Vec2f& size);
    
    
    //! Get pointer to cube at index x y z
    EdgeCube* getEdgeCube(int x, int y, int z);
    //! Get pointer to cube at index x y z, ax ay az added
    EdgeCube* getEdgeCube(int x, int y, int z, int ax, int ay, int az);
    //! Get pointer to cube index ax ay az added
    EdgeCube* getEdgeCube(EdgeCube& baseCube, int ax, int ay, int az);
    
    //! Get points of grid bulb edges are connected to
    const std::vector<ci::Vec3f>& getGridPoints() const{ return mPoints;}
    //! Get edges all perpendicular axes
    const std::vector<Edge>& getEdgesX() const{ return mEdgesX; };
    const std::vector<Edge>& getEdgesY() const{ return mEdgesY; };
    const std::vector<Edge>& getEdgesZ() const{ return mEdgesZ; };
    std::vector<Edge>& getEdgesX(){ return mEdgesX; }
    std::vector<Edge>& getEdgesY(){ return mEdgesY; }
    std::vector<Edge>& getEdgesZ(){ return mEdgesZ; }
    //! Get edges all diagonal axes
    const std::vector<Edge>& getEdgesXD() const{ return mEdgesXD; }
    const std::vector<Edge>& getEdgesYD() const{ return mEdgesYD; }
    const std::vector<Edge>& getEdgesZD() const{ return mEdgesZD; }
    std::vector<Edge>& getEdgesXD(){ return mEdgesXD; }
    std::vector<Edge>& getEdgesYD(){ return mEdgesYD; }
    std::vector<Edge>& getEdgesZD(){ return mEdgesZD; }
    
    //! Get cubes formed defined by bulb edges
    const std::vector<EdgeCube>& getCubes() const{return mCubes; }
    std::vector<EdgeCube>& getCubes(){ return mCubes; }
    
    //! Get nums
    int getNumCubes(){ return mNumCubes;};
    int getNumCubesX(){return mNumCubesX;};
    int getNumCubesY(){return mNumCubesY;};
    int getNumCubesZ(){return mNumCubesZ;};
    
    
    int getNumEdgesX() {return mEdgesX.size();};
    int getNumEdgesY() {return mEdgesY.size();};
    int getNumEdgesZ() {return mEdgesZ.size();};
    int getNumEdgesXD(){return mEdgesXD.size();};
    int getNumEdgesYD(){return mEdgesYD.size();};
    int getNumEdgesZD(){return mEdgesZD.size();};
    
    //! Debug view grid points
    void drawDebugPoints();
    //! Debug view all perpendicular axes
    void drawDebugEdgesX(){ this->drawDebugBulb(mEdgesX);};
    void drawDebugEdgesY(){ this->drawDebugBulb(mEdgesY);};
    void drawDebugEdgesZ(){ this->drawDebugBulb(mEdgesZ);};
    //! Debug view all diagonal axes
    void drawDebugEdgesXD(){ this->drawDebugBulb(mEdgesXD);};
    void drawDebugEdgesYD(){ this->drawDebugBulb(mEdgesYD);};
    void drawDebugEdgesZD(){ this->drawDebugBulb(mEdgesZD);};
    //! Debug view all cubes
    void drawDebugCubes();


    
    
   
    
    
    
    
};


#endif
