//
//  Scene.cpp
//  WindingCubes
//
//  Created by Henryk Wollik on 16/12/13.
//
//

#include "Scene.h"
#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"

Scene::Scene(){
    int numPointsX = 2;
    int numPointsY = 2;
    int numPoints  = numPointsX * numPointsY;
    int numPoints_2= numPointsX * 0.5;
    
    float scaleStartX = 1;
    float scaleStartY = 1;
    float scaleEndX   = 1;
    float scaleEndY   = 1;
    float distance;
    
    mPoints.resize(numPoints);
    mEdges.resize( mPoints.size() * 0.5);
    
    
    
    int i,j;
    
    i = -1;
    while (++i < mEdges.size()) {
        j = i * 2;
        
        // Start
        if(j < )
        
        
        
        // End
        
        
    }
}

Scene::~Scene(){}

void Scene::draw(float time, float timeDelta, int tick){
    
    // Draw Points
    glColor3f(1, 1, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
    glDrawArrays(GL_POINTS, 0, mPoints.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    
    
    
}

void Scene::update(float time, float timeDelta, int tick){
    
}