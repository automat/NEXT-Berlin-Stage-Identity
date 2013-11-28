//
//  SegCube.cpp
//  TriangleGeometry
//
//  Created by Henryk Wollik on 27/11/13.
//
//

#include "SegCube.h"

SegCube::SegCube():
SegSkeleton(){
    init();
}

SegCube::SegCube(std::vector<ci::Vec3f>* path) :
SegSkeleton(path){
    init();
}

void SegCube::init(){
    this->getVertices().resize(24);
    this->getNormals().resize(24);
    
    float tindices[36] = {
        0,1,2,
        0,2,3,
        4,5,6,
        4,6,7,
        8,9,10,
        8,10,11,
        12,13,14,
        12,14,15,
        16,17,18,
        16,18,19,
        20,21,22,
        20,22,23
    };
    
    std::vector<uint>& indices = this->getIndices();
    
    int i = 0;
    while (i < 36) {
        indices.push_back(tindices[i  ]);
        indices.push_back(tindices[i+1]);
        indices.push_back(tindices[i+2]);
        i+=3;
    }
    
    
}

void SegCube::update_Internal(){
    this->getRotationMatrix(0, &mTempMatrix);
    mTempMatrix.setTranslate(mPoints[0].lerp(0.5, mPoints[1]));
    mObjScale.x = mPoints[0].distance(mPoints[1]);
    
    static const float normVertices[72] =
    {
         0.5f, 0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f,-0.5f,-0.5f,
        
         0.5f, 0.5f,-0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f, 0.5f,-0.5f,
        
        -0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
        
        -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
        
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        
        -0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
        
         0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f,-0.5f,
        
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,
         0.5f, 0.5f,-0.5f
    };
    
    std::vector<ci::Vec3f>& vertices         = this->getVertices();
    std::vector<ci::Vec3f>::iterator itrObjV = vertices.begin();
    
    ci::Vec3f temp;
    
    int i = 0;
    int i3;
    while (itrObjV != vertices.end()) {
        i3 = i * 3;
        temp.set(normVertices[i3  ] * mObjScale.x,
                 normVertices[i3+1] * mObjScale.y,
                 normVertices[i3+2] * mObjScale.z);

        *itrObjV  = mTempMatrix.transformPoint(temp);
        ++itrObjV;
        ++i;
    }
    
    this->recalculateNormals();
}

void SegCube::draw(){
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

