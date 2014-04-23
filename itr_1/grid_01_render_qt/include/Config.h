//
//  Config.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#pragma once

#include "cinder/Color.h"
#include "cinder/Vector.h"
#include "cinder/CinderResources.h"
using namespace ci;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Oberall
//
////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------*/
// Config
/*--------------------------------------------------------------------------------------------*/

#define CONFIG_USE_BAKED
#define CONFIG_FILE_PATH_BAKED     "/Users/automat/Projects/next/itr_1/grid_01_render_qt/xcode/build/Release/config.json"
#define CONFIG_FILE_PATH_RELATIVE

/*--------------------------------------------------------------------------------------------*/
// Stage
/*--------------------------------------------------------------------------------------------*/

#define STAGE_WIDTH  3552
#define STAGE_HEIGHT 1105
#define STAGE_SCALE  1

/*--------------------------------------------------------------------------------------------*/
// App
/*--------------------------------------------------------------------------------------------*/

#define APP_WIDTH  STAGE_WIDTH / STAGE_SCALE
#define APP_HEIGHT STAGE_HEIGHT / STAGE_SCALE
#define APP_FPS 30.0f

#define APP_USE_BAKED_CONFIG

#define APP_OUTPUT_VIEW_WIDTH 1920
#define APP_OUTPUT_VIEW_HEIGHT 1080


/*--------------------------------------------------------------------------------------------*/
// Window
/*--------------------------------------------------------------------------------------------*/

extern int   WINDOW_WIDTH;
extern int   WINDOW_HEIGHT;
extern bool  WINDOW_BORDERLESS;
extern int   WINDOW_DISPLAY;
extern bool  WINDOW_ALWAYS_ON_TOP;
extern bool  WINDOW_FIXED_POSITION;
extern Vec2i WINDOW_POSITION;
extern float WINDOW_FPS;


/*--------------------------------------------------------------------------------------------*/
// Projection
/*--------------------------------------------------------------------------------------------*/

extern float PROJECTION_SCALE;
extern bool  PROJECTION_OVERLAP;
extern int   PROJECTION_OVERLAP_EDGE;


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Stage
//
////////////////////////////////////////////////////////////////////////////////////////////////

#define STAGE_SKIP_SCHEDULE_VIEW
//#define STAGE_SKIP_THEME_VIEW

#define STAGE_MODEL_SCALE_MIN 0.65f
#define STAGE_MODEL_SCALE STAGE_MODEL_SCALE_MIN
#define STAGE_MODEL_SCALE_MAX STAGE_MODEL_SCALE_MIN * 4
#define STAGE_MODEL_CAM_ZOOM  1
#define STAGE_MODEL_CAM_NEAR_CLIP 0.0001
#define STAGE_MODEL_CAM_FAR_CLIP 5.0f

#define STAGE_GRID_NUM_CELLS_XY 13
#define STAGE_TYPESETTER_FONT_SIZE 400.0f
#define STAGE_TYPESETTER_FONT_SCALE 0.7f

//#define STAGE_SKIP_FX_SHADER
#define STAGE_LIVE_EDIT_FX_SHADER

extern float STAGE_FX_SHADER_BLUR_SCALE;
extern float STAGE_FX_SHADER_BLUR_RADIAL_SCALE;
extern float STAGE_FX_SHADER_BLUR_RADIAL_RADIUS_SCALE;

extern Vec3f  STAGE_LANTERN_0_DIRECTION;
extern Colorf STAGE_LANTERN_0_COLOR_AMBIENT;
extern Colorf STAGE_LANTERN_0_COLOR_DIFFUSE;
extern Colorf STAGE_LANTERN_0_COLOR_SPECULAR;
extern float  STAGE_LANTERN_0_ATTENUATION;
extern float  STAGE_LANTERN_0_CONSTANT_ATTENUATION;
extern float  STAGE_LANTERN_0_LINEAR_ATTENUATION;
extern float  STAGE_LANTERN_0_QUADRIC_ATTENUATION;

extern bool STAGE_LANTERN_0_DEBUG_DRAW;

extern Vec3f  STAGE_LANTERN_1_DIRECTION;
extern Colorf STAGE_LANTERN_1_COLOR_AMBIENT;
extern Colorf STAGE_LANTERN_1_COLOR_DIFFUSE;
extern Colorf STAGE_LANTERN_1_COLOR_SPECULAR;
extern float  STAGE_LANTERN_1_ATTENUATION;
extern float  STAGE_LANTERN_1_CONSTANT_ATTENUATION;
extern float  STAGE_LANTERN_1_LINEAR_ATTENUATION;
extern float  STAGE_LANTERN_1_QUADRIC_ATTENUATION;

extern bool STAGE_LANTERN_1_DEBUG_DRAW;

//#define DEBUG_STAGE_TYPESETTER_TEXCOORDS
//#define DEBUG_STAGE_TYPESETTER
//#define DEBUG_STAGE_TYPESETTER_TEXTURE
//#define DEBUG_STAGE_AREA_DRAW

//#define DEBUG_STAGE_COORDINATE_FRAME
//#define DEBUG_STAGE_CAM_FRUSTUM
//#define DEBUG_STAGE_GRID_DRAW_INDICES

/*--------------------------------------------------------------------------------------------*/
// Theme
/*--------------------------------------------------------------------------------------------*/

