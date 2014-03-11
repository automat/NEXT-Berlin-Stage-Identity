//
//  Config.h
//  ProjectionSplitMerger
//
//  Created by Henryk Wollik on 10/03/14.
//
//

#ifndef ProjectionSplitMerger_Config_h
#define ProjectionSplitMerger_Config_h

/*
#define STAGE_SCREEN_WIDTH 3552
#define STAGE_SCREEN_HEIGHT 1105

#define WINDOW_STAGE_SCREEN_DIV 2 // for fitting the screen
#define WINDOW_OVERLAP_ZONE 288 / WINDOW_STAGE_SCREEN_DIV

#define VIEWPORT_WIDTH  STAGE_SCREEN_WIDTH / WINDOW_STAGE_SCREEN_DIV
#define VIEWPORT_HEIGHT STAGE_SCREEN_HEIGHT / WINDOW_STAGE_SCREEN_DIV

#define WINDOW_WIDTH  VIEWPORT_WIDTH + WINDOW_OVERLAP_ZONE / WINDOW_STAGE_SCREEN_DIV
#define WINDOW_HEIGHT VIEWPORT_HEIGHT
#define APP_FPS 32.0f
*/

//#define DISPLAY_WIDTH  1920

#include "cinder/Area.h"
#include "cinder/Rect.h"
#include "cinder/gl/TextureFont.h"

using namespace ci;

static int   PROJECTION_EDGE_WIDTH, PROJECTION_EDGE_WEIGHT;

static int   WINDOW_PROJECTION_SCALE;
static int   WINDOW_WIDTH, WINDOW_HEIGHT;
static bool  WINDOW_REZISABLE, WINDOW_BORDERLESS;
static int   WINDOW_DISPLAY;
static bool  WINDOW_FIXED_POSITION;
static bool  WINDOW_ALWAYS_ON_TOP;
static int   WINDOW_POSITION_X, WINDOW_POSITION_Y;

static int   DISPLAY_WIDTH;

static float APP_FPS;
static int   APP_VIEWPORT_WIDTH, APP_VIEWPORT_HEIGHT;
static bool  APP_DEBUG_DRAW_VIEWPORTS;
static int   APP_ANTIALIASING;

static Area  FBO_DISPLAY0_AREA;
static Area  FBO_DISPLAY1_AREA;

static Rectf DISPLAY0_RECT;
static Rectf DISPLAY1_RECT;

#include <iostream>
#include <string>
#include "cinder/Json.h"


namespace config {
    using namespace std;
    
    inline static void Load(const string& path){
        JsonTree json = JsonTree(loadFile(path));
        
        // Projection
        int projectionWidth     = json.getChild("Projection.Width").getValue<int>();
        int projectionHeight    = json.getChild("Projection.Height").getValue<int>();
        int projectionEdgeWidth = json.getChild("Projection.Edge.Width").getValue<int>();
        
        
        // Window
        WINDOW_PROJECTION_SCALE = json.getChild("Window.ProjectionScale").getValue<int>();
        WINDOW_REZISABLE        = json.getChild("Window.Rezisable").getValue<bool>();
        WINDOW_BORDERLESS       = json.getChild("Window.Borderless").getValue<bool>();
        WINDOW_FIXED_POSITION   = json.getChild("Window.FixedPosition").getValue<bool>();
        WINDOW_ALWAYS_ON_TOP    = json.getChild("Window.AlwaysOnTop").getValue<bool>();
        WINDOW_POSITION_X       = json.getChild("Window.Position.x").getValue<int>();
        WINDOW_POSITION_Y       = json.getChild("Window.Position.y").getValue<int>();
        
        PROJECTION_EDGE_WIDTH  = projectionEdgeWidth / WINDOW_PROJECTION_SCALE;
        PROJECTION_EDGE_WEIGHT = json.getChild("Projection.Edge.Weight").getValue<int>();
        
        WINDOW_WIDTH            = (projectionWidth + projectionEdgeWidth)  / WINDOW_PROJECTION_SCALE;
        WINDOW_HEIGHT           = projectionHeight / WINDOW_PROJECTION_SCALE;
        
        DISPLAY_WIDTH = WINDOW_WIDTH / 2;
        
        
        // App
        APP_FPS = json.getChild("App.FPS").getValue<float>();
        APP_VIEWPORT_WIDTH  = WINDOW_WIDTH - PROJECTION_EDGE_WIDTH ;
        APP_VIEWPORT_HEIGHT = WINDOW_HEIGHT;
        APP_ANTIALIASING    = json.getChild("App.Antialiasing").getValue<int>();
        
        DISPLAY0_RECT = Rectf(0,0,DISPLAY_WIDTH,WINDOW_HEIGHT);
        DISPLAY1_RECT = Rectf(DISPLAY_WIDTH,0,DISPLAY_WIDTH + DISPLAY_WIDTH , WINDOW_HEIGHT);
        
        FBO_DISPLAY0_AREA = Area(Vec2f(),
                                 Vec2f(DISPLAY_WIDTH, APP_VIEWPORT_HEIGHT));
        FBO_DISPLAY1_AREA = Area(Vec2f(DISPLAY_WIDTH - PROJECTION_EDGE_WIDTH,0),
                                 Vec2f(DISPLAY_WIDTH - PROJECTION_EDGE_WIDTH + DISPLAY_WIDTH, APP_VIEWPORT_HEIGHT));
    
    }
}



#endif
