//
//  Config.h
//  ProjectionSplitMerger
//
//  Created by Henryk Wollik on 10/03/14.
//
//

#ifndef ProjectionSplitMerger_Config_h
#define ProjectionSplitMerger_Config_h

#define STAGE_SCREEN_WIDTH 3552
#define STAGE_SCREEN_HEIGHT 1105

#define WINDOW_STAGE_SCREEN_DIV 2 // for fitting the screen
#define WINDOW_OVERLAP_ZONE 288 / WINDOW_STAGE_SCREEN_DIV

#define VIEWPORT_WIDTH  STAGE_SCREEN_WIDTH / WINDOW_STAGE_SCREEN_DIV
#define VIEWPORT_HEIGHT STAGE_SCREEN_HEIGHT / WINDOW_STAGE_SCREEN_DIV

#define WINDOW_WIDTH  VIEWPORT_WIDTH + WINDOW_OVERLAP_ZONE / WINDOW_STAGE_SCREEN_DIV
#define WINDOW_HEIGHT VIEWPORT_HEIGHT
#define APP_FPS 32.0f



#endif
