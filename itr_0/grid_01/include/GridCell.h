//
//  GridCell.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridCell_h
#define grid_01_GridCell_h

#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>

#include "cinder/Vector.h"
#include "GridDiver.h"
#include "cinder/Rand.h"

#include "cinder/Perlin.h"

using namespace ci;
using namespace std;

struct GridCellId{
    int x;
    int y;
    GridCellId(int x, int y):x(x),y(y){};
};

class GridCell {
    Vec3f      mPos;
    Vec3f      mSize;
    int        mNumDivers;
    GridCellId mId;
    Perlin     mPerlin;
    
    
    vector<GridDiver*> mDivers;
    
    
public:
    GridCell(Vec3f pos, Vec3f size, GridCellId id) :
    mPos(pos),
    mSize(size),
    mId(id){
        
        mPerlin = Perlin(2,clock() & 65535);
        this->reset();
    }
    
    inline void reset(){
        mNumDivers = Rand::randInt(2,30);
        
        // free prev divers
        while (!mDivers.empty()) delete mDivers.back(), mDivers.pop_back();

        float marginX = -0.5f + 1.0f / mNumDivers * 0.5f;
        Vec3f start,end;
        int i = -1;
        while (++i < mNumDivers) {
            Vec3f end(marginX + float(i) / float(mNumDivers), 0.0, -0.5f);
            Vec3f start(end.x, end.y, 0.5f);
            mDivers.push_back(new GridDiver(start,end));
        }
        
        
        
    }
    
    
    
    inline void debugDraw(){
        glColor3f(1, 0, 0);
        
        static const float unitPoints[]  = {
            -0.5,0,-0.5,
             0.5,0,-0.5,
             0.5,0, 0.5,
            -0.5,0, 0.5
        };
        
        glPushMatrix();
        glTranslatef(mPos.x, mPos.y, mPos.z);
        gl::enableAlphaBlending();
        
        gl::disableAlphaBlending();
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &unitPoints[0]);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        for (vector<GridDiver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr) {
            (*itr)->debugDraw();
        }
        
        glPopMatrix();
    }
    
    inline void draw(){
        
    }
    
    inline void update(){
        for(vector<GridDiver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
            GridDiver* diver = *itr;
            
            const Vec3f& pos = diver->getPos();
            diver->setVel(mPerlin.fBm(pos.x,pos.z,app::getElapsedSeconds()) * 1.0f);
            diver->update();
        }
        
    }
};



#endif
