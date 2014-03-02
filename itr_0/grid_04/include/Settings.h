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
#define APP_FPS 30.0f

/*--------------------------------------------------------------------------------------------*/
// WORLD
/*--------------------------------------------------------------------------------------------*/


#define WORLD_NUM_CELLS_XY 11

#define CELL_MIN_NUM_DIVERS   1
#define CELL_MAX_NUM_DIVERS   1
#define CELL_DIVER_NUM_POINTS 10

#define DIVER_MIN_SPEED  0.005f
#define DIVER_MAX_SPEED  0.007f
#define DIVER_MIN_LENGTH 0.5f
#define DIVER_MAX_LENGTH 1.0f

#endif
