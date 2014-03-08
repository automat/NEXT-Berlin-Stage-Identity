//
//  GridController.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridController_h
#define grid_01_GridController_h

#include "Config.h"
#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>
#include "cinder/Frustum.h"
#include "FrustumOrtho.h"
#include <vector>
#include "cinder/Rect.h"
#include "cinder/Matrix44.h"
#include "cinder/Perlin.h"
#include "Oscillator.h"

#include "Cell.h"
#include "cinder/Thread.h"

using namespace std;
using namespace cinder;

class Controller {
    int mSizeX;
    int mSizeY;
    size_t mNumCells;
    
    Matrix44f mTransform;
    Rectf     mBounds;
    
    
    vector<Cell*> mCells;
    Oscillator    mOscillator;
    
    thread        mUpdatePathsThread;
    thread        mUpdateDiversThread;
    float         mUpdateInterval;
    bool          mUpdatePathsThreadIsDead;
    bool          mUpdateDiversThreadIsDead;
    mutex         mPathWriteMutex;
    mutex         mDiverWriteMutex;
    
    
public:
    Controller(int sizeX = 3, int sizeY = 3) :
    mSizeX(sizeX),
    mSizeY(sizeY){
        mNumCells = sizeX * sizeY;
        int sizeX_2 = mSizeX / 2;
        int sizeY_2 = mSizeY / 2;
        //  init
        Vec3f pos;
        int i,j;
        i = -1;
        
        while (++i < mSizeY) {
            j = -1;
            while (++j < mSizeX) {
                int id[] = {i,j};
                pos.x = -sizeX_2 + j;
                pos.z = -sizeY_2 + i;
                mCells.push_back(new Cell(id,pos,&mOscillator));
                
                // mCellMeshNormalMiddleBuffer.push_back(vector<Vec3f>(mCells.back()->copyNormalTopBuffer()));
            }
        }
        
        mBounds.x1 = -sizeX_2 - 0.5f;
        mBounds.y1 = -sizeX_2 - 0.5f;
        mBounds.x2 = mBounds.x1 + sizeX;
        mBounds.y2 = mBounds.y1 + sizeY;
        
        mUpdateInterval      = (1.0f / APP_CTRL_PATH_THREAD_FPS) * 1000.0f;
        startThreads();
    }
    
    ~Controller(){}
    
    inline void draw(){
        glPushMatrix();
        glMultMatrixf(&mTransform[0]);
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->draw();
        }
        glPopMatrix();
    }
    
    inline void reset(){
        joinThreads();
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->reset();
        }
        mOscillator.reset();
        startThreads();
    }
    
private:
    inline void startThreads(){
        mUpdatePathsThreadIsDead = false;
        mUpdateDiversThreadIsDead = false;
        mUpdatePathsThread   = thread(bind(&Controller::updatePaths,this));
        mUpdateDiversThread  = thread(bind(&Controller::updateDivers,this));
    }
    
    inline void joinThreads(){
        mUpdatePathsThreadIsDead = true;
        mUpdateDiversThreadIsDead = true;
        mUpdatePathsThread.join();
        mUpdateDiversThread.join();
        sleep(1000.0f);
    }
    
    inline void resetCells(){
        
    }
    
public:
    
    
    inline void update(float t){
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->update();
        }
    }
    
    
    inline void updatePaths(){
        while (!mUpdatePathsThreadIsDead) {
            
            mPathWriteMutex.lock(); //naak
            //more stuff here, soon
            for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
                (*itr)->updatePaths();
            }
            mPathWriteMutex.unlock();
            
            sleep(mUpdateInterval);
        }
    }
    
    inline void updateDivers(){
        while (!mUpdateDiversThreadIsDead) {
            
            mDiverWriteMutex.lock(); //naak
            //more stuff here, soon
            for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
                (*itr)->updateDivers();
            }
            mDiverWriteMutex.unlock();
            
            sleep(mUpdateInterval);
        }
    }
    
    //! draw boundaries and diver path
    inline void debugArea(){
        glPushMatrix();
        glMultMatrixf(&mTransform[0]);
        glLineWidth(3);
        
        float vertices[] = {
            mBounds.x1,0,mBounds.y1,
            mBounds.x2,0,mBounds.y1,
            mBounds.x2,0,mBounds.y2,
            mBounds.x1,0,mBounds.y2
        };
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glColor3f(1, 0, 1);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        
        glLineWidth(1);
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->debugArea();
        }
        glPopMatrix();
    }
    
    
    inline void debugDraw(){
        glPushMatrix();
        glMultMatrixf(&mTransform[0]);
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->debugDraw();
        }
        glPopMatrix();
    }
    
    
    
    //! check if cells are within the orthographic frustum
    inline void checkFrustum(const FrustumOrtho& frustum){
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->checkFrustum(frustum,mTransform);
        }
    }
    
    //! scale the the world
    inline void transform(float scale){
        mTransform = mTransform.identity();
        mTransform.scale(scale);
    }
};



#endif
