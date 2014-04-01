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

#define DEBUG_SINGLE_DIVER_FIELD

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

//#define DEBUG_WORLD_COORDINATE_FRAME
//#define DEBUG_WORLD_CAM_FRUSTUM
//#define DEBUG_WORLD_GRID_DRAW_INDICES
//#define DEBUG_WORLD_GRID_CELL_DRAW
#define DEBUG_WORLD_AREA_DRAW

/*--------------------------------------------------------------------------------------------*/
// Board
/*--------------------------------------------------------------------------------------------*/

#define DEBUG_BOARD_FIELD_DIVER_AREA_DRAW
//#define DEBUG_BOARD_FIELD_DIVER_PATH_SURFACE_DRAW
//#define DEBUG_BOARD_FIELD_DIVER_DIVER

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
// DiverField
/*--------------------------------------------------------------------------------------------*/

#define DIVER_FIELD_NUM_DIVERS_MIN 5
#define DIVER_FIELD_NUM_DIVERS_MAX 50
#define DIVER_FIELD_SURFACE_PATH_DENSITY 1.0f
#define DIVER_FIELD_SURFACE_PATH_OFFSET_SPEED   0.0025f
#define DIVER_FIELD_SURFACE_PATH_AMPLITUDE 0.125f//0.325f


#define DIVER_FIELD_DIVER_NUM_POINTS 20 //10
#define DIVER_FIELD_DIVER_MIN_HEIGHT 0.015f
#define DIVER_FIELD_DIVER_MAX_HEIGHT 0.065f
#define DIVER_FIELD_DIVER_MIN_OFFSET -0.9f//1.0f
#define DIVER_FIELD_DIVER_MAX_OFFSET -1.0f
#define DIVER_FIELD_DIVER_MIN_SPEED  0.0025f
#define DIVER_FIELD_DIVER_MAX_SPEED  0.0045f
#define DIVER_FIELD_DIVER_MIN_LENGTH 1.0f//0.1f
#define DIVER_FIELD_DIVER_MAX_LENGTH 1.0f//0.275f


/*--------------------------------------------------------------------------------------------*/
// JSON
/*--------------------------------------------------------------------------------------------*/

namespace config {
    inline void Load(){}
}


