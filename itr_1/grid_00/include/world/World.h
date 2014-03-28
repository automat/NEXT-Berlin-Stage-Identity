//
//  World.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_World_h
#define grid_00_World_h

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "util/FrustumOrtho.h"

#include "util/Utils.h"
#include "layout/geom/LayoutArea.h"
#include "layout/quote/QuoteTypesetter.h"


#include "cinder/Matrix44.h"
#include "cinder/Vector.h"

#include "Config.h"

#include "world/grid/Grid.h"
#include "world/board/Board.h"

/*--------------------------------------------------------------------------------------------*/

typedef std::shared_ptr<class World> WorldRef;

/*--------------------------------------------------------------------------------------------*/

using namespace ci;
class World {
    
    
    CameraOrtho  mCamera;
    float        mCameraAspectRatio;
    float        mModelScale;
    Matrix44f    mTransform;
    FrustumOrtho mFrustum;
    
    LayoutArea       mArea;
    Grid*            mGrid;
    Board*           mBoard;
    QuoteTypesetter* mTypesetter;
    
    /*--------------------------------------------------------------------------------------------*/
    // Init Cells
    /*--------------------------------------------------------------------------------------------*/
    
    inline void initCells(){
        
    }
    
    
    /*--------------------------------------------------------------------------------------------*/
    // Draw Scene
    /*--------------------------------------------------------------------------------------------*/
    
    
    inline void drawScene(){
        
#ifdef DEBUG_WORLD_GRID_DRAW_INDICES
        mGrid->debugDrawIndices(mCamera);
#endif
#ifdef DEBUG_WORLD_AREA_DRAW
        static const int indices[] = {0,1,3,2};
        static const Vec3f vertices[] = {
            mArea.getTL(),
            mArea.getTR(),
            mArea.getBL(),
            mArea.getBR()
        };
        
        glColor3f(0.5f,0,0.125f);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
        glDisableClientState(GL_VERTEX_ARRAY);
#endif
#ifdef DEBUG_WORLD_GRID_CELL_DRAW
        {
            const vector<Cell*> cells = mGrid->getCells();
            for(vector<Cell*>::const_iterator itr = cells.begin(); itr != cells.end(); ++itr){
                (*itr)->debugDrawArea();
            }
        }
#endif
    }
    

    
public:
    
    /*--------------------------------------------------------------------------------------------*/
    // Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    World(){
        //
        //  Camera
        //
        mCameraAspectRatio = app::getWindowAspectRatio();
        mCamera.setOrtho(-mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM, mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM,
                         -WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_NEAR_CLIP, WORLD_MODEL_CAM_FAR_CLIP);
        viewOrtho();

        mModelScale = WORLD_MODEL_SCALE;
        mTransform  = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
        mFrustum.set(mCamera);
        
        
        //
        //  Environment
        //
        mGrid = new Grid(WORLD_GRID_NUM_CELLS_XY,WORLD_GRID_NUM_CELLS_XY);
        
        // get Area from initial frustum
        const vector<Vec3f>& frPlaneNear = mFrustum.getNearPlane();
        const vector<Vec3f>& frPlaneFar  = mFrustum.getFarPlane();
        Planef xzPlane(Vec3f::zero(), Vec3f::yAxis());
        Vec3f tl,tr,bl,br;
        utils::getIntersection(xzPlane, frPlaneNear[0], frPlaneFar[0], &tl);
        utils::getIntersection(xzPlane, frPlaneNear[1], frPlaneFar[1], &tr);
        utils::getIntersection(xzPlane, frPlaneNear[2], frPlaneFar[2], &br);
        utils::getIntersection(xzPlane, frPlaneNear[3], frPlaneFar[3], &bl);
        mArea = LayoutArea(tl,tr,bl,br);
        mArea*= mTransform.inverted();
        
        
        //
        //  Typesetter
        //
        mTypesetter = new QuoteTypesetter(mGrid, mArea);
        mTypesetter->setFont(Font(app::loadResource(RES_FONT_TRANSCRIPT),400.0f),0.7f);
        mTypesetter->constrain(false);
        mTypesetter->manualLineBreak(true);
        mTypesetter->setAlign(QuoteTypesetter::Align::CENTER);
        mTypesetter->debugTexture();
        
    }
    
    ~World(){
        delete mTypesetter;
        delete mGrid;
    }
    
    
    /*--------------------------------------------------------------------------------------------*/
    // Update / Draw
    /*--------------------------------------------------------------------------------------------*/
    
    inline void update(){
        
    }
    
    inline void draw(){
        gl::enableDepthRead();
        gl::setMatrices(mCamera);
        gl::drawCoordinateFrame();

#ifdef DEBUG_WORLD_CAM_FRUSTUM
        mFrustum.draw();
#endif

        glPushMatrix();
        glMultMatrixf(&mTransform[0]);
        drawScene();
        glPopMatrix();
        gl::disableDepthRead();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Model zoom / Camera ortho,top
    /*--------------------------------------------------------------------------------------------*/
    
    inline void zoomModelIn(){
        mModelScale = MIN(WORLD_MODEL_SCALE_MIN, mModelScale * 2);
        mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    }
    
    inline void zoomModelOut(){
        mModelScale = MIN(mModelScale / 2, WORLD_MODEL_SCALE_MAX);
        mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    }
    
    inline void viewTop(){
        mCamera.lookAt(Vec3f(0,1,0), Vec3f::zero());
    }
    
    inline void viewOrtho(){
        mCamera.lookAt(Vec3f(-1,1,-1), Vec3f::zero());
    }
    

    /*--------------------------------------------------------------------------------------------*/

    inline static WorldRef create(){
        return std::make_shared<World>();
    }
};

#endif
