//
//  InfoPanel.h
//  grid_03
//
//  Created by Henryk Wollik on 28/02/14.
//
//

#ifndef grid_03_InfoPanel_h
#define grid_03_InfoPanel_h

#include "cinder/app/AppBasic.h"
#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"


using namespace ci;

class InfoPanel {
    Rectf  mRect;
    float  mFps;
    float* mModelScale;
    int*   mModelZoom;
    
public:
    InfoPanel(Rectf rect) :
    mRect(rect){
    
    }
    
    inline void setModelScale(float* scale){
        mModelScale = scale;
    }
    
    inline void setModelZoom(int* zoom){
        mModelZoom = zoom;
    }
    
    
    inline void setFps(float fps){
        mFps = fps;
    }
    
    
    
    
    
    inline void draw(){
        static const float padding = 10;
        
        
        float prevColor[4];
        gl::enableAlphaBlending();
        gl::disableDepthRead();
        glPushAttrib(GL_VIEWPORT_BIT);
        gl::setMatricesWindow(app::getWindowSize());
        //glViewport(0, 0, app::getWindowWidth(), app::getWindowHeight());
    
        glColor4f(0,0,0,0.65f);
        gl::drawSolidRect(mRect);
        glPushMatrix();
        glTranslatef(padding, padding, 0);
        glColor3f(1,1,1);
        gl::drawString("INFO",Vec2f(0,padding));
        glColor3f(0.5f,0.5f,0.5f);
        gl::drawLine(Vec2f(0,23.5f), Vec2f(mRect.x2 - padding * 2,23.5f));
        glColor3f(1,1,1);
        gl::drawString("FPS: "         + toString(mFps),Vec2f(0,padding + 30));
        gl::drawString("MODEL_SCALE: " + toString(*mModelScale),Vec2f(0,padding + 50));
        gl::drawString("MODEL_ZOOM: "  + toString(*mModelZoom),Vec2f(0,padding + 70));

        
        glPopMatrix();
        glPopAttrib();
        
        
        
        
        glColor4f(prevColor[0], prevColor[1], prevColor[2], prevColor[3]);
        gl::enableDepthRead();
        gl::disableAlphaBlending();
    }
    
};



#endif
