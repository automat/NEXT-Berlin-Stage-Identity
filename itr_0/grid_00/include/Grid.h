//
//  Grid.h
//  grid_00
//
//  Created by Henryk Wollik on 22/02/14.
//
//

#ifndef grid_00_Grid_h
#define grid_00_Grid_h

#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include <math.h>
#include <vector>
#include "cinder/Utilities.h"
#include "cinder/gl/Material.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace std;

class Diver {
    Vec3f* mStart;
    Vec3f* mEnd;
    float  mWidth;
    float  mTimeStart;
    float  mOffset;
    float  mLength;
    
    
public:
    Diver(Vec3f* start = NULL, Vec3f* end = NULL, float offset = 0.0) :
        mStart(start), mEnd(end),mOffset(offset){
            mTimeStart = Rand::randFloat() * 2;
            mLength    = Rand::randFloat();
        
    }
    
    inline void draw(){
        glColor3f(0, 0, 1);
        gl::drawLine(*mStart, *mEnd);
    };
    
    inline const Vec3f& getStart() const{
        return *mStart;
    }
    
    inline const Vec3f& getEnd() const{
        return *mEnd;
    }
    
    inline float getOffset(){
        return mOffset;
    }
    
    inline void reset(){
        mOffset = Rand::randFloat() * 0.125f;
    }
    
    inline bool isOut(){
        float startz = mStart->z;
        float endz   = mEnd->z;
        return (startz >= 0.5f && endz >= 0.5f) || (startz <= -0.5f && endz <= -0.5f);
    }
    
    
    
    inline void update(float time, int index, int length){
        float n = float(index)/float(length - 1);
        float pi = (float)M_PI;

        //time = n  * 0.5f+ time * 0.125f;
        time = mTimeStart  * 0.5f+ time * 0.125f;
        
        float saw = time - floorf(time);
        
        float start = 2  - saw * 4;
        float end   = start + mLength;
        mStart->z = fmax(-0.5f, fmin(start, 0.5f));
        mEnd->z   = fmax(-0.5f, fmin(end,   0.5f));
        
    };
};


class Grid {
    int mNumPointsX;
    int mNumPointsY;
    vector<Vec3f> mPoints;
    vector<Diver*> mDivers;
    
    vector<Vec3f> mVertices;
    vector<Vec3f> mNormals;
    vector<Vec3f> mTexcoords;
    vector<uint>  mIndices;
    
    float mSpaceY;
    Vec3f mOffset;
    
    gl::Material mMaterial;
    Vec3f mPos;
    
