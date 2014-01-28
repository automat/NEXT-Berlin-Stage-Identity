//
//  FragmentSurface.cpp
//  Fragment_00
//
//  Created by Henryk Wollik on 23/01/14.
//
//

#include "FragmentSurface.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include <math.h>


FragmentSurface::FragmentSurface(){
    this->init();

    ci::ImageSourceRef srcRef = ci::loadImage(ci::app::loadResource("Artboard.png"));
    mTexture = ci::gl::Texture(srcRef);
    mTexture.setWrap(GL_REPEAT, GL_REPEAT);
};

void FragmentSurface::init(size_t resolutionXY){
    mNumPointsX = resolutionXY;
    mNumPointsY = resolutionXY;
    mNumPoints  = mNumPointsX * mNumPointsY;

    size_t numCellsX = mNumPointsX - 1;
    size_t numCellsY = mNumPointsY - 1;
    size_t numCells  = numCellsX * numCellsY;

    size_t numPointsX_1 = mNumPointsX - 1;
    size_t numPointsY_1 = mNumPointsY - 1;
    
    mNumFieldPointsX = numCellsX * 8 + 1;
    mNumFieldPointsY = numCellsY * 8 + 1;
    mNumFieldPoints  = mNumFieldPointsX * mNumFieldPointsY;
    
    size_t numFieldPointsX_1 = mNumFieldPointsX - 1;
    size_t numFieldPointsY_1 = mNumFieldPointsY - 1;
 
    mTriangleTexcoords.resize(0);
    mTrianglePoints.resize(0);
    mTriangleCenters.resize(0);
    
    
    mTexcoords.resize(mNumPoints);
    mPoints.resize(mNumPoints);
    
    mFieldPoints.resize(mNumFieldPoints);
    mFieldTangents.resize(mNumFieldPoints);
    mFieldTangentIndex.resize(0);



    float fj, fi, fjn, fin, index, index6,
          fNumPointsX_1 = (float)numPointsX_1,
          fNumPointsY_1 = (float)numPointsY_1;

    int i,j;
    i = -1;
    while (++i < mNumPointsY) {
        fi = (float)i;
        j = -1;
        while (++j < mNumPointsX) {
            fj = (float)j;
            index = uint(i * mNumPointsX + j);
            
            fjn = fj / fNumPointsX_1;
            fin = fi / fNumPointsY_1;

            mPoints[index].set(-0.5 + fjn,0.0f, -0.5 + fin);
            mTexcoords[index].set(fjn,fin);
        }
    }
    
    i = -1;
    while (++i < mNumFieldPointsY) {
        fi = (float)i;
        j = -1;
        while (++j < mNumFieldPointsX) {
            fj = (float)j;
            index = uint(i * mNumFieldPointsX + j);
            
            fjn = fj / numFieldPointsX_1;
            fin = fi / numFieldPointsY_1;
            
            mFieldPoints[index].set(-0.5 + fjn,0.0f,-0.5f + fin);
            
            
        }
    }
    
    
    
    
    
    

    uint indices[4]; //tr tl bl br
    
    ci::Vec3f  centerPoint;
    ci::Vec2f  centerTexcoord;
    ci::Vec3f* tl;
    ci::Vec3f* tr;
    ci::Vec3f* bl;
    ci::Vec3f* br;
    ci::Vec2f* ttl;
    ci::Vec2f* ttr;
    ci::Vec2f* tbl;
    ci::Vec2f* tbr;
    
    uint indexX,indexY;
    
    i = -1;
    while(++i < numCellsY){
        j = -1;
        while(++j < numCellsX){
            index  = i * mNumPointsX + j;
            index6 = index * 6;
            
            indices[0] = index; //tl
            indices[1] = indices[0] + 1; //tr
            indices[2] = uint((i + 1) * mNumPointsX + j); //bl
            indices[3] = indices[2] + 1; // br
            
            tl = &(mPoints[indices[0]]);
            tr = &(mPoints[indices[1]]);
            bl = &(mPoints[indices[2]]);
            br = &(mPoints[indices[3]]);
            
            ttl = &(mTexcoords[indices[0]]);
            ttr = &(mTexcoords[indices[1]]);
            tbl = &(mTexcoords[indices[2]]);
            tbr = &(mTexcoords[indices[3]]);
            
            centerPoint    = (*tr + *tl + *bl + *br) / 4;
            centerTexcoord = (*ttr + *ttl + *tbl + *tbr) / 4;
            
            /*----------------------------------------------------------------------------------------------------*/
            
            //0
            mTrianglePoints.push_back(*tl);
            mTrianglePoints.push_back(centerPoint);
            mTrianglePoints.push_back(*bl);
            
            mTriangleCenters.push_back( (*tl + centerPoint + *bl) / 3);
            indexX = (0.5f + mTriangleCenters.back().x) * numFieldPointsX_1;
            indexY = (0.5f + mTriangleCenters.back().z) * numFieldPointsY_1;
            mFieldTangentIndex.push_back(uint(indexY * mNumFieldPointsX + indexX));

            mTriangleTexcoords.push_back(*ttl);
            mTriangleTexcoords.push_back(centerTexcoord);
            mTriangleTexcoords.push_back(*tbl);
            
            
            /*----------------------------------------------------------------------------------------------------*/
            
            
            //1
            mTrianglePoints.push_back(*tl);
            mTrianglePoints.push_back(*tr);
            mTrianglePoints.push_back(centerPoint);

            mTriangleCenters.push_back( (*tl + *tr + centerPoint) / 3);
            indexX = (0.5f + mTriangleCenters.back().x) * numFieldPointsX_1;
            indexY = (0.5f + mTriangleCenters.back().z) * numFieldPointsY_1;
            mFieldTangentIndex.push_back(uint(indexY * mNumFieldPointsX + indexX));
            
            mTriangleTexcoords.push_back(*ttl);
            mTriangleTexcoords.push_back(*ttr);
            mTriangleTexcoords.push_back(centerTexcoord);
            
            
            
            /*----------------------------------------------------------------------------------------------------*/
            
            
            //2
            mTrianglePoints.push_back(*tr);
            mTrianglePoints.push_back(*br);
            mTrianglePoints.push_back(centerPoint);
            mTriangleCenters.push_back( (*tr + *br + centerPoint) / 3);

            
            mTriangleCenters.push_back( (*tr + *br + centerPoint) / 3);
            indexX = (0.5f + mTriangleCenters.back().x) * numFieldPointsX_1;
            indexY = (0.5f + mTriangleCenters.back().z) * numFieldPointsY_1;
            mFieldTangentIndex.push_back(uint(indexY * mNumFieldPointsX + indexX));
            
            mTriangleTexcoords.push_back(*ttr);
            mTriangleTexcoords.push_back(*tbr);
            mTriangleTexcoords.push_back(centerTexcoord);
            
            
            /*----------------------------------------------------------------------------------------------------*/
            
            
            //3
            
            mTrianglePoints.push_back(centerPoint);
            mTrianglePoints.push_back(*br);
            mTrianglePoints.push_back(*bl);
            
            
            mTriangleCenters.push_back( (centerPoint + *br + *bl) / 3);
            indexX = (0.5f + mTriangleCenters.back().x) * numFieldPointsX_1;
            indexY = (0.5f + mTriangleCenters.back().z) * numFieldPointsY_1;
            mFieldTangentIndex.push_back(uint(indexY * mNumFieldPointsX + indexX));
        
             
            mTriangleTexcoords.push_back(centerTexcoord);
            mTriangleTexcoords.push_back(*tbr);
            mTriangleTexcoords.push_back(*tbl);
            
            
        }
    }
};

