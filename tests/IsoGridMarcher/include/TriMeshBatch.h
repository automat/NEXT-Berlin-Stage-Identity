//
//  TriMeshBatch.h
//  ciATMUtils
//
//  Created by Henryk Wollik on 28/11/13.
//
//

#ifndef ciATMUtils_TriMeshBatch_h
#define ciATMUtils_TriMeshBatch_h

#include "cinder/TriMesh.h"
#include <vector>
#include "cinder/Vector.h"

class TriMeshBatch {
    private:
        int mIndexVertices;
        int mIndexNormals;
        int mIndexColors;
        int mIndexTexCoords;
        int mIndexIndices;
        
        std::vector<ci::Vec3f> mVertices;
        std::vector<ci::Vec3f> mNormals;
        std::vector<ci::Color> mColors;
        std::vector<ci::Vec2f> mTexCoords;
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
            mIndexVertices  = 0;
            mIndexNormals   = 0;
            mIndexColors    = 0;
            mIndexTexCoords = 0;
            mIndexIndices   = 0;
            
        }
    
    void clear(){
        mVertices.clear();
        mNormals.clear();
        mColors.clear();
        mTexCoords.clear();
        mIndices.clear();
        this->reset();
    }
        
        void put(ci::TriMesh& mesh){
            std::vector<ci::Vec3f>& meshVertices  = mesh.getVertices();
            std::vector<ci::Vec3f>& meshNormals   = mesh.getNormals();
            std::vector<ci::Color>& meshColors    = mesh.getColorsRGB();
            std::vector<ci::Vec2f>& meshTexCoords = mesh.getTexCoords();
            std::vector<uint>&      meshIndices   = mesh.getIndices();
            
            
            int offsetVerticesSize  = mIndexVertices  + meshVertices.size();
            int offsetNormalsSize   = mIndexNormals   + meshNormals.size();
            int offsetColorsSize    = mIndexColors    + meshColors.size();
            int offsetTexCoordsSize = mIndexTexCoords + meshTexCoords.size();
            int offsetIndicesSize   = mIndexIndices   + meshIndices.size();
            
            int i = 0;
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
            
            i = 0;
            while (mIndexColors < offsetColorsSize) {
                mColors[mIndexColors].set(meshColors[i]);
                ++mIndexColors;
                ++i;
            }
            
            i = 0;
            while (mIndexTexCoords < offsetTexCoordsSize) {
                mTexCoords[mIndexTexCoords].set(meshTexCoords[i]);
                ++mIndexTexCoords;
                ++i;
            }
        }
        
        
        const std::vector<ci::Vec3f>& getVertices() const{
            return mVertices;
        };
        
        const std::vector<ci::Vec3f>& getNormals()  const{
            return mNormals;
        }
        
        const std::vector<ci::Color>& getColors()  const{
            return mColors;
        }
        
        const std::vector<ci::Vec2f>& getTexCoords()  const{
            return mTexCoords;
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
        
        std::vector<ci::Color>& getColors(){
            return mColors;
        }
        
        std::vector<ci::Vec2f>& getTexCoords(){
            return mTexCoords;
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
        
        size_t getNumColors(){
            return mColors.size();
        }
        
        size_t getNumTexCoords(){
            return mTexCoords.size();
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
        
        size_t getNumColorsUsed(){
            return mIndexColors;
        }
        
        size_t getNumTexCoordsUsed(){
            return mIndexTexCoords;
        }
        
        size_t getNumIndicesUsed(){
            return mIndexIndices;
        }
        
        
        bool hasVertices(){
            return mIndexVertices != 0;
        }
        
        bool hasNormals(){
            return mIndexNormals != 0;
        }
        
        bool hasColors(){
            return mIndexColors != 0;
        }
        
        bool hasTexCoords(){
            return mIndexTexCoords != 0;
        }
        
        bool hasIndices(){
            return mIndexIndices != 0;
        }
        
    };

#endif
