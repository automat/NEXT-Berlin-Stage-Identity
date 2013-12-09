//
//  LightCube.cpp
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 07/12/13.
//
//

#include "Cube.h"

Cube::Cube(int x, int y, int z) :
mX(x),mY(y),mZ(z){
    mEdges[ 0] = mEdges[ 1] = mEdges[ 2] = mEdges[ 3] =    // bottom
    mEdges[ 4] = mEdges[ 5] = mEdges[ 6] = mEdges[ 7] =    // top
    mEdges[ 8] = mEdges[ 9] = mEdges[10] = mEdges[11] =    // sides
    mEdges[12] = mEdges[13] =                              // bottom top diagonal
    mEdges[14] = mEdges[15] = mEdges[16] = mEdges[17] = 0; // sided diagonal
    
    mPoints[0] = mPoints[1] = mPoints[2] = mPoints[3] =
    mPoints[4] = mPoints[5] = mPoints[6] = mPoints[7] = 0;
}

void Cube::setEdges(Edge* bulb00, Edge* bulb01, Edge* bulb02, Edge* bulb03,
                        Edge* bulb04, Edge* bulb05, Edge* bulb06, Edge* bulb07,
                        Edge* bulb08, Edge* bulb09, Edge* bulb10, Edge* bulb11,
                        Edge* bulb12, Edge* bulb13,
                        Edge* bulb14, Edge* bulb15, Edge* bulb16, Edge* bulb17){
    mEdges[ 0] = bulb00; mEdges[ 1] = bulb01; mEdges[ 2] = bulb02; mEdges[ 3] = bulb03;
    mEdges[ 4] = bulb04; mEdges[ 5] = bulb05; mEdges[ 6] = bulb06; mEdges[ 7] = bulb07;
    mEdges[ 8] = bulb08; mEdges[ 9] = bulb09; mEdges[10] = bulb10; mEdges[11] = bulb11;
    mEdges[12] = bulb12; mEdges[13] = bulb13;
    mEdges[14] = bulb14; mEdges[15] = bulb15; mEdges[16] = bulb16; mEdges[17] = bulb17;
}

void Cube::setPoints(ci::Vec3f* p0, ci::Vec3f* p1, ci::Vec3f* p2, ci::Vec3f* p3,
                     ci::Vec3f* p4, ci::Vec3f* p5, ci::Vec3f* p6, ci::Vec3f* p7){
    mPoints[0] = p0; mPoints[1] = p1; mPoints[2] = p2; mPoints[3] = p3;
    mPoints[4] = p4; mPoints[5] = p5; mPoints[6] = p6; mPoints[7] = p7;
}

void Cube::switchOn(){
    int i = -1;
    while (++i < NUM_EDGES) {
        mEdges[i]->switchOn();
    }
}

void Cube::switchOn(int index){
    mEdges[index]->switchOn();
}

void Cube::switchOff(){
    int i = -1;
    while (++i < NUM_EDGES) {
        mEdges[i]->switchOff();
    }
}

void Cube::switchRandom(float triggerTolerance){
    int i = -1;
    while (++i < NUM_EDGES) {
        mEdges[i]->switchRandom(triggerTolerance);
    }
}








