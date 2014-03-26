//
//  WindowPreviewIso.h
//  NextQuoteProcessor
//
//  Created by Henryk Wollik on 25/03/14.
//
//

#ifndef NextQuoteProcessor_WindowPreviewIso_h
#define NextQuoteProcessor_WindowPreviewIso_h


#include "CinderGwen.h"
#include "Gwen/Controls/WindowControl.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "Cell.h"
#include "Quote.h"
#include "QuoteTypesetter.h"
#include "Grid.h"


using namespace ci;


class WindowPreviewIso : public Gwen::Controls::WindowControl {
    float       mCameraZoom;
    CameraOrtho mCamera;
    
    Grid* mGrid;
    QuoteTypesetter* mTypesetter;
    
    
    
public:
    WindowPreviewIso(Gwen::Controls::Base* base) : Gwen::Controls::WindowControl(base){
        mCameraZoom = 3.25f;
        mCamera.lookAt(Vec3f(1,1,1),Vec3f::zero());
        
        
    }
    
    inline void connect(Grid* grid, QuoteTypesetter* typesetter){
        mGrid = grid;
        mTypesetter = typesetter;
    }
    
    
    inline void Render( Gwen::Skin::Base* skin ){
        //Gwen::Controls::WindowControl::Render(skin);
        
        static const float paddingT  = 0.0f;
        
        float windowHeight   = app::getWindowHeight();
        float viewportWidth  = this->GetSize().x;
        float viewportHeight = this->GetSize().y - paddingT;
        float globalPosX     = LocalPosToCanvas().x;
        float globalPosY     = LocalPosToCanvas().y + paddingT;
        
        Area  viewport;
        
        static float viewportWidthPrev  = -1;
        static float viewportHeightPrev = -1;
        
        if(viewportWidth != viewportWidthPrev || viewportHeight != viewportHeightPrev){
            float viewportRatio = viewportHeight / viewportWidth;
            mCamera.setOrtho(-mCameraZoom, mCameraZoom, -mCameraZoom * viewportRatio, mCameraZoom * viewportRatio, -10.0f, 10.f);
        
            viewportWidthPrev  = viewportWidth;
            viewportHeightPrev = viewportHeight;
        }
        
        viewport = Area(Rectf(globalPosX,                 windowHeight - globalPosY,
                              globalPosX + viewportWidth, windowHeight - viewportHeight - globalPosY ));
        
        
        glPushAttrib(GL_VIEWPORT_BIT);
      
        gl::setViewport(viewport);
        gl::setMatricesWindow(viewportWidth, viewportHeight,true);

        glColor3f(0,0,0);
        gl::drawSolidRect(Rectf(0,0,viewportWidth,viewportHeight));

        gl::setMatrices(mCamera);
        
        // gl::drawCoordinateFrame(4);
        
        glPushMatrix();
        glScalef(0.65f, 0.65f, 0.65f);
        
        gl::disableDepthRead();
        mTypesetter->debugDrawArea();
        const vector<Cell*>& cells = mGrid->getCells();
        
        glColor3f(0.5f,0,0);
        for (auto* cell : cells) {
            cell->debugDrawArea();
        }
        mGrid->debugDrawIndices(mCamera);
        
        gl::enableDepthRead();
        gl::enableAlphaTest();
        gl::enableAlphaBlending();
        gl::enableAdditiveBlending();
        glColor3f(1,1,1);
        mTypesetter->debugDrawString();
        gl::disableAlphaBlending();
        gl::disableAlphaTest();
        gl::disableDepthRead();
        
        glPopMatrix();
        
        gl::setMatricesWindow(app::getWindowSize());
        glPopAttrib();
    }
};


#endif
