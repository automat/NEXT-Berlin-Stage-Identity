//
//  Room.cpp
//  Room_00
//
//  Created by Henryk Wollik on 01/02/14.
//
//

#include "Room.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include <OpenGL/OpenGL.h>

#include "GeomHelper.h"

Room::Room(){};

Room::Room(float dim, CameraPersp* camera) :
    mDim(dim,dim,dim),
    mTime(app::getElapsedSeconds()),
    mTimeStart(app::getElapsedSeconds()),
    mFrame(app::getElapsedFrames()),
    mCameraRef(camera){
        
        GeomHelper::genIcosahedron(&mMeshDome,4);
        GeomHelper::genIcosahedron(&mMeshFloor,3);
        
        GeomHelper::invertNormals(mMeshDome);
        
        //
        mLightDebug = new gl::Light(gl::Light::DIRECTIONAL,0);
        mLightDebug->lookAt(Vec3f(3,3,3), Vec3f(0,0,0));
        mLightDebug->setAmbient(Color(0.1,0.1,0.1));
        mLightDebug->setDiffuse(Color(1,1,1));
        mLightDebug->setSpecular(Color(1,1,1));
        //mLightDebug->update(*mCameraRef);
        mLightDebug->enable();
        
        mMaterialDebug = new gl::Material();
        mMaterialDebug->setAmbient(Color( 1, 0.25f, 1.0f ));
        mMaterialDebug->setDiffuse(Color( 1, 0.25f, 1.0f ));
        mMaterialDebug->setSpecular(Color(1,1,1));
        mMaterialDebug->setShininess(10.0f);
        
        try{
            mNormalDebugShader = gl::GlslProg(app::loadResource("normal_vert.glsl"),app::loadResource("normal_frag.glsl"));
        } catch (gl::GlslProgCompileExc &exc){
            std::cout << exc.what() << std::endl;
        } catch (...){
            throw "Can't load shader.";
        }
        
        
        //
        
        
}

void Room::update(){
    mTime  = app::getElapsedSeconds();
    mFrame = app::getElapsedFrames();
}

void Room::draw(){
    static const Vec3f ROOM_CENTER(0,0,0);
   
    glColor3f(1,1,1);
    gl::drawStrokedCube(ROOM_CENTER, mDim);
    glPushMatrix();
    
    glPushMatrix();
    glScalef(5.0f, 5.0f, 5.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mMeshDome.getVertices()[0].x);
    glDrawArrays(GL_POINTS, 0, mMeshDome.getNumVertices());
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glShadeModel(GL_FLAT);
    //glCullFace(GL_FRONT_AND_BACK);
    
    //glColor3f(0.25f, 0.25f, 0.25f);
    mMaterialDebug->apply();
    glPushMatrix();
    glScalef(5, 5, 5);
    gl::draw(mMeshDome);
    glPopMatrix();
    
    /*glPushMatrix();
    mNormalDebugShader.bind();
   
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mMeshFloor.getVertices()[0].x);
    glNormalPointer(GL_FLOAT, 0, &mMeshFloor.getNormals()[0].x);
    glDrawElements(GL_TRIANGLES, mMeshFloor.getNumIndices(), GL_UNSIGNED_INT, &mMeshFloor.getIndices()[0]);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    //gl::draw(mMeshFloor);
    mNormalDebugShader.unbind();
    glPopMatrix();
    */
    gl::draw(mMeshFloor);
    glPopMatrix();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

}

const Vec3f& Room::getDim(){
    return mDim;
}
