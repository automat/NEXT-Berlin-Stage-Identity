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

#include "cinder/Thread.h"
#include "cinder/Rect.h"
#include "cinder/Matrix44.h"
#include "cinder/Perlin.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Material.h"

#include <OpenGL/OpenGL.h>
#include <vector>

#include "FrustumOrtho.h"
#include "Oscillator.h"

#include "Cell.h"
#include "StringCell.h"

#include "QuoteField.h"

using namespace std;
using namespace cinder;

class Controller {
    int    mSizeX;
    int    mSizeY;
    size_t mNumCells;
    
    Matrix44f mTransform;
    Rectf     mBounds;
    
    
    vector<Cell*>       mCells;
    vector<StringCell*> mStringCells;
    Oscillator          mOscillator;
    
    Material    mBaseMaterial0;
    Material    mBaseMaterial1;
    
    thread        mUpdatePathsThread;
    thread        mUpdateDiversThread;
    float         mUpdateInterval;
    bool          mUpdatePathsThreadIsDead;
    bool          mUpdateDiversThreadIsDead;
    mutex         mPathWriteMutex;
    mutex         mDiverWriteMutex;
    
    //temp
    QuoteField  mQuoteField;
    
    
public:
    Controller(int sizeX = 3, int sizeY = 3) :
    mSizeX(sizeX),
    mSizeY(sizeY){
        mNumCells = sizeX * sizeY;
        int sizeX_2 = mSizeX / 2;
        int sizeY_2 = mSizeY / 2;
        
        
        mBaseMaterial0 = CELL_MATERIAL0;
        mBaseMaterial1 = STRING_CELL_MATERIAL0;
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
            }
        }
        int id[] = {0,0};
        //test draw string
        mStringCells.push_back(new StringCell(id,Vec3f(1,0,-3),&mOscillator,Vec3f(4,1,1)));
        mStringCells.push_back(new StringCell(id,Vec3f(0,0,-2),&mOscillator,Vec3f(5,1,1)));
        mStringCells.push_back(new StringCell(id,Vec3f(-1,0,-1),&mOscillator,Vec3f(5,1,1)));
        
        mBounds.x1 = -sizeX_2 - 0.5f;
        mBounds.y1 = -sizeX_2 - 0.5f;
        mBounds.x2 = mBounds.x1 + sizeX;
        mBounds.y2 = mBounds.y1 + sizeY;
        
        mUpdateInterval = (1.0f / APP_CTRL_PATH_THREAD_FPS) * 1000.0f;
        startThreads();
        
        mQuoteField.setSize(4, 4);
        mQuoteField.gen("Hello Bello" , 0);
    }
    
  
    
    ~Controller(){
        while (!mCells.empty()) delete mCells.back(), mCells.pop_back();
    }
    
    inline void draw(){
        glPushMatrix();
        glMultMatrixf(&mTransform[0]);
        
        mBaseMaterial0.apply();
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->draw();
        }
        mBaseMaterial1.apply();
        for(vector<StringCell*>::const_iterator itr = mStringCells.begin(); itr != mStringCells.end(); ++itr){
            (*itr)->draw();
        }
        
        
        glPopMatrix();
    }
    
    inline void reset(){
        joinThreads();
        
        mBaseMaterial0 = CELL_MATERIAL0;
        mBaseMaterial1 = STRING_CELL_MATERIAL0;
        
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->reset();
        }
        
        for(vector<StringCell*>::const_iterator itr = mStringCells.begin(); itr != mStringCells.end(); ++itr){
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
    
    inline void freeCells(){
        while (!mCells.empty()) delete mCells.back(), mCells.pop_back();
        while (!mStringCells.empty()) delete mStringCells.back(), mStringCells.pop_back();
    }
    
public:
    
    
    inline void update(){
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->update();
        }
        
        for(vector<StringCell*>::const_iterator itr = mStringCells.begin(); itr != mStringCells.end(); ++itr){
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
            
            for(vector<StringCell*>::const_iterator itr = mStringCells.begin(); itr != mStringCells.end(); ++itr){
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
            
            for(vector<StringCell*>::const_iterator itr = mStringCells.begin(); itr != mStringCells.end(); ++itr){
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
        
        for(vector<StringCell*>::const_iterator itr = mStringCells.begin(); itr != mStringCells.end(); ++itr){
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
        
        for(vector<StringCell*>::const_iterator itr = mStringCells.begin(); itr != mStringCells.end(); ++itr){
            (*itr)->debugDraw();
        }
         
        glPopMatrix();
    }
    
    
    
    //! check if cells are within the orthographic frustum
    inline void checkFrustum(const FrustumOrtho& frustum){
        for(vector<Cell*>::const_iterator itr = mCells.begin(); itr != mCells.end(); ++itr){
            (*itr)->checkFrustum(frustum,mTransform);
        }
        
        for(vector<StringCell*>::const_iterator itr = mStringCells.begin(); itr != mStringCells.end(); ++itr){
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