#define THEME_LIVE_EDIT_MATERIAL_SHADER

//#define THEME_SKIP_DRAW_FIELD_DIVER
//#define THEME_SKIP_DRAW_QUOTE_DIVER

//#define DEBUG_THEME_FIELD_DIVER_PATH_SURFACE
//#define DEBUG_THEME_FIELD_DIVER
//#define DEBUG_THEME_FIELD_QUOTE
//#define DEBUG_THEME_FIELD_QUOTE_TEXCOORDS

/*--------------------------------------------------------------------------------------------*/
// Background
/*--------------------------------------------------------------------------------------------*/

#define BACKGROUND_LIVE_EDIT_SHADER

/*--------------------------------------------------------------------------------------------*/
// Oscillator
/*--------------------------------------------------------------------------------------------*/

#define OSCILLATOR_PERLIN_NUM_OCTAVES 1
#define OSCILLATOR_PERLIN_SEED clock() & 65535


/*--------------------------------------------------------------------------------------------*/
// Pathsurface
/*--------------------------------------------------------------------------------------------*/

#define PATH_SLICE_NUM_POINTS 40
extern ColorAf PATH_SURFACE_COLOR;

/*--------------------------------------------------------------------------------------------*/
// Abstract Field
/*--------------------------------------------------------------------------------------------*/
//#define ABSTRACT_FIELD_PUT_NORMAL_COLORS

/*--------------------------------------------------------------------------------------------*/
// DiverField
/*--------------------------------------------------------------------------------------------*/

#define DIVER_FIELD_NUM_DIVERS_MIN 10
#define DIVER_FIELD_NUM_DIVERS_MAX 40
#define DIVER_FIELD_SURFACE_PATH_DENSITY 1.25f
#define DIVER_FIELD_SURFACE_PATH_OFFSET_SPEED  0.001f //0.0125f
#define DIVER_FIELD_SURFACE_PATH_AMPLITUDE 0.65f//0.325f

#define DIVER_FIELD_DIVER_NUM_POINTS 10
#define DIVER_FIELD_DIVER_MIN_HEIGHT 0.015f
#define DIVER_FIELD_DIVER_MAX_HEIGHT 0.045f
#define DIVER_FIELD_DIVER_MIN_OFFSET -1.0f//1.0f
#define DIVER_FIELD_DIVER_MAX_OFFSET 1.0f//-1.0f
#define DIVER_FIELD_DIVER_MIN_SPEED  0.0015f
#define DIVER_FIELD_DIVER_MAX_SPEED  0.0045f
#define DIVER_FIELD_DIVER_MIN_LENGTH 0.2f//0.1f
#define DIVER_FIELD_DIVER_MAX_LENGTH 0.5f//0.275f

#define DIVER_FIELD_PUT_NORMAL_COLORS

extern ColorAf DIVER_FIELD_MATERIAL_AMBIENT;
extern ColorAf DIVER_FIELD_MATERIAL_DIFFUSE;
extern ColorAf DIVER_FIELD_MATERIAL_SPECULAR;
extern float   DIVER_FIELD_MATERIAL_SHININESS;

/*--------------------------------------------------------------------------------------------*/
// QuoteField
/*--------------------------------------------------------------------------------------------*/

#define QUOTE_FIELD_NUM_DIVERS_MIN 10//10
#define QUOTE_FIELD_NUM_DIVERS_MAX 50//50
#define QUOTE_FIELD_SURFACE_PATH_DENSITY 0.75f
#define QUOTE_FIELD_SURFACE_PATH_OFFSET_SPEED   0.0075f
#define QUOTE_FIELD_SURFACE_PATH_AMPLITUDE 0.55f//0.25f//0.125f//0.325f


#define QUOTE_FIELD_DIVER_NUM_POINTS 20
#define QUOTE_FIELD_DIVER_MIN_HEIGHT 0.015f
#define QUOTE_FIELD_DIVER_MAX_HEIGHT 0.05f
#define QUOTE_FIELD_DIVER_MIN_OFFSET -1.0f
#define QUOTE_FIELD_DIVER_MAX_OFFSET -0.8f
#define QUOTE_FIELD_DIVER_MIN_SPEED  0.00325f
#define QUOTE_FIELD_DIVER_MAX_SPEED  0.00325f
#define QUOTE_FIELD_DIVER_MIN_LENGTH 1.0f//0.1f
#define QUOTE_FIELD_DIVER_MAX_LENGTH 1.0f//0.275f

extern ColorAf QUOTE_FIELD_MATERIAL_AMBIENT;
extern ColorAf QUOTE_FIELD_MATERIAL_DIFFUSE;
extern ColorAf QUOTE_FIELD_MATERIAL_SPECULAR;
extern float   QUOTE_FIELD_MATERIAL_SHININESS;

//#define QUOTE_FIELD_PUT_NORMAL_COLORS

/*--------------------------------------------------------------------------------------------*/
// JSON
/*--------------------------------------------------------------------------------------------*/

#include <string>
#include "cinder/Json.h"

using namespace std;
using namespace ci;

class Config {
public:
    static bool LoadJson(const string& filepath, string* msg);
    static bool Reload(string* msg);
    static bool DidChange();
    static bool IsValid();
};

