//
//  GridCube.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 28/11/13.
//
//

#ifndef IsoGridMarcher_GridCube_h
#define IsoGridMarcher_GridCube_h

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "Edge.h"
#include <cassert>

class Cube {
public:
    const static int NUM_EDGES  = 18;
    const static int NUM_POINTS = 8;
    
private:
    int mX,mY,mZ;
    int   mPoints[NUM_POINTS];
    Edge* mEdges[NUM_EDGES];
    
public:
    
    Cube(int x = 0, int y = 0, int z = 0);
    
    void setPointIndices(int index0, int index1, int index2, int index3,
                         int index4, int index5, int index6, int index7);
                         
    void setEdges(Edge* bulb00, Edge* bulb01, Edge* bulb02, Edge* bulb03,
                  Edge* bulb04, Edge* bulb05, Edge* bulb06, Edge* bulb07,
                  Edge* bulb08, Edge* bulb09, Edge* bulb10, Edge* bulb11,
                  Edge* bulb12, Edge* bulb13,
                  Edge* bulb14, Edge* bulb15, Edge* bulb16, Edge* bulb17);
    
    void switchOn();
    void switchOn(int index);
    
    void switchOff();
    void switchOff(int index);
    
    void switchRandom(float triggerTolerance = 0.0015f);
    
    int getIX(){ return mX; }
    int getIY(){ return mY; }
    int getIZ(){ return mZ; }
    
    Edge* operator[](int n){
        return mEdges[n];
    }
    
    const Edge* operator[](int n) const{
        return mEdges[n];
    }
    
    const int* getPointIndices() const{
        return mPoints;
    }
    
    int* getPointIndices(){
        return mPoints;
    }
};

#endif
