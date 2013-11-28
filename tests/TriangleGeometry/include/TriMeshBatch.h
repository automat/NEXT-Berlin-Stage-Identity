//
//  TriMeshBatch.h
//  TriangleGeometry
//
//  Created by Henryk Wollik on 27/11/13.
//
//

#ifndef TriangleGeometry_TriMeshBatch_h
#define TriangleGeometry_TriMeshBatch_h

#include "cinder/TriMesh.h"
#include <vector>
#include "cinder/Vector.h"

class TriMeshBatch {
private:
    int mIndexVertices;
    int mIndexNormals;
    int mIndexIndices;
    
    std::vector<ci::Vec3f> mVertices;
    std::vector<ci::Vec3f> mNormals;
    std::vector<uint>      mIndices;
    
public:
    TriMeshBatch(){
        resize(0,0);
    }
    
    TriMeshBatch(int sizeVertices,
                 int sizeInidices){
        
        this->resize(sizeVertices,
                     sizeInidices);
    }
    
    void resize(int sizeVertices,
                int sizeInidices){
        
        mVertices.resize(sizeVertices);
        mNormals.resize(sizeVertices);
        mIndices.resize(sizeInidices);
    
        this->reset();
    };
    
    void reset(){
        mIndexVertices = 0;
        mIndexNormals  = 0;
        mIndexIndices  = 0;
    }
    
    void put(ci::TriMesh& mesh){
        std::vector<ci::Vec3f>& meshVertices = mesh.getVertices();
        std::vector<ci::Vec3f>& meshNormals  = mesh.getNormals();
        std::vector<uint>&      meshIndices  = mesh.getIndices();
        
        int offsetVerticesSize = mIndexVertices + meshVertices.size();
        int offsetNormalsSize  = mIndexNormals  + meshNormals.size();
        int offsetIndicesSize  = mIndexIndices  + meshIndices.size();
        int i;
        
        if(offsetVerticesSize > mVertices.size()) {
            throw "Vertex size exceeds reserved capacity.";
        }
        
        if(offsetIndicesSize > mIndices.size()){
            throw "Index size exceeds reserved capacity.";
        }
        
        i = 0;
        while (mIndexIndices < offsetIndicesSize) {
            mIndices[mIndexIndices] = mIndexVertices + meshIndices[i];
            ++mIndexIndices;
            ++i;
        }
        
        i = 0;
        while (mIndexVertices < offsetVerticesSize) {
            mVertices[mIndexVertices].set(meshVertices[i]);
            ++mIndexVertices;
            ++i;
        }
        
        
        i = 0;
        while (mIndexNormals < offsetNormalsSize) {
            mNormals[mIndexNormals].set(meshNormals[i]);
            ++mIndexNormals;
            ++i;
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
    
    size_t getNumVertices(){
        return mVertices.size();
    }
    
    size_t getNumNormals(){
        return mNormals.size();
    }
    
    size_t getNumIndices(){
        return mIndices.size();
    }
    
    size_t getNumVerticesUsed(){
        return mIndexVertices;
    }
    
    size_t getNumNormalsUsed(){
        return mIndexNormals;
    }
    
    size_t getNumIndicesUsed(){
        return mIndexIndices;
    }
    
    
    
};



#endif
