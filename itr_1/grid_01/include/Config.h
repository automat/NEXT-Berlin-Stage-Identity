//
//  Config.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#pragma once

/*--------------------------------------------------------------------------------------------*/
// Overall
/*--------------------------------------------------------------------------------------------*/

#define STAGE_WIDTH  3552
#define STAGE_HEIGHT 1105
#define STAGE_SCALE  2

#define APP_WIDTH  STAGE_WIDTH / STAGE_SCALE
#define APP_HEIGHT STAGE_HEIGHT / STAGE_SCALE
#define APP_FPS 36.0f


//#define DEBUG_SINGLE_DIVER_FIELD
//#define DEBUG_SINGLE_QUOTE_FIELD




/*--------------------------------------------------------------------------------------------*/
// World
/*--------------------------------------------------------------------------------------------*/

#define WORLD_MODEL_SCALE_MIN 0.65f
#define WORLD_MODEL_SCALE WORLD_MODEL_SCALE_MIN
#define WORLD_MODEL_SCALE_MAX WORLD_MODEL_SCALE_MIN * 4
#define WORLD_MODEL_CAM_ZOOM  1
#define WORLD_MODEL_CAM_NEAR_CLIP 0.0001
#define WORLD_MODEL_CAM_FAR_CLIP 5.0f

#define WORLD_GRID_NUM_CELLS_XY 13

#define WORLD_TYPESETTER_FONT_SIZE 400.0f
#define WORLD_TYPESETTER_FONT_SCALE 0.7f

#define WORLD_LIVE_EDIT_FX_SHADER

//#define DEBUG_WORLD_TYPESETTER_TEXCOORDS
//#define DEBUG_WORLD_TYPESETTER
//#define DEBUG_WORLD_TYPESETTER_TEXTURE
//#define DEBUG_WORLD_AREA_DRAW

//#define DEBUG_WORLD_COORDINATE_FRAME
//#define DEBUG_WORLD_CAM_FRUSTUM
//#define DEBUG_WORLD_GRID_DRAW_INDICES


/*--------------------------------------------------------------------------------------------*/
// Board
/*--------------------------------------------------------------------------------------------*/

#define BOARD_LIVE_EDIT_MATERIAL_SHADER


//#define BOARD_SKIP_DRAW_FIELD_DIVER
//#define BOARD_SKIP_DRAW_QUOTE_DIVER

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
#define DIVER_FIELD_NUM_DIVERS_MAX 50
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

/*--------------------------------------------------------------------------------------------*/
// QuoteField
/*--------------------------------------------------------------------------------------------*/

#define QUOTE_FIELD_NUM_DIVERS_MIN 30//10
#define QUOTE_FIELD_NUM_DIVERS_MAX 70//50
#define QUOTE_FIELD_SURFACE_PATH_DENSITY 0.75f
#define QUOTE_FIELD_SURFACE_PATH_OFFSET_SPEED   0.0075f
#define QUOTE_FIELD_SURFACE_PATH_AMPLITUDE 0.55f//0.25f//0.125f//0.325f


#define QUOTE_FIELD_DIVER_NUM_POINTS 20
#define QUOTE_FIELD_DIVER_MIN_HEIGHT 0.015f
#define QUOTE_FIELD_DIVER_MAX_HEIGHT 0.065f
#define QUOTE_FIELD_DIVER_MIN_OFFSET -1.0f
#define QUOTE_FIELD_DIVER_MAX_OFFSET -0.8f
#define QUOTE_FIELD_DIVER_MIN_SPEED  0.0115f
#define QUOTE_FIELD_DIVER_MAX_SPEED  0.0125f
#define QUOTE_FIELD_DIVER_MIN_LENGTH 1.0f//0.1f
#define QUOTE_FIELD_DIVER_MAX_LENGTH 1.0f//0.275f

//#define QUOTE_FIELD_PUT_NORMAL_COLORS

/*--------------------------------------------------------------------------------------------*/
// JSON
/*--------------------------------------------------------------------------------------------*/


