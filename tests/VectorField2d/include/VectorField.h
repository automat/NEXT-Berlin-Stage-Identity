//
//  VectorField.h
//  VectorField2d
//
//  Created by Henryk Wollik on 22/11/13.
//
//

#ifndef VectorField2d_VectorField_h
#define VectorField2d_VectorField_h

#include "cinder/Vector.h"
#include <vector>
#include <math.h>
#include <iostream>
#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

struct Particle{
    ci::Vec3f pos;
    int age;
    ci::Vec3f vel;
    
    int minAge;
    int maxAge;
    
    float speed;
    
    Particle() :
    minAge(100) ,
    maxAge(600){
        
        reset();
    }
    
    void reset(){
        pos = ci::Vec3f(ci::randFloat() - 0.5f, 0.0f, ci::randFloat() - 0.5f);
        age = ci::randInt(minAge,maxAge);
        vel = ci::Vec3f(ci::randFloat(-1, 1), 0.0f, ci::randFloat(-1, 1));
        speed = 0.0025f;
    }
    
    void reset(const ci::Vec3f& resetPos){
        
        //pos.set(resetPos);
        pos = ci::Vec3f(ci::randFloat() - 0.5f, 0.0f, ci::randFloat() - 0.5f);
        
        age = ci::randInt(minAge,maxAge);
        vel = ci::Vec3f(ci::randFloat(-1, 1), 0.0f, ci::randFloat(-1, 1));
        speed = 0.0025f;
    }
    
    bool isDead(){
        return age < 0;
    }
};

struct Magnet{
    ci::Vec3f posPositive;
    ci::Vec3f posNegative;
    float strength;
    
    Magnet(const ci::Vec3f& poleP, const ci::Vec3f& poleN, float s = 10.0f) :
    posPositive(poleP),
    posNegative(poleN),
    strength(s){}
};

class VectorField {
private:
    int mSizeX;
    int mSizeY;
    int mSizeX_1;
    int mSizeY_1;
    float mScale;
    int mNumParticles;
    
    std::vector<ci::Vec3f> mPoints;
    std::vector<ci::Vec3f> mTangents;
    std::vector<Particle>  mParticles;
    std::vector<Magnet>    mMagnets;
    
    std::vector<ci::Vec3f> mParticleDrawVertices;
    std::vector<ci::Colorf>mParticleDrawColors;
    
    void init(){
        mSizeX = 180;
        mSizeY = 180;
        
        mScale = 1.0f;
        mSizeX_1 = mSizeX - 1;
        mSizeY_1 = mSizeY - 1;
        mNumParticles = 5000;
        
        int len = mSizeX * mSizeY;
        
        mPoints.resize(len);
        mTangents.resize(len);
        mParticles.resize(mNumParticles);
        mParticleDrawVertices.resize(mNumParticles * 2);
        mParticleDrawColors.resize(mNumParticles * 2);
        
        //! set point bundle in unit dim -0.5,0.5 * scale
        int i, j;
        float ni,nj;
        i = -1;
        while (++i < mSizeY) {
            j = -1;
            ni = (float)i / (float)(mSizeY-1);
            while (++j < mSizeX) {
                nj = (float) j /(float)(mSizeX-1);
                mPoints[j + i * mSizeY].set((-0.5f + nj) * mScale, 0.0f,(-0.5f + ni) * mScale);
            }
        }
        
        int num = 6;
        float angleStep = (float)M_PI * 2 / ((float)num * 2);
        float radius = 0.25f;
        float r;
        i =0;
        while (i < num  *2) {
            r = radius + radius * ((i/2)%2==1);
            mMagnets.push_back(Magnet(ci::Vec3f(sinf(angleStep * (float)i)*r,0,cosf(angleStep * (float)i)*r),
                                      ci::Vec3f(sinf(angleStep * (float)(i+1))*r,0,cosf(angleStep * (float)(i+1))*r),
                                      10));
            i+=2;
                               
        }
        
        
        
    }
    
    
public:
    VectorField(){
        init();
    }
    
    //! Resets tangent bundle
    void clear(){
        int i = -1;
        while (++i < mTangents.size()) {
            mTangents[i].x = mTangents[i].z = 0.0f;
        }
    }
    