    inline void subdivide(){
        int i,j;
        i = -1;
        float numPointsX = float(mNumPointsX);
        float numPointsY = float(mNumPointsY);
        mSpaceY = 0.0125f;
        mOffset = Vec3f(1.0f / numPointsX * 0.5f, 0.0 , 0);

        
        
        float marginX =  -0.5f + 1.0f / numPointsX * 0.5f;
        while (++i < mNumPointsY) {
            j = -1;
            while (++j < mNumPointsX) {
                mPoints.push_back(Vec3f(marginX  + (j / numPointsX),
                                        0.0,
                                        -0.5f + (i / (numPointsY - 1))));
            }
        }
        
        j = 0;
        i = -1;
        while (++i < mPoints.size() / 2) {
            Vec3f& start = mPoints[i];
            Vec3f& end   = mPoints[i + numPointsX];
            
            mDivers.push_back(new Diver(&start,&end,Rand::randFloat()*0.05f));
            
            //  top base
            Vec3f topTL = start - mOffset;
            Vec3f topTR = start + mOffset;
            Vec3f topBL = end   - mOffset;
            Vec3f topBR = end   + mOffset;
            topTL.y =
            topTR.y =
            topBL.y =
            topBR.y = mSpaceY;
            
            //  bottom base
            Vec3f bottomTL = topTL;
            Vec3f bottomTR = topTR;
            Vec3f bottomBL = topBL;
            Vec3f bottomBR = topBR;
            bottomTL.y =
            bottomTR.y =
            bottomBL.y =
            bottomBR.y = -mSpaceY;
            
            //  top
            mVertices.push_back(topTL);
            mVertices.push_back(topTR);
            mVertices.push_back(topBL);
            mVertices.push_back(topTR);
            mVertices.push_back(topBL);
            mVertices.push_back(topBR);
            
            mNormals.push_back(Vec3f(0, 1,0));
            mNormals.push_back(Vec3f(0, 1,0));
            mNormals.push_back(Vec3f(0, 1,0));
            mNormals.push_back(Vec3f(0, 1,0));
            mNormals.push_back(Vec3f(0, 1,0));
            mNormals.push_back(Vec3f(0, 1,0));
            
            //  bottom
            mVertices.push_back(bottomTL);
            mVertices.push_back(bottomTR);
            mVertices.push_back(bottomBL);
            mVertices.push_back(bottomTR);
            mVertices.push_back(bottomBL);
            mVertices.push_back(bottomBR);
            
            mNormals.push_back(Vec3f(0,-1,0));
            mNormals.push_back(Vec3f(0,-1,0));
            mNormals.push_back(Vec3f(0,-1,0));
            mNormals.push_back(Vec3f(0,-1,0));
            mNormals.push_back(Vec3f(0,-1,0));
            mNormals.push_back(Vec3f(0,-1,0));
            
            //  side top
            mVertices.push_back(topTL);
            mVertices.push_back(topTR);
            mVertices.push_back(bottomTL);
            mVertices.push_back(topTR);
            mVertices.push_back(bottomTL);
            mVertices.push_back(bottomTR);
            
            mNormals.push_back(Vec3f(1,0,0));
            mNormals.push_back(Vec3f(1,0,0));
            mNormals.push_back(Vec3f(1,0,0));
            mNormals.push_back(Vec3f(1,0,0));
            mNormals.push_back(Vec3f(1,0,0));
            mNormals.push_back(Vec3f(1,0,0));
            
            //  side bottom
            mVertices.push_back(topBL);
            mVertices.push_back(topBR);
            mVertices.push_back(bottomBL);
            mVertices.push_back(topBR);
            mVertices.push_back(bottomBL);
            mVertices.push_back(bottomBR);
            
            mNormals.push_back(Vec3f(-1,0,0));
            mNormals.push_back(Vec3f(-1,0,0));
            mNormals.push_back(Vec3f(-1,0,0));
            mNormals.push_back(Vec3f(-1,0,0));
            mNormals.push_back(Vec3f(-1,0,0));
            mNormals.push_back(Vec3f(-1,0,0));
            
            //  side left
            mVertices.push_back(topBL);
            mVertices.push_back(topTL);
            mVertices.push_back(bottomBL);
            mVertices.push_back(topTL);
            mVertices.push_back(bottomTL);
            mVertices.push_back(bottomBL);
            
            mNormals.push_back(Vec3f(0,0,-1));
            mNormals.push_back(Vec3f(0,0,-1));
            mNormals.push_back(Vec3f(0,0,-1));
            mNormals.push_back(Vec3f(0,0,-1));
            mNormals.push_back(Vec3f(0,0,-1));
            mNormals.push_back(Vec3f(0,0,-1));
            
            //  side right
            mVertices.push_back(topBR);
            mVertices.push_back(topTR);
            mVertices.push_back(bottomBR);
            mVertices.push_back(topTR);
            mVertices.push_back(bottomTR);
            mVertices.push_back(bottomBR);
            
            mNormals.push_back(Vec3f(0,0,1));
            mNormals.push_back(Vec3f(0,0,1));
            mNormals.push_back(Vec3f(0,0,1));
            mNormals.push_back(Vec3f(0,0,1));
            mNormals.push_back(Vec3f(0,0,1));
            mNormals.push_back(Vec3f(0,0,1));
            

     
        }
        
    }
    
    
public:
    Grid(Vec3f pos,int numPointsX = 4, int numPointsY = 2 ) :
        mPos(pos),
        mNumPointsX(numPointsX) ,
        mNumPointsY(numPointsY){
            
            mMaterial.setDiffuse(Color(0.135f,0.135f,0.135f));
            mMaterial.setAmbient(Color(0.5f,0.15f,0.6f));
            mMaterial.setSpecular(Color(1,1,1));
            mMaterial.setShininess( 15.0f );
            this->subdivide();
    
    };
    
    inline void drawIndex(const Vec3f& pos, int index){
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(0.0075f, -0.0075f, 0.0075f);
        gl::drawString(toString(index), Vec2f::zero());
        glPopMatrix();
    }
    
