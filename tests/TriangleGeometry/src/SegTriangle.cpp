//
//  SegTriangle.cpp
//  TriangleGeometry
//
//  Created by Henryk Wollik on 28/11/13.
//
//

#include "SegTriangle.h"

SegTriangle::SegTriangle() :
SegSkeleton(2){
    this->init();
}

SegTriangle::SegTriangle(std::vector<ci::Vec3f>* path) :
SegSkeleton(path,2){
    this->init();
}

void SegTriangle::init(){
    int numVertices = 18;
    
    mNormVertices.resize(numVertices);
    this->getVertices().resize(numVertices);
    this->getNormals().reserve(numVertices);
    
    
    
    float angleRatio = (float)M_PI / 2.0f;

    float angleStep  = angleRatio;
    
    //top
    mNormVertices[ 0].set(cosf(angleRatio  )*0.5f + 0.25f,sinf(angleRatio  )*0.5f,-0.05f);
    mNormVertices[ 1].set(cosf(angleRatio*2)*0.5f + 0.25f,sinf(angleRatio*2)*0.5f,-0.05f);
    mNormVertices[ 2].set(cosf(angleRatio*3)*0.5f + 0.25f,sinf(angleRatio*3)*0.5f,-0.05f);
    
    //bottom
    mNormVertices[15].set(cosf(angleRatio  )*0.5f + 0.25f,sinf(angleRatio  )*0.5f, 0.05f);
    mNormVertices[16].set(cosf(angleRatio*2)*0.5f + 0.25f,sinf(angleRatio*2)*0.5f, 0.05f);
    mNormVertices[17].set(cosf(angleRatio*3)*0.5f + 0.25f,sinf(angleRatio*3)*0.5f, 0.05f);
    
    //sides
    
    mNormVertices[ 3].set(mNormVertices[ 0]);
    mNormVertices[ 8].set(mNormVertices[ 0]);
   
    mNormVertices[ 4].set(mNormVertices[ 1]);
    mNormVertices[ 5].set(mNormVertices[ 1]);
    
    mNormVertices[ 6].set(mNormVertices[ 2]);
    mNormVertices[ 7].set(mNormVertices[ 2]);
    
    
    
    mNormVertices[ 9].set(mNormVertices[15]);
    mNormVertices[14].set(mNormVertices[15]);
    
    mNormVertices[10].set(mNormVertices[16]);
    mNormVertices[11].set(mNormVertices[16]);
    
    mNormVertices[12].set(mNormVertices[17]);
    mNormVertices[13].set(mNormVertices[17]);
    
    std::vector<uint>& indices = this->getIndices();
    
    indices.push_back( 0);
    indices.push_back( 1);
    indices.push_back( 2);
    
    indices.push_back( 3);
    indices.push_back( 4);
    indices.push_back(10);
    
    indices.push_back( 3);
    indices.push_back(10);
    indices.push_back( 9);
    
    indices.push_back( 5);
    indices.push_back( 6);
    indices.push_back(12);
    
    indices.push_back( 5);
    indices.push_back(12);
    indices.push_back(11);
    
    indices.push_back( 7);
    indices.push_back( 8);
    indices.push_back(14);
    
    indices.push_back( 7);
    indices.push_back(14);
    indices.push_back(13);
    
    
    
    indices.push_back(15);
    indices.push_back(16);
    indices.push_back(17);


    

   /*
    int i = -1;
    while (++i < numVertices) {
        mNormVertices[i].set(cosf(angleStep)*0.5f + 0.25f,sinf(angleStep)*0.5f,-0.5f);
        angleStep+=angleRatio;
    }
    */
}

void SegTriangle::update_Internal(){
    this->getRotationMatrix(0, &mTempMatrix);
    mTempMatrix.setTranslate(mPoints[0].lerp(0.5, mPoints[1]));
    float dist = mPoints[0].distance(mPoints[1]);
    ci::Vec3f scale = mObjScale * 1;
    
    std::vector<ci::Vec3f>& vertices         = this->getVertices();
    std::vector<ci::Vec3f>::iterator itrObjV = vertices.begin();
    std::vector<ci::Vec3f>::iterator itrObjN = mNormVertices.begin();
    
    ci::Vec3f temp;
    

    while (itrObjV != vertices.end()) {
        temp.set(*itrObjN * scale);
        *itrObjV  = mTempMatrix.transformPoint(temp);
        ++itrObjV;
        ++itrObjN;
    }
    
    this->recalculateNormals();


}

void SegTriangle::draw(){
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