    void update(float t = 0.0f){
        int i, j, k;
        int index;
        float ni, nj;
        float scale_2 = mScale * 0.5f;
        float x,y;
     
        //! Update tangent bundle
        i = -1;
        while (++i < mSizeY) {
            j = -1;
            ni = (float)i / (float)(mSizeY-1);
            while (++j < mSizeX) {
                nj = (float) j /(float)(mSizeX-1);
                index = j + i * mSizeY;
                x = (-0.5f + nj);
                y = (-0.5f + ni);
                ci::Vec3f& tangent = mTangents[index];
                tangent.x += x ;//+ cosf(x * (float)M_PI * 16 + t*0.25f) * 0.825f;
                tangent.z += y ;//+ sinf(y * (float)M_PI * 16+ t*0.25f) * 0.825f;
                tangent.normalize();
            }
        }
        
        
        
        //! this just measures tangents, multiplied by inverted distance * strength
        //! Update fake magnets
        ci::Vec3f vecMagPointP; //vec to pole positive
        ci::Vec3f vecMagPointN; //vec to pole negative
        float invDistP;
        float invDistN;
        i = -1;
        while (++i < mMagnets.size()) {
            Magnet& magnet = mMagnets[i];
            
            magnet.strength = 5.0f + abs(sinf((float)i/(float)(mMagnets.size()) * (float)M_PI * 2 + t*0.25f)*20);
            
            k = -1;
            while (++k < mPoints.size()) {
                vecMagPointP = mPoints[k] - magnet.posPositive;
                vecMagPointN = mPoints[k] - magnet.posNegative;
                
                invDistP = 1.0f / vecMagPointP.length();
                invDistN = 1.0f / vecMagPointN.length();
                
                vecMagPointP.normalize();
                vecMagPointN.normalize();
                vecMagPointN*=-1;
                
                
                vecMagPointP *= invDistP * magnet.strength;
                vecMagPointN *= invDistN * magnet.strength;
                
                ci::Vec3f& tangent = mTangents[k];
                
                tangent += vecMagPointP;
                tangent += vecMagPointN;
                
            }
            
            
            
            
        }
        
        //! Update particles
        k = -1;
        while (++k < mParticles.size()) {
            Particle& p = mParticles[k];
            p.age--;
            //!get tangent bundle index according to position
            i = (int)((p.pos.x + 0.5f * mScale) * (mSizeX_1));
            j = (int)((p.pos.z + 0.5f * mScale) * (mSizeY_1));
            
            if (p.pos.x < -scale_2 || p.pos.x > scale_2 ||
                p.pos.z < -scale_2 || p.pos.z > scale_2 ||
                p.isDead()){
                p.reset();
                continue;
            }
            index = i + j * mSizeY;
            p.vel += mTangents[index];
            p.vel.normalize();
            p.vel *= p.speed;
            p.pos += p.vel;
        }
    }
    
    
    //! Draw the shits
    void draw(){
        glColor3f(0.25f,0.25f,0.25f);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
        glDrawArrays(GL_POINTS, 0, mPoints.size());
        glDisableClientState(GL_VERTEX_ARRAY);


        std::vector<ci::Vec3f> vertices;
        vertices.resize(mTangents.size() * 2);
        
        int i = -1;
        while (++i < mTangents.size()) {
            vertices[i*2  ].set(mPoints[i]);
            vertices[i*2+1].set(mPoints[i] + mTangents[i].normalized() * 0.0125f);
        }
       
        /*
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
        glDrawArrays(GL_LINES, 0, vertices.size());
        glDisableClientState(GL_VERTEX_ARRAY);
*/
    
        /*
        int index;
        i = -1;
        while (++i < mParticles.size()) {
            index = i * 2;

            mParticleDrawColors[index  ].set(1, 0, 0);
            mParticleDrawColors[index+1].set(1, 0, 1);
            
            mParticleDrawVertices[index  ].set( mParticles[i].pos);
            mParticleDrawVertices[index+1].set( mParticles[i].pos + mParticles[i].vel * 2);
        }
       */
        /*
        glColor3f(1, 1, 1);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mParticleDrawVertices[0].x);
        glColorPointer(3, GL_FLOAT, 0, &mParticleDrawColors[0].r);
        glDrawArrays(GL_LINES, 0, mParticleDrawVertices.size());
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        */
       
      
        
        
        
        glColor3f(1, 1, 1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(Particle), &mParticles[0].pos.x);
        glDrawArrays(GL_POINTS, 0, mParticles.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        
    
        float sphereRadius = 0.005f;
   
        i = -1;
        while (++i < mMagnets.size()) {
            glColor3f(0,0,1);
            ci::gl::drawSphere(mMagnets[i].posPositive, sphereRadius);
            glColor3f(1,0,0);
            ci::gl::drawSphere(mMagnets[i].posNegative, sphereRadius);
            glColor3f(1, 1, 1);
            ci::gl::drawVector(mMagnets[i].posPositive, mMagnets[i].posNegative,sphereRadius*0.5f,sphereRadius*0.25f);
        }
        
       
    }
    

};



#endif
