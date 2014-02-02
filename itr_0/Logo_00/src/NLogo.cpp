//
//  NLogo.cpp
//  Logo_00
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#include "NLogo.h"
#include "GeomHelper.h"
#include <OpenGL/OpenGL.h>
#include "cinder/app/AppBasic.h"
#include "Resources.h"
#include <math.h>

using namespace std;

NLogo::NLogo(){
    float unit  = mUnit = 1.0f / 3.0f;
    float unit2 = unit * 2.0f;
    float unit3 = 1.0f;
    float uniti;
    
    int i = -1;
    while (++i < 3) { //1st, 2nd, 3rd row
        uniti = unit * i;
        mMesh.appendVertex(Vec3f(0,    0,uniti));
        mMesh.appendVertex(Vec3f(unit, 0,uniti));
        mMesh.appendVertex(Vec3f(unit2,0,uniti));
        mMesh.appendVertex(Vec3f(unit3,0,uniti));
        
        mMesh.appendTexCoord(Vec2f(0,    uniti));
        mMesh.appendTexCoord(Vec2f(unit, uniti));
        mMesh.appendTexCoord(Vec2f(unit2,uniti));
        mMesh.appendTexCoord(Vec2f(unit3,uniti));
   }
    //zipfel
    mMesh.appendVertex(Vec3f(unit,0,unit * 3));
    mMesh.appendTexCoord(Vec2f(unit,unit * 3));
    
    //gen basefaces
    GeomHelper::addFace(0, 1, 5, &mMesh);
    GeomHelper::addFace(0, 5, 4, &mMesh);
    GeomHelper::addFace(1, 2, 6, &mMesh);
    GeomHelper::addFace(1, 6, 5, &mMesh);
    GeomHelper::addFace(2, 3, 7, &mMesh);
    GeomHelper::addFace(2, 7, 6, &mMesh);
    
    GeomHelper::addFace(4, 5, 9, &mMesh);
    GeomHelper::addFace(4, 9, 8, &mMesh);
    GeomHelper::addFace(5, 6, 10, &mMesh);
    GeomHelper::addFace(5, 10, 9, &mMesh);
    GeomHelper::addFace(6, 7, 11, &mMesh);
    GeomHelper::addFace(6, 11, 10, &mMesh);
    
    GeomHelper::addFace(8, 9, 12, &mMesh);
    
    GeomHelper::subdivideFaces(&mMesh,1);
    
    mMesh.recalculateNormals();
    
 
}

void NLogo::draw(){
    glPushMatrix();
    glTranslatef(-0.5, 0, -0.5);
    
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mMesh.getVertices()[0].x);
    glDrawArrays(GL_POINTS, 0, mMesh.getNumVertices());
    glDisableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_LIGHTING);
    
    /*/
    glShadeModel(GL_FLAT);
    gl::draw(mMesh);
    glShadeModel(GL_SMOOTH);
    /*/
    gl::draw(mMesh);
    /**/
    
    glPopMatrix();
    
}

void NLogo::update(float t){
    vector<Vec3f>& meshVertices = mMesh.getVertices();
    float nx, ny;
    
    for(vector<Vec3f>::iterator itr = meshVertices.begin(); itr != meshVertices.end(); ++itr){
        nx = 0.5f + itr->x;
        ny = 0.5f + itr->z;
        itr->y = sinf(nx * ny * M_PI * 4.0 + t)*0.0125f;
    };
    
    mMesh.recalculateNormals();
    
}