void FragmentSurface::draw(){
    static const float quadVerts[12] = {-0.5,0,-0.5,
                                          0.5,0,-0.5,
                                          0.5,0,0.5,
                                         -0.5,0,0.5};

    glColor3f(1.0f, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &quadVerts[0]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    
    /*----------------------------------------------------------------------------------------------*/
    
    glColor3f(1,1,1);
    mTexture.enableAndBind();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mTrianglePoints[0].x);
    glTexCoordPointer(2, GL_FLOAT, 0, &mTriangleTexcoords[0].x);
    glDrawArrays(GL_TRIANGLES, 0, mTrianglePoints.size());
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    mTexture.disable();
    
    
    glPushMatrix();
    glTranslatef(0, 0.0125f, 0);
    
    
    glPointSize(2);
    glColor3f(1.0f, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mFieldPoints[0].x);
    glDrawArrays(GL_POINTS, 0, mFieldPoints.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPointSize(5);
    glColor3f(1.0f, 0.5f, 0.5f);;
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mTriangleCenters[0].x);
    glDrawArrays(GL_POINTS, 0, mTriangleCenters.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
    
  
    glColor3f(1.0f, 0, 1.0f);
    int index;
    int i = -1;
    while (++i < mFieldTangentIndex.size()) {
        index = mFieldTangentIndex[i];
        
        
        
        ci::gl::drawVector(mFieldPoints[index], mFieldPoints[index] + mFieldTangents[index],0.0125f,0.005f);
    }
    
    
   
    glPopMatrix();
     
   
};

void FragmentSurface::update(float t){
    ci::Vec3f* tangent;
    int i,j;
    float fin,fjn;
    float angle;
    i = -1;
    while (++i < mNumFieldPointsY) {
        j = -1;
        while (++j < mNumFieldPointsX) {
            tangent = &mFieldTangents[i * mNumFieldPointsX + j];
            fjn = -0.5f+(float)j / (float)(mNumFieldPointsX-1);
            fin = -0.5f+(float)i / (float)(mNumFieldPointsY-1);
            
            angle = fjn * fin * (float)M_2_PI * 64 + t;
            
            tangent->x = 0.0;//std::cosf(angle) * 0.0125f;
            tangent->y = 0.0;
            tangent->z = std::sinf(angle) * 0.0125f;
        }
    }
    
    
    i = -1;
    while (++i < mFieldTangentIndex.size()) {
        mTriangleTexcoords[i*3]  -=mFieldTangents[mFieldTangentIndex[i]].xz();
        mTriangleTexcoords[i*3+1]-=mFieldTangents[mFieldTangentIndex[i]].xz();
        mTriangleTexcoords[i*3+2]-=mFieldTangents[mFieldTangentIndex[i]].xz();
        
       // mTrianglePoints[i*3  ].y+=std::atan2f(mFieldTangents[mFieldTangentIndex[i]].z,mFieldTangents[mFieldTangentIndex[i]].x)*0.0025f;
       // mTrianglePoints[i*3+1].y+=std::atan2f(mFieldTangents[mFieldTangentIndex[i]].z,mFieldTangents[mFieldTangentIndex[i]].x)*0.0025f;
       // mTrianglePoints[i*3+2].y+=std::atan2f(mFieldTangents[mFieldTangentIndex[i]].z,mFieldTangents[mFieldTangentIndex[i]].x)*0.0025f;
        
        
    }
    
    
    
    /*
    int i3;
    i = -1;
    while (++i < mFieldTangents.size()) {
        i3 = i * 3;
        mTriangleTexcoords[i3  ]+=mFieldTangents[i].xz();
        mTriangleTexcoords[i3+1]+=mFieldTangents[i].xz();
        mTriangleTexcoords[i3+2]+=mFieldTangents[i].xz();
    }
    */

    
};