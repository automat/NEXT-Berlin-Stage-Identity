//
//  FragmentSurface.cpp
//  Fragment_01
//
//  Created by Henryk Wollik on 26/01/14.
//
//

#include "FragSurface.h"
#include "Resources.h"

#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include "cinder/Color.h"
#include "cinder/Font.h"


bool FragSurface::ANIMATE = true;
bool FragSurface::DEBUG_DRAW_FIELD = false;
bool FragSurface::DEBUG_DRAW_TANGENTS = true;
bool FragSurface::DRAW_TRIANGLES = false;


FragSurface::FragSurface(){
    ImageSourceRef srcRef = loadImage(ci::app::loadResource(TEST_PNG));
    mTexture = gl::Texture(srcRef);
    mTexture.setWrap(GL_REPEAT, GL_REPEAT);
    
    Font font("transcript-bold",180);
    
    TextBox textbox;
    textbox.setFont(font);
    textbox.setColor(Color(1,1,1));
    textbox.setSize(Vec2f(1024,1024));
    textbox.setText("Drones dancing.\nPeople cheering.");
    
    mQuoteTexture = gl::Texture(textbox.render());
    mQuoteTexture.setWrap(GL_REPEAT, GL_REPEAT);
    
    
    int numPoints = 4;
    
    this->initSurface(numPoints, numPoints);
    this->initFbo();
};

/*---------------------------------------------------------------------------------------------------------------*/

void FragSurface::initSurface(int numPointsX, int numPointsY){
    //Set norm grid points + texcoords;
    int numPointsX_1 = numPointsX - 1;
    int numPointsY_1 = numPointsY - 1;
    
    mPoints.resize(numPointsX * numPointsY);
    mTexcoords.resize(mPoints.size());
    
    int i,j;
    float in,jn;
    int index;
    i = -1;
    while (++i < numPointsY) {
        j = -1;
        while (++j < numPointsX) {
            index = i * numPointsX + j;
            jn = (float)j/(float)numPointsX_1;
            in = (float)i/(float)numPointsY_1;
            
            mPoints[index].set(-0.5f + jn, 0.0f, -0.5f + in);
            mTexcoords[index].set(jn,in);
        }
    }
    
    /*---------------------------------------------------------------------------------------------------------------*/
    
    //Set tangent bundle
    int numTangentsPerCellXY = 8;
    mNumFieldTangentsX   = numPointsX_1 * numTangentsPerCellXY + 1;
    mNumFieldTangentsY   = numPointsY_1 * numTangentsPerCellXY + 1;
    mNumFieldTangentsX_1 = mNumFieldTangentsX - 1;
    mNumFieldTangentsY_1 = mNumFieldTangentsY - 1;
    
    mFieldPoints.resize(mNumFieldTangentsX * mNumFieldTangentsY);
    mFieldTangents.resize(0);
    
    i = -1;
    while (++i < mNumFieldTangentsY) {
        j = -1;
        while (++j < mNumFieldTangentsX) {
            index = i * mNumFieldTangentsX + j;
            mFieldPoints[index].set(-0.5f + (float)j/(float)mNumFieldTangentsX_1, 0.0, -0.5f + (float)i/(float)mNumFieldTangentsY_1);
            mFieldTangents.push_back(Vec3f(0, 0, 0.0125f));
        }
    }

    /*---------------------------------------------------------------------------------------------------------------*/

    //Set triangles;
    int indexTL,indexTR,indexBL,indexBR;
    Vec3f pointCenter;
    Vec2f texcoordCenter;
    Vec3f triangleCenter;
    
    mFragTriangleVertices.resize(0);
    mFragTriangleCenters.resize(0);
    mFragTriangleTexcoords.resize(0);
    mFragTriangleTexcoordsMultiplied.resize(0);
    mFragTriangleTangentIndex.resize(0);
    
    i = -1;
    while (++i < numPointsY_1) {
        j = -1;
        while (++j < numPointsX_1) {
            indexTL   = i * numPointsX + j;
            indexTR   = indexTL + 1;
            indexBL   = (i+1) * numPointsX + j;
            indexBR   = indexBL + 1;
            
            Vec3f& pointTL = mPoints[indexTL];
            Vec3f& pointTR = mPoints[indexTR];
            Vec3f& pointBL = mPoints[indexBL];
            Vec3f& pointBR = mPoints[indexBR];
            pointCenter = (pointTL + pointTR + pointBL + pointBR) / 4;
            
            Vec2f& texcoordTL = mTexcoords[indexTL];
            Vec2f& texcoordTR = mTexcoords[indexTR];
            Vec2f& texcoordBL = mTexcoords[indexBL];
            Vec2f& texcoordBR = mTexcoords[indexBR];
            texcoordCenter = (texcoordTL + texcoordTR + texcoordBL + texcoordBR) / 4;
            
            this->initTriangle(pointTL,     pointTR,     pointCenter, texcoordTL,     texcoordTR,     texcoordCenter);
            this->initTriangle(pointTR,     pointBR,     pointCenter, texcoordTR,     texcoordBR,     texcoordCenter);
            this->initTriangle(pointCenter, pointBR,     pointBL,     texcoordCenter, texcoordBR,     texcoordBL);
            this->initTriangle(pointTL,     pointCenter, pointBL,     texcoordTL,     texcoordCenter, texcoordBL);
        }
    }
}

/*---------------------------------------------------------------------------------------------------------------*/

void FragSurface::initFbo(){
    int windowWidth  = ci::app::getWindowWidth();
    int windowHeight = ci::app::getWindowHeight();
    
    mFboWidth = windowHeight < windowWidth ? windowHeight : windowWidth;
    gl::Fbo::Format format;
    format.setSamples(4);
    format.setWrap(GL_REPEAT, GL_REPEAT);
    mFboRender = gl::Fbo(mFboWidth,mFboWidth,format);
}

void FragSurface::resizeFbo(){
    int windowWidth = ci::app::getWindowWidth();
    int windowHeight = ci::app::getWindowHeight();
    
    mFboWidth = windowHeight < windowWidth ? windowHeight : windowWidth;
    
}

/*---------------------------------------------------------------------------------------------------------------*/

void FragSurface::initTriangle(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2,
                               const Vec2f& t0, const Vec2f& t1, const Vec2f& t2){
    mFragTriangleVertices.push_back(p0);
    mFragTriangleVertices.push_back(p1);
    mFragTriangleVertices.push_back(p2);
    
    mFragTriangleTexcoords.push_back(t0);
    mFragTriangleTexcoords.push_back(t1);
    mFragTriangleTexcoords.push_back(t2);
    
    mFragTriangleTexcoordsMultiplied.push_back(t0);
    mFragTriangleTexcoordsMultiplied.push_back(t1);
    mFragTriangleTexcoordsMultiplied.push_back(t2);
    
    
    
    mFragTriangleCenters.push_back((p0 + p1 + p2) / 3);
    
    int indexTangentX = (0.5f + mFragTriangleCenters.back().x) * mNumFieldTangentsX_1;
    int indexTangentY = (0.5f + mFragTriangleCenters.back().z) * mNumFieldTangentsY_1;
    mFragTriangleTangentIndex.push_back(uint(indexTangentY * mNumFieldTangentsX + indexTangentX));
}

/*---------------------------------------------------------------------------------------------------------------*/

void FragSurface::drawFbo(){
    /*
    mFboRender.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl::clear(Color(0,0,0));
    gl::setMatricesWindow(ci::app::getWindowSize());
    gl::enableAlphaBlending(false);
    std::string str( "Drones dancing.\nPeople cheering." );
	Rectf boundsRect( 80, mFont->getAscent() + 80, mFboRender.getWidth() - 80, mFboRender.getHeight() - 80 );
    glColor3f(1,1,1);
    mFont->drawStringWrapped( str, boundsRect );
    gl::drawSolidRect(boundsRect);
    gl::disableAlphaBlending();
    mFboRender.unbindFramebuffer();
     */
}

/*---------------------------------------------------------------------------------------------------------------*/

