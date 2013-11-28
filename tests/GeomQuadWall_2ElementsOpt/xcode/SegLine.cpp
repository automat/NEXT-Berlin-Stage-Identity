//
//  SegLine.cpp
//  TriangleGeometry
//
//  Created by Henryk Wollik on 27/11/13.
//
//

#include "SegLine.h"

SegLine::SegLine(int numPoints, int numSegments) :
mNumSegments(numSegments),
SegSkeleton(numPoints){
    init();
}

SegLine::SegLine(std::vector<ci::Vec3f>* path, int numPoints, int numSegments) :
mNumSegments(numSegments),
SegSkeleton(path,numPoints){
    init();
}

void SegLine::init(){
    int numPoints = this->getNumPoints();
    int len       = numPoints * mNumSegments;
    
    this->getVertices().resize(len);
    this->getNormals().resize(len);
    
    std::vector<uint>& indices = this->getIndices();
    
    int i;
    
    if(mNumSegments > 2){
        int v0,v1,v2,v3;
        int offset;
        int offsetLen;
        int numSegments_1 = mNumSegments - 1;
        int numPoints_1   = numPoints - 1;

        i = 0;
        while (i < numPoints_1) {
            offset    = i * mNumSegments;
            offsetLen = offset + numSegments_1;

            while (offset < offsetLen) {
                v0 = offset;
                v1 = offset + 1;
                v2 = offset + mNumSegments + 1;
                v3 = offset + mNumSegments;
                
                indices.push_back(v0);
                indices.push_back(v1);
                indices.push_back(v3);
                
                indices.push_back(v1);
                indices.push_back(v2);
                indices.push_back(v3);
                
                ++offset;
            }
            
            offset = i * mNumSegments;
            
            v0 = offset + numSegments_1;
            v1 = offset;
            v2 = offset + numSegments_1 + 1;
            v3 = offset + mNumSegments + numSegments_1;
            
            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v3);
            
            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);
 
            ++i;
        }
    }else{
        
    }
}

void SegLine::update_Internal(){

    int numPoints = this->getNumPoints();
    std::vector<ci::Vec3f>& vertices = this->getVertices();
    ci::Vec3f tempVec;
    
    float angleRatio = (float)M_PI * 2 / (float)mNumSegments;
    float angleStep  = angleRatio;
    
    int offset, offsetLen;
    int i = 0;
    
    while (i < numPoints) {
        angleStep = angleRatio;
        offset    = i * mNumSegments;
        offsetLen = offset + mNumSegments;
       
        this->getRotationMatrix(i, &mTempMatrix);
        mTempMatrix.setTranslate(mPoints[i]);
        
        while (offset < offsetLen) {
            tempVec.z   = cosf(angleStep) * mObjScale.x;
            tempVec.y   = sinf(angleStep) * mObjScale.y;
            vertices[offset] = mTempMatrix.transformPoint(tempVec);
            
            angleStep += angleRatio;
            ++offset;
        }
        ++i;
    }
    
    this->recalculateNormals();
}

void SegLine::draw(){
    std::vector<ci::Vec3f>& vertices = this->getVertices();
    std::vector<uint>&      indices  = this->getIndices();
    std::vector<ci::Vec3f>& normals  = this->getNormals();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3,GL_FLOAT, 0, &vertices[0].x);
    glNormalPointer(GL_FLOAT, 0, &normals[0].x);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}




