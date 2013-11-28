//
//  LineTriMesh.cpp
//  PRDCB
//
//  Created by Henryk Wollik on 19/11/13.
//
//

#include "LineTriMesh.h"

namespace pr {
    namespace geom{
        
        /*---------------------------------------------------------------------------------------------------------*/

        LineTriMesh::LineTriMesh() :
        mNumSegments(10),
        mRadius(0.25f),
        mCloseCaps(false),
        ci::TriMesh(){
            
        }
        
        LineTriMesh::LineTriMesh(const std::vector<ci::Vec3f>& points,
                                 size_t numSegments,
                                 float radius,
                                 bool closeCaps) :
        mNumSegments(numSegments),
        mRadius(radius),
        mCloseCaps(closeCaps),
        ci::TriMesh(){
            this->setPoints(points);
        }

        /*---------------------------------------------------------------------------------------------------------*/
        
        void LineTriMesh::setUnitRadius(float radius){
            int offset = mNumSegments * 2;
            int i = 0;
            size_t l = mPoints.size() * offset;
            while (i < l) {
                mVerticesNormal[i  ] *= radius;
                mVerticesNormal[i+1] *= radius;
                i+=2;
            }
        }
        
        void LineTriMesh::setRadius(size_t index, float radius){
            int offset = mNumSegments * 2;
            int i = index * offset;
            size_t l = i + offset;
            while (i < l) {
                mVerticesNormal[i  ] *= radius;
                mVerticesNormal[i+1] *= radius;
                i+=2;
            }
        }
        
        void LineTriMesh::setNumSegments(size_t num){
            num = num < 2 ? 2 : num;
            size_t numPoints = mPoints.size();
            
            std::vector<uint>& indices = this->getIndices();
            indices.clear();
            
            int i,j;
            int index;
            size_t len;
            
            
            if(num > 2){
                len = num - 1;
                i = -1;
                int indexSeg;
                
                int v0;
                int v1;
                int v2;
                int v3;
                
                while (++i < numPoints) {
                    index = i * mNumSegments;
                    j = -1;
                    while (++j < len) {
                        indexSeg = index + j;
                        v0 = indexSeg;
                        v1 = indexSeg + 1;
                        v2 = indexSeg + mNumSegments + 1;
                        v3 = indexSeg + mNumSegments;
                        
                        indices.push_back(v0);
                        indices.push_back(v1);
                        indices.push_back(v3);
                        
                        indices.push_back(v1);
                        indices.push_back(v2);
                        indices.push_back(v3);
                    }
                    
                    v0 = index + len;
                    v1 = index;
                    v2 = index + len + 1;
                    v3 = index + mNumSegments + len;
                    
                    indices.push_back(v0);
                    indices.push_back(v1);
                    indices.push_back(v3);
                    
                    indices.push_back(v1);
                    indices.push_back(v2);
                    indices.push_back(v3);
                }
            }
            else{
                i = -1;
                while (++i < numPoints - 1) {
                    index = i * 2;
                    
                    indices.push_back(index  );
                    indices.push_back(index+1);
                    indices.push_back(index+2);
                    
                    indices.push_back(index+1);
                    indices.push_back(index+3);
                    indices.push_back(index+2);
                }
            }
            
            std::vector<ci::Vec2f>& texCoords = this->getTexCoords();
            
            float nh, nv;
            
            
            len = numPoints * mNumSegments;
            i = -1;
            while (++i < numPoints) {
                index = i * mNumSegments;
                nh = (float)i/(float)(numPoints - 1);
                
                j = -1;
                while (++j < mNumSegments) {
                    nv = 1.0f - (float)j/(float)(mNumSegments - 1);
                    texCoords[index + j].set(nh,nv);
                }
            }
            
            this->setCloseCaps(mCloseCaps);
            this->applySliceFunc();
            
            
        }
        
        void LineTriMesh::setCloseCaps(bool close){
            if (mNumSegments == 2)return;
            
            std::vector<uint>& indices = this->getIndices();
            std::vector<uint> temp = this->getIndices();
            
            int i;
            size_t len;
            
            if(close){
                len = mNumSegments - 2;
                i = -1;while(++i < len){
                    temp.push_back(0  );
                    temp.push_back(i+1);
                    temp.push_back(i+2);
                }
                
                int j;
                len+=(mPoints.size() - 1) * mNumSegments + 1;
                i = j = len - mNumSegments + 1;
                while (++i < len) {
                    temp.push_back(j  );
                    temp.push_back(i  );
                    temp.push_back(i+1);
                }
            }else{
                temp.erase(temp.end() - (mNumSegments - 2) * 2);
            }
            
            indices.resize(temp.size());
            i = -1;
            while (++i < indices.size()) {
                indices[i] = temp[i];
            }
            
            this->recalculateNormals();
            mCloseCaps = close;
        }
        
        void LineTriMesh::setPoints(const std::vector<ci::Vec3f>& points){
            this->clear();
            mPoints = points;
            if (mPoints.size() > 0) {
                size_t num = mPoints.size() * mNumSegments;
                mVerticesNormal.resize(num * 2);
                this->getVertices().resize(num);
                this->getNormals().resize(num);
                this->getColorsRGBA().resize(num);
            }
        }
        
        void LineTriMesh::setPoint(size_t index, float x, float y, float z){
            mPoints[index].set(x,y,z);
        }
        
        void LineTriMesh::setPoint(size_t index, const ci::Vec3f& point){
            mPoints[index].set(point);
        }
        
        void LineTriMesh::appendPoint(const ci::Vec3f& point){
            mPoints.push_back(point);
        }
        
        void LineTriMesh::appendPoint(float x, float y, float z){
            mPoints.push_back(ci::Vec3f(x,y,z));
        }
        
        void LineTriMesh::appendPoints(const ci::Vec3f* points, size_t num){
            mPoints.insert(mPoints.end(), points, points + num);
        }
        
        
        void LineTriMesh::applySliceFunc(){
            
        }
        
        void LineTriMesh::update(){
            
        }
        
        void LineTriMesh::clear(){
            ci::TriMesh::clear();
            mVerticesNormal.clear();
            mPoints.clear();
        }
      
        
        /*---------------------------------------------------------------------------------------------------------*/
        
        
        void LineTriMesh::getPoint(size_t index, ci::Vec3f* point){
            *point = mPoints[index];
        }
        
        const std::vector<ci::Vec3f>& LineTriMesh::getPoints() const{
            return mPoints;
        }
        
        std::vector<ci::Vec3f>& LineTriMesh::getPoints(){
            return mPoints;
        }
        
        size_t LineTriMesh::getNumPoints(){
            return mPoints.size();
        }
        
        size_t LineTriMesh::getNumSegments(){
            return mNumSegments;
        }
    }
}