void FragSurface::draw(float t){
    static const float quadVerts[12] = {-0.5,0,-0.5,
                                         0.5,0,-0.5,
                                         0.5,0, 0.5,
                                        -0.5,0, 0.5};
    
    //this->drawFbo();
    
    mTexture.enableAndBind();
    //mQuoteTexture.enableAndBind();
    
    glColor3f(1,1,1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mFragTriangleVertices[0].x);
    glTexCoordPointer(2, GL_FLOAT, 0, &mFragTriangleTexcoordsMultiplied[0].x);
    glDrawArrays(GL_TRIANGLES, 0, mFragTriangleVertices.size());
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    //gl::drawCube(Vec3f::zero(), Vec3f(0.5f,0.5f,0.5f));
     //mQuoteTexture.disable();
    mTexture.disable();
    
    
    
    if(DEBUG_DRAW_FIELD){
        // Draw unit quad
        
        glColor3f(1.0f, 0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &quadVerts[0]);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw grid points
        glColor3f(0,0,0.25f);
        glPointSize(3);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
        glDrawArrays(GL_POINTS, 0, mPoints.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw triangles
        glPushMatrix();
        glTranslatef(0, 0.0025f, 0);

        //draw triangle center
        glColor3f(1, 1, 1);
        glPointSize(4);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mFragTriangleCenters[0].x);
        glDrawArrays(GL_POINTS, 0, mFragTriangleCenters.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        
        
        glColor3f(1,1,1);
        glPointSize(4);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mFragTriangleVertices[0].x);
        glDrawArrays(GL_POINTS, 0, mFragTriangleVertices.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        
        
        // Draw tangent bundle
        glColor3f(1.0f, 0, 1.0f);
        glPointSize(2);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mFieldPoints[0].x);
        glDrawArrays(GL_POINTS, 0, mFieldPoints.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        
        glPopMatrix();
    }
    
    if (DRAW_TRIANGLES) {
        
        // Draw triangles
        glPushMatrix();
        glTranslatef(0, 0.0025f, 0);
        glColor3f(0,0,0);
        
        static Vec3f triangleVertices[4];
        static float triangleColors[16] = {0,0,0,0.25f, 0,0,0,0.25f, 0,0,0,0.25f, 0,0,0,0.25f};
        gl::enableAlphaBlending();
        gl::enableAdditiveBlending();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        int i = 0;
        int l = mFragTriangleVertices.size() - 1;
        int j = 0;
        float n;
        while (i < mFragTriangleVertices.size()) {
            triangleVertices[0] = mFragTriangleVertices[i  ];
            triangleVertices[1] = mFragTriangleVertices[i+1];
            triangleVertices[2] = mFragTriangleVertices[i+2];
            triangleVertices[0] = mFragTriangleVertices[i  ];
            
            n = (float)j/l * (float)M_PI_2 * 64;
            
            triangleColors[0 ] = triangleColors[1 ] = triangleColors[2 ] /*= triangleColors[3 ]*/ = 0.5f + sinf(n + t * 4) * 0.5f;
            triangleColors[4 ] = triangleColors[5 ] = triangleColors[6 ] /*= triangleColors[7 ]*/ = 0.5f + sinf(n + (float)M_PI_2 * 0.75f + t * 4) * 0.5f;
            triangleColors[8 ] = triangleColors[9 ] = triangleColors[10] /*= triangleColors[11]*/ = 0.5f + sinf(n + (float)M_PI_2 * 0.5f + t * 4) * 0.5f;
            triangleColors[12] = triangleColors[13] = triangleColors[14] /*= triangleColors[11]*/ = 0.5f + sinf(n + (float)M_PI_2 * 0.25f + t * 4) * 0.5f;
            
            glVertexPointer(3, GL_FLOAT, 0, &triangleVertices[0].x);
            glColorPointer(4, GL_FLOAT, 0, &triangleColors[0]);
            glDrawArrays(GL_LINE_LOOP, 0, 3);
            j++;
            i+=3;
        }
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    
        gl::disableAlphaBlending();
        glPopMatrix();

    }
    
    if(DEBUG_DRAW_TANGENTS){
        glPushMatrix();
        glTranslatef(0, 0.0125f, 0);
        
        int index;
        int i = -1;
        while (++i < mFragTriangleTangentIndex.size()) {
            index = mFragTriangleTangentIndex[i];
            glColor3f((float)i/(float)(mFragTriangleTangentIndex.size() - 1),0,1.0f-(float)i/(float)(mFragTriangleTangentIndex.size() - 1));
            //ci::gl::drawVector(mFieldPoints[index], mFieldPoints[index] + mFieldTangents[index],0.0125f,0.005f);
            ci::gl::drawLine(mFieldPoints[index], mFieldPoints[index] + mFieldTangents[index] * 0.125f);
        }
        glPopMatrix();
    }
    
    
}

/*---------------------------------------------------------------------------------------------------------------*/


void FragSurface::update(float t){
    if(ANIMATE){
        int i,j,index;
        float x,y;
        static const float maxLength = 0.125f;
        static const float scale = 3;
        
        i = -1;
        while (++i < mNumFieldTangentsY) {
            j = -1;
            while (++j < mNumFieldTangentsX) {
                index = i * mNumFieldTangentsX + j;
                Vec3f& fieldPoint = mFieldPoints[index];
                x = (fieldPoint.x) * scale;
                y = (-0.5f + fieldPoint.z) * scale;
                
                Vec3f& fieldTangent = mFieldTangents[index];
                
                fieldTangent.x = 1.0f / (1.0f + pow(y, 2));// * cosf(t);
                fieldTangent.z = x * y / pow(1.0f + pow(y,2),2);// * sinf(t) ;
                //fieldTangent.x = 0.0f;
                //fieldTangent.z = sin(x * y * (float)M_2_PI * 64 + t) * 0.005f;
                fieldTangent.limit(maxLength);
            }
        }
        
        i = -1;
        while (++i < mFragTriangleTangentIndex.size()) {
            index = i * 3;
            mFragTriangleTexcoordsMultiplied[index  ] = mFragTriangleTexcoords[index  ] - mFieldTangents[mFragTriangleTangentIndex[i]].xz() * t;
            mFragTriangleTexcoordsMultiplied[index+1] = mFragTriangleTexcoords[index+1] - mFieldTangents[mFragTriangleTangentIndex[i]].xz() * t;
            mFragTriangleTexcoordsMultiplied[index+2] = mFragTriangleTexcoords[index+2] - mFieldTangents[mFragTriangleTangentIndex[i]].xz() * t;
             
            /*
            mFragTriangleTexcoordsMultiplied[index  ] -=  mFieldTangents[mFragTriangleTangentIndex[i]].xz();
            mFragTriangleTexcoordsMultiplied[index+1] -=  mFieldTangents[mFragTriangleTangentIndex[i]].xz();
            mFragTriangleTexcoordsMultiplied[index+2] -=  mFieldTangents[mFragTriangleTangentIndex[i]].xz();
            */
        }
        
        
    }
}

/*---------------------------------------------------------------------------------------------------------------*/





