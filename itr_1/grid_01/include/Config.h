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
#define CONFIG_FILE_PATH_BAKED     "/Users/automat/Projects/next/itr_1/grid_01/xcode/config.json"
#define CONFIG_FILE_PATH_RELATIVE

/*--------------------------------------------------------------------------------------------*/
// Stage
/*--------------------------------------------------------------------------------------------*/

#define STAGE_WIDTH  3552
#define STAGE_HEIGHT 1105
#define STAGE_SCALE  2

/*--------------------------------------------------------------------------------------------*/
// App
/*--------------------------------------------------------------------------------------------*/

#define APP_WIDTH  STAGE_WIDTH / STAGE_SCALE
#define APP_HEIGHT STAGE_HEIGHT / STAGE_SCALE
#define APP_FPS 36.0f

#define APP_USE_BAKED_CONFIG


//#define DEBUG_SINGLE_DIVER_FIELD
//#define DEBUG_SINGLE_QUOTE_FIELD


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  World
//
////////////////////////////////////////////////////////////////////////////////////////////////

#define WORLD_MODEL_SCALE_MIN 0.65f
#define WORLD_MODEL_SCALE WORLD_MODEL_SCALE_MIN
#define WORLD_MODEL_SCALE_MAX WORLD_MODEL_SCALE_MIN * 4
#define WORLD_MODEL_CAM_ZOOM  1
#define WORLD_MODEL_CAM_NEAR_CLIP 0.0001
#define WORLD_MODEL_CAM_FAR_CLIP 5.0f

#define WORLD_GRID_NUM_CELLS_XY 13
#define WORLD_TYPESETTER_FONT_SIZE 400.0f
#define WORLD_TYPESETTER_FONT_SCALE 0.7f

//#define WORLD_SKIP_FX_SHADER
#define WORLD_LIVE_EDIT_FX_SHADER
#define WORLD_FX_SHADER_BLUR_SCALE 1.0f
#define WORLD_FX_SHADER_BLUR_RADIAL_SCALE 2.0f

static Colorf WORLD_LANTERN_0_COLOR_AMBIENT;
static Colorf WORLD_LANTERN_0_COLOR_DIFFUSE;
static Colorf WORLD_LANTERN_0_COLOR_SPECULAR;


#define DEBUG_WORLD_TYPESETTER_TEXCOORDS
#define DEBUG_WORLD_TYPESETTER
//#define DEBUG_WORLD_TYPESETTER_TEXTURE
#define DEBUG_WORLD_AREA_DRAW

//#define DEBUG_WORLD_COORDINATE_FRAME
//#define DEBUG_WORLD_CAM_FRUSTUM
//#define DEBUG_WORLD_GRID_DRAW_INDICES

/*--------------------------------------------------------------------------------------------*/
// Board
/*--------------------------------------------------------------------------------------------*/

#define BOARD_LIVE_EDIT_MATERIAL_SHADER

#define BOARD_SKIP_DRAW_FIELD_DIVER
#define BOARD_SKIP_DRAW_QUOTE_DIVER

//#define DEBUG_BOARD_FIELD_DIVER_PATH_SURFACE
//#define DEBUG_BOARD_FIELD_DIVER
//#define DEBUG_BOARD_FIELD_QUOTE
//#define DEBUG_BOARD_FIELD_QUOTE_TEXCOORDS

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
#define DIVER_FIELD_DIVER_MIN_LENGTH 0.1f//0.1f
#define DIVER_FIELD_DIVER_MAX_LENGTH 0.275f//0.275f

#define DIVER_FIELD_PUT_NORMAL_COLORS

static ColorAf DIVER_FIELD_MATERIAL_AMBIENT;
static ColorAf DIVER_FIELD_MATERIAL_DIFFUSE;
static ColorAf DIVER_FIELD_MATERIAL_SPECULAR;
static float   DIVER_FIELD_MATERIAL_SHININESS;


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
#define QUOTE_FIELD_DIVER_MAX_HEIGHT 0.025f
#define QUOTE_FIELD_DIVER_MIN_OFFSET -1.0f
#define QUOTE_FIELD_DIVER_MAX_OFFSET -0.8f
#define QUOTE_FIELD_DIVER_MIN_SPEED  0.015f
#define QUOTE_FIELD_DIVER_MAX_SPEED  0.0151f
#define QUOTE_FIELD_DIVER_MIN_LENGTH 1.0f//0.1f
#define QUOTE_FIELD_DIVER_MAX_LENGTH 1.0f//0.275f

static ColorAf QUOTE_FIELD_MATERIAL_AMBIENT;
static ColorAf QUOTE_FIELD_MATERIAL_DIFFUSE;
static ColorAf QUOTE_FIELD_MATERIAL_SPECULAR;
static float   QUOTE_FIELD_MATERIAL_SHININESS;

//#define QUOTE_FIELD_PUT_NORMAL_COLORS

/*--------------------------------------------------------------------------------------------*/
// JSON
/*--------------------------------------------------------------------------------------------*/

#include <string>
#include "cinder/Json.h"
#include "util/SharedFileWatcher.h"

using namespace std;
using namespace ci;

class Config {
public:
    static bool LoadJson(const string& filepath, string* msg);
    static bool DidChange(string* msg);
    static bool IsValid();
};

