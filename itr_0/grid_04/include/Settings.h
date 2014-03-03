//
//  Settings.h
//  grid_04
//
//  Created by Henryk Wollik on 02/03/14.
//
//

#ifndef grid_04_Settings_h
#define grid_04_Settings_h

/*--------------------------------------------------------------------------------------------*/
// OVERALL
/*--------------------------------------------------------------------------------------------*/

#define STAGE_WIDTH  3552
#define STAGE_HEIGHT 1105
#define STAGE_SCALE  2

#define APP_WIDTH  STAGE_WIDTH / STAGE_SCALE
#define APP_HEIGHT STAGE_HEIGHT / STAGE_SCALE
#define APP_FPS 36.0f



#define APP_USE_THREADS
#define APP_CTRL_PATH_THREAD_FPS 40.0f

/*--------------------------------------------------------------------------------------------*/
// WORLD
/*--------------------------------------------------------------------------------------------*/

#define WORLD_MODEL_ZOOM_MIN   1
#define WORLD_MODEL_ZOOM_MAX   1
#define WORLD_MODEL_SCALE_MIN  WORLD_MODEL_SCALE_STEP
#define WORLD_MODEL_SCALE_MAX  1
#define WORLD_MODEL_SCALE_STEP 0.05f

#define WORLD_MODEL_SCALE_INITIAL 0.65f
#define WORLD_MODEL_ZOOM_INITIAL  1

#define WORLD_NUM_CELLS_XY 1

#define OSCILLATOR_NUM_SOURCES_XY WORLD_NUM_CELLS_XY
#define OSCILLATOR_OCTAVES 1
#define OSCILLATOR_SEED    clock() & 65535

#define CELL_MIN_NUM_DIVERS   2
#define CELL_MAX_NUM_DIVERS   2
#define CELL_DIVER_NUM_POINTS 3
#define CELL_DIVER_MIN_HEIGHT 0.01f
#define CELL_DIVER_MAX_HEIGHT 0.05f


#define DIVER_MIN_OFFSET 0
#define DIVER_MAX_OFFSET 0
#define DIVER_MIN_SPEED  0.005f
#define DIVER_MAX_SPEED  0.015f
#define DIVER_MIN_LENGTH 0.5f
#define DIVER_MAX_LENGTH 1.0f
#define DIVER_NUM_POINTS CELL_DIVER_NUM_POINTS


/*--------------------------------------------------------------------------------------------*/
// DRAW/DEBUG
/*--------------------------------------------------------------------------------------------*/

#define WORLD_DEBUG_DRAW_CELL_AREA
#define WORLD_DEBUG_DRAW_CELL
#define WORLD_DRAW_CELL


//#define CELL_DEBUG_DRAW_DIVER
//#define CELL_DEBUG_DRAW_PATH
//#define CELL_DEBUG_DRAW_MESH_INDICES


#endif