    inline void update(){
        float time = app::getElapsedSeconds();
        
        int l = mDivers.size();
        int i = 0;
        int j = 0;
        for(vector<Diver*>::iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
            Diver* diver = (*itr);
            diver->update(time,j,l);
            
            const Vec3f& start = diver->getStart();
            const Vec3f& end   = diver->getEnd();
            float offset = diver->getOffset();
            
            Vec3f topTL = start - mOffset;
            Vec3f topTR = start + mOffset;
            Vec3f topBL = end   - mOffset;
            Vec3f topBR = end   + mOffset;
            topTL.y =
            topTR.y =
            topBL.y =
            topBR.y = mSpaceY + offset;
            
            Vec3f bottomTL = topTL;
            Vec3f bottomTR = topTR;
            Vec3f bottomBL = topBL;
            Vec3f bottomBR = topBR;
            bottomTL.y =
            bottomTR.y =
            bottomBL.y =
            bottomBR.y = -mSpaceY + offset;
           
           
            if(diver->isOut()){
                static const Vec3f zero(0,0,0);
                mVertices[i   ].set(zero);
                mVertices[i+ 1].set(zero);
                mVertices[i+ 2].set(zero);
                mVertices[i+ 3].set(zero);
                mVertices[i+ 4].set(zero);
                mVertices[i+ 5].set(zero);
                mVertices[i+ 6].set(zero);
                mVertices[i+ 7].set(zero);
                mVertices[i+ 8].set(zero);
                mVertices[i+ 9].set(zero);
                mVertices[i+10].set(zero);
                mVertices[i+11].set(zero);
                mVertices[i+12].set(zero);
                mVertices[i+13].set(zero);
                mVertices[i+14].set(zero);
                mVertices[i+15].set(zero);
                mVertices[i+16].set(zero);
                mVertices[i+17].set(zero);
                mVertices[i+18].set(zero);
                mVertices[i+19].set(zero);
                mVertices[i+20].set(zero);
                mVertices[i+21].set(zero);
                mVertices[i+22].set(zero);
                mVertices[i+23].set(zero);
                mVertices[i+24].set(zero);
                mVertices[i+25].set(zero);
                mVertices[i+26].set(zero);
                mVertices[i+27].set(zero);
                mVertices[i+28].set(zero);
                mVertices[i+29].set(zero);
                mVertices[i+30].set(zero);
                mVertices[i+31].set(zero);
                mVertices[i+32].set(zero);
                mVertices[i+33].set(zero);
                mVertices[i+34].set(zero);
                mVertices[i+35].set(zero);
                
                diver->reset();

                
            }
            else {
                //  top
                mVertices[i  ].set(topTL);
                mVertices[i+1].set(topTR);
                mVertices[i+2].set(topBL);
                mVertices[i+3].set(topTR);
                mVertices[i+4].set(topBL);
                mVertices[i+5].set(topBR);
                
                //  bottom
                mVertices[i+ 6].set(bottomTL);
                mVertices[i+ 7].set(bottomTR);
                mVertices[i+ 8].set(bottomBL);
                mVertices[i+ 9].set(bottomTR);
                mVertices[i+10].set(bottomBL);
                mVertices[i+11].set(bottomBR);
                
                
                //side top
                mVertices[i+12].set(topTL);
                mVertices[i+13].set(topTR);
                mVertices[i+14].set(bottomTL);
                mVertices[i+15].set(topTR);
                mVertices[i+16].set(bottomTL);
                mVertices[i+17].set(bottomTR);
                
                //side bottom
                mVertices[i+18].set(topBL);
                mVertices[i+19].set(topBR);
                mVertices[i+20].set(bottomBL);
                mVertices[i+21].set(topBR);
                mVertices[i+22].set(bottomBL);
                mVertices[i+23].set(bottomBR);
                
                //side left
                mVertices[i+24].set(topBL);
                mVertices[i+25].set(topTL);
                mVertices[i+26].set(bottomBL);
                mVertices[i+27].set(topTL);
                mVertices[i+28].set(bottomTL);
                mVertices[i+29].set(bottomBL);
                
                //side left
                mVertices[i+30].set(topBR);
                mVertices[i+31].set(topTR);
                mVertices[i+32].set(bottomBR);
                mVertices[i+33].set(topTR);
                mVertices[i+34].set(bottomTR);
                mVertices[i+35].set(bottomBR);
            }
            
            
         
            i+=36;
            j++;
        }
        
    };
    
    
    inline void draw(){
        // bounds
        /*
        glColor3f(0.5f, 0.5f, 0.5f);
        
        static const float bounds[] = {
            -0.5f,0,-0.5,
             0.5f,0,-0.5,
             0.5f,0,0.5,
            -0.5f,0, 0.5,
        };
        glDisable(GL_LIGHTING);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, bounds);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        */
         /*
        glColor3f(1, 0, 0);
        glPointSize(5.0f);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
        glDrawArrays(GL_POINTS, 0, mPoints.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        
        glColor3f(1, 1, 1);
        glPointSize(5.0f);
        */
    
        //glEnable(GL_LIGHTING);
        mMaterial.apply();
        glPushMatrix();
        glTranslatef(mPos.x, 0.0, mPos.z);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mVertices[0].x);
        glNormalPointer(GL_FLOAT, 0, &mNormals[0].x);
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPopMatrix();
        //glDisable(GL_LIGHTING);
    
        /*
        
        for(vector<Diver*>::iterator itr = mDivers.begin(); itr != mDivers.end(); itr++){
            (*itr)->draw();
        }
         */
        
        /*
        gl::enableAlphaBlending();
        int i = -1;
        while (++i < mVertices.size()) {
            this->drawIndex(mVertices[i], i);
        }
        gl::disableAlphaBlending();
         */
    };
    
    
    
    
};


#endif
