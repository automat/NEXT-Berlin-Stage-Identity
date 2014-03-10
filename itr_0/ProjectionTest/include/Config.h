//
//  Config.h
//  grid_07
//
//  Created by Henryk Wollik on 08/03/14.
//
//

#ifndef grid_07_Config_h
#define grid_07_Config_h

/*--------------------------------------------------------------------------------------------*/
// DEFINES
/*--------------------------------------------------------------------------------------------*/

#define PATH_CONFIG_JSON "/Users/automat/Projects/next/itr_0/ProjectionTest/resources/config.json"
#define CONFIGURABLE_SETUP

/*--------------------------------------------------------------------------------------------*/
// CONFIGURABLES
/*--------------------------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include "cinder/Json.h"

using namespace ci;
using namespace std;
using namespace gl;


static int   APP_WIDTH;
static int   APP_HEIGHT;
static float APP_FPS;
static bool  WINDOW_RESIZABLE;
static bool  WINDOW_BORDERLESS;
static int   WINDOW_DISPLAY_ID;
static Vec2f WINDOW_DISPLAY_POSITION;



namespace config {
    inline static void Load(const string& path){
        JsonTree jsonData;
        
        try{
            jsonData = JsonTree(loadFile(path));
        } catch (JsonTree::Exception& exc) {
            cout << exc.what() << endl;
            return;
        }
        
        int   stage_width;
        int   stage_height;
        float app_scale;
        float app_fps;
        int   window_display_id;
        bool  window_resizable;
        bool  window_borderless;
        Vec2f window_display_position;
        
        try {
            stage_width             = jsonData.getChild("Stage.width").getValue<int>();
            stage_height            = jsonData.getChild("Stage.height").getValue<int>();
            app_scale               = jsonData.getChild("App.invScale").getValue<int>();
            app_fps                 = jsonData.getChild("App.fps").getValue<float>();
            window_display_id       = jsonData.getChild("Window.Display").getValue<int>();
            window_resizable        = jsonData.getChild("Window.Resizable").getValue<bool>();
            window_borderless       = jsonData.getChild("Window.Borderless").getValue<bool>();
            window_display_position = Vec2f(jsonData.getChild("Window.Position.x").getValue<int>(),
                                            jsonData.getChild("Window.Position.y").getValue<int>());
            
        } catch (JsonTree::Exception& exc) {
            cout << exc.what() << endl;
            return;
        }
        
        APP_WIDTH               = stage_width / app_scale;
        APP_HEIGHT              = stage_height / app_scale;
        APP_FPS                 = app_fps;
        WINDOW_DISPLAY_ID       = window_display_id;
        WINDOW_RESIZABLE        = window_resizable;
        WINDOW_BORDERLESS       = window_borderless;
        WINDOW_DISPLAY_POSITION = window_display_position;
    }
}





#endif
