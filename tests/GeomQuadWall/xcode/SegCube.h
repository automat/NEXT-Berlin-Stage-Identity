//
//  ParticleCube.h
//  TriangleGeometry
//
//  Created by Henryk Wollik on 26/11/13.
//
//

#ifndef TriangleGeometry_ParticleCube_h
#define TriangleGeometry_ParticleCube_h

#include "Skeleton.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include <vector>


class SegCube : public Skeleton{
private:
    std::vector<ci::Vec3f> mNormVertices;
    std::vector<ci::Vec3f> mScaledVertices;
    
    void init(){
        
        mNormVertices.push_back(ci::Vec3f( 0.5f, 0.5f, 0.5f)); //0 //0
        mNormVertices.push_back(ci::Vec3f( 0.5f,-0.5f, 0.5f)); //0 //1
        mNormVertices.push_back(ci::Vec3f( 0.5f,-0.5f,-0.5f)); //0 //2
        
        mNormVertices.push_back(ci::Vec3f( 0.5f, 0.5f,-0.5f)); //1 //3
        mNormVertices.push_back(ci::Vec3f( 0.5f, 0.5f, 0.5f)); //1 //4
        mNormVertices.push_back(ci::Vec3f( 0.5f, 0.5f,-0.5f)); //1 //5
        
        mNormVertices.push_back(ci::Vec3f(-0.5f, 0.5f,-0.5f)); //2 //6
        mNormVertices.push_back(ci::Vec3f(-0.5f, 0.5f, 0.5f)); //2 //7
        mNormVertices.push_back(ci::Vec3f( 0.5f, 0.5f, 0.5f)); //2 //8
        
        mNormVertices.push_back(ci::Vec3f(-0.5f, 0.5f, 0.5f)); //3 //9
        mNormVertices.push_back(ci::Vec3f(-0.5f,-0.5f, 0.5f)); //3 //10
        mNormVertices.push_back(ci::Vec3f( 0.5f,-0.5f, 0.5f)); //3 //11
        
        mNormVertices.push_back(ci::Vec3f(-0.5f, 0.5f, 0.5f)); //4 //12
        mNormVertices.push_back(ci::Vec3f(-0.5f, 0.5f,-0.5f)); //4 //13
        mNormVertices.push_back(ci::Vec3f(-0.5f,-0.5f,-0.5f)); //4 //14
        
        mNormVertices.push_back(ci::Vec3f(-0.5f,-0.5f, 0.5f)); //5 //15
        mNormVertices.push_back(ci::Vec3f(-0.5f,-0.5f,-0.5f)); //5 //16
        mNormVertices.push_back(ci::Vec3f( 0.5f,-0.5f,-0.5f)); //5 //17
        
        mNormVertices.push_back(ci::Vec3f( 0.5f,-0.5f, 0.5f)); //6 //18
        mNormVertices.push_back(ci::Vec3f(-0.5f,-0.5f, 0.5f)); //6 //19
        mNormVertices.push_back(ci::Vec3f( 0.5f,-0.5f,-0.5f)); //6 //20
        
        mNormVertices.push_back(ci::Vec3f(-0.5f,-0.5f,-0.5f)); //7 //21
        mNormVertices.push_back(ci::Vec3f(-0.5f, 0.5f,-0.5f)); //7 //22
        mNormVertices.push_back(ci::Vec3f( 0.5f, 0.5f,-0.5f)); //7 //23
         
        
        mScaledVertices.resize(mNormVertices.size());
        
        this->getVertices().resize(mNormVertices.size());

        this->setScale(1,1,1);
        
        std::vector<uint>& indices = this->getIndices();
        //top
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);
        
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(4);
        indices.push_back(6);
        indices.push_back(7);
        
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(8);
        indices.push_back(10);
        indices.push_back(11);
        
        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);
        indices.push_back(12);
        indices.push_back(14);
        indices.push_back(15);
        
        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        indices.push_back(16);
        indices.push_back(18);
        indices.push_back(19);
        
        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);
        indices.push_back(20);
        indices.push_back(22);
        indices.push_back(23);
        
        
        std::vector<ci::Vec3f>& normals =  this->getNormals();
        
        normals.push_back(ci::Vec3f(1,0,0));
        normals.push_back(ci::Vec3f(1,0,0));
        normals.push_back(ci::Vec3f(1,0,0));
        
        normals.push_back(ci::Vec3f(1,0,0));
        normals.push_back(ci::Vec3f(0,1,0));
        normals.push_back(ci::Vec3f(0,1,0));
        
        normals.push_back(ci::Vec3f(0,1,0));
        normals.push_back(ci::Vec3f(0,1,0));
        normals.push_back(ci::Vec3f(0,0,1));
        
        normals.push_back(ci::Vec3f(0,0,1));
        normals.push_back(ci::Vec3f(0,0,1));
        normals.push_back(ci::Vec3f(0,0,1));
        
        normals.push_back(ci::Vec3f(-1,0,0));
        normals.push_back(ci::Vec3f(-1,0,0));
        normals.push_back(ci::Vec3f(-1,0,0));
        
        normals.push_back(ci::Vec3f(-1,0,0));
        normals.push_back(ci::Vec3f(0,-1,0));
        normals.push_back(ci::Vec3f(0,-1,0));
        
        normals.push_back(ci::Vec3f(0,-1,0));
        normals.push_back(ci::Vec3f(0,-1,0));
        normals.push_back(ci::Vec3f(0,-0,1));
        
        normals.push_back(ci::Vec3f(0,0,-1));
        normals.push_back(ci::Vec3f(0,0,-1));
        normals.push_back(ci::Vec3f(0,0,-1));
  
    }

protected:
    
    void update_Internal(){
        this->getRotationMatrix(0, &mTempMatrix);
        mTempMatrix.setTranslate(mPoints[0].lerp(0.5, mPoints[1]));
        mObjScale.x = mPoints[0].distance(mPoints[1]);
        
        std::vector<ci::Vec3f>& vertices          = this->getVertices();
        std::vector<ci::Vec3f>::iterator itrNormV = mNormVertices.begin();
        std::vector<ci::Vec3f>::iterator itrScldV = mScaledVertices.begin();
        std::vector<ci::Vec3f>::iterator itrObjV  = vertices.begin();
        
        while (itrObjV != vertices.end()) {
            *itrScldV = *itrNormV * mObjScale;
            *itrObjV  = mTempMatrix.transformPoint(*itrScldV);
            
            ++itrNormV;
            ++itrScldV;
            ++itrObjV;
        }
        
        this->recalculateNormals();
    }
    
public:
    SegCube() :
    Skeleton(2){
        init();
    }
    
    SegCube(std::vector<ci::Vec3f>* path) :
    Skeleton(path,2){
        init();
    }
    
    void draw(){
        std::vector<ci::Vec3f>& vertices = this->getVertices();
        std::vector<uint>&      indices  = this->getIndices();
        std::vector<ci::Vec3f>& normals  = this->getNormals();
        
        glPointSize(4);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3,GL_FLOAT, 0, &vertices[0].x);
        glNormalPointer(GL_FLOAT, 0, &normals[0].x);

        glColor3f(1, 1, 1);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
        glColor3f(1,0,1);
        
        //glDrawArrays(GL_POINTS, 0, vertices.size());
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
};

#endif
