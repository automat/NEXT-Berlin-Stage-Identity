//
//  TriMeshBatch.h
//  TriangleGeometry
//
//  Created by Henryk Wollik on 27/11/13.
//
//

#ifndef TriangleGeometry_TriMeshBatch_h
#define TriangleGeometry_TriMeshBatch_h

#include "cinder/gl/TriMesh.h"
#include <vector>
#include "cinder/Vector.h"

class TriMeshBatch {
private:
    int mIndexVertices;
    int mIndexNormals;
    int mIndexIndices;
    
    std::vector<ci::Vec3f> mVertices;
    std::Vector<ci::Vec3f> mNormals;
    std::vector<uint>      mIndices;
    
public:
    TriMeshBatch();
    TriMeshBatch(int sizeVertices,
                 int sizeNormals,
                 int sizeInidices){
        this->resize(sizeVertices,
                     sizeNormals,
                     sizeInidices);
    }

    void resize(int sizeVertices,
                int sizeNormals,
                int sizeInidices){
        mIndexVertices = 0;
        mIndexNormals  = 0;
        mIndexIndices  = 0;
        
        mVertices.resize(sizeVertices);
        mNormals.resize(sizeNormals);
        mIndices.resize(sizeInidices);
        
    };
    
    void reset(){
        mIndexVertices = 0;
        mIndexNormals  = 0;
        mIndexIndices  = 0;
    }
    
    void put(const ci::TriMesh& mesh){
        std::vector<ci::Vec3f>& meshVertices = mesh.getVertices();
        std::vector<ci::Vec3f>& meshNormals  = mesh.getNormals();
        std::vector<uint>&      meshIndices  = mesh.getIndices();

        int offsetVerticesSize = mIndexVertices + meshVertices.size();
        int offsetNormalsSize  = mIndexNormals  + meshNormals.size();
        int offsetIndicesSize  = mIndexIndices  + meshIndices.size();
        
        while (mIndexVertices < offsetVerticesSize) {
            mVertices[mIndexVertices].set(meshVertices[mIndexVertices++]);
        }
        
        while (mIndexNormals < offsetNormalsSize) {
            mNormals[mIndexNormals].set(meshNormals[mIndexNormals++]);
        }
        
        while (mIndexIndices < offsetIndicesSize) {
            mIndices[mIndexIndices].set(meshIndices[mIndexIndices++]);
        }
    }
    
    const std::vector<ci::Vec3f>& getVertices() const{
        return mVertices;
    };
    
    const std::vector<ci::Vec3f>& getNormals()  const{
        return mNormals;
    }
    
    const std::vector<uint>& getIndices()const{
        return mIndices;
    }
    
    std::vector<ci::Vec3f>& getVertices(){
        return mVertices;
    }
    
    std::vector<ci::Vec3f>& getNormals(){
        return mNormals;
    }
    
    std::vector<uint>& getIndices(){
        return mIndices;
    }
    
    size_t getVerticesNum(){
        return mVertices.size();
    }
    
    size_t getNormalsNum(){
        return mNormals.size();
    }
    
    size_t getIndicesNum(){
        return mIndices.size();
    }
    

    
};



#endif
