#include "stage/grid/Cell.h"


Cell::Cell(const Index& index, const Vec3f& pos) :
mIndex(index), mPos(pos),mArea(pos,1,1,true){}

void Cell::debugDrawArea_Internal(){
    static const float zero[3] = {
        0,0,0
    };
    static const float verticesArea[12] = {
        -0.5f,0,-0.5f,
        0.5f,0,-0.5f,
        0.5f,0, 0.5f,
        -0.5f,0, 0.5f
    };
    
    glPushMatrix();
    glTranslatef(mPos.x, mPos.y, mPos.z);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &verticesArea[0]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glVertexPointer(3, GL_FLOAT, 0, &zero[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void Cell::debugDrawArea(){
    glColor3f(0, 0, 0.25f);
    debugDrawArea_Internal();
}