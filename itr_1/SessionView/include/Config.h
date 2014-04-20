//
//  Config.h
//  EventsDeck
//
//  Created by Henryk Wollik on 19/04/14.
//
//

#ifndef EventsDeck_Config_h
#define EventsDeck_Config_h

#include "cinder/Color.h"
#include "cinder/Vector.h"

#define SCALE 2

//#define SESSION_VIEW_DEBUG_STATE
//#define SESSION_VIEW_DEBUG_DRAW

#define SESSION_EVENT_TIME_MAX 1.0f    //10
#define SESSION_EVENT_TIME_SPEAKER_MIN 0.0f  //3

#define SESSION_LABEL_TITLE_FONT_SIZE 118
#define SESSION_LABEL_TITLE_FONT_SCALAR 2
#define SESSION_LABEL_TITLE_LINE_HEIGHT 1.275f
#define SESSION_LABEL_TITLE_UNDERLINE_HEIGHT 32
#define SESSION_LABEL_TITLE_SHADOW_STRENGTH 0.05f
#define SESSION_LABEL_TITLE_SHADOW_OFFSET Vec2f(1,1)

#define SESSION_LABEL_SESSION_TITLE_POS              ci::Vec2f(255,320)
#define SESSION_LABEL_SESSION_TITLE_FONT_SIZE        SESSION_LABEL_TITLE_FONT_SIZE
#define SESSION_LABEL_SESSION_TITLE_FONT_SCALAR      SESSION_LABEL_TITLE_FONT_SCALAR
#define SESSION_LABEL_SESSION_TITLE_FONT_COLOR       ci::Color::hex(0x00ADE2)
#define SESSION_LABEL_SESSION_TITLE_UNDERLINE_COLOR  ci::Color::hex(0x042B3B)
#define SESSION_LABEL_SESSION_TITLE_UNDERLINE_HEIGHT SESSION_LABEL_TITLE_UNDERLINE_HEIGHT
#define SESSION_LABEL_SESSION_TITLE_LINE_HEIGHT      SESSION_LABEL_TITLE_LINE_HEIGHT
#define SESSION_LABEL_SESSION_TITLE_BOX_WIDTH        580
#define SESSION_LABEL_SESSION_TITLE_SHADOW_COLOR     ci::ColorAf(0,0,0,0.5f)
#define SESSION_LABEL_SESSION_TITLE_SHADOW_OFFSET    SESSION_LABEL_TITLE_SHADOW_OFFSET
#define SESSION_LABEL_SESSION_TITLE_SHADOW_STRENGTH  SESSION_LABEL_TITLE_SHADOW_STRENGTH



#define SESSION_LABEL_EVENT_TITLE_POS              ci::Vec2f(1160,320)
#define SESSION_LABEL_EVENT_TITLE_FONT_SIZE        SESSION_LABEL_TITLE_FONT_SIZE
#define SESSION_LABEL_EVENT_TITLE_FONT_SCALAR      SESSION_LABEL_TITLE_FONT_SCALAR
#define SESSION_LABEL_EVENT_TITLE_FONT_COLOR       ci::Color::white()
#define SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR  ci::Color::hex(0xDE3865)
#define SESSION_LABEL_EVENT_TITLE_UNDERLINE_HEIGHT SESSION_LABEL_TITLE_UNDERLINE_HEIGHT
#define SESSION_LABEL_EVENT_TITLE_LINE_HEIGHT      SESSION_LABEL_TITLE_LINE_HEIGHT
#define SESSION_LABEL_EVENT_TITLE_BOX_WIDTH        1200
#define SESSION_LABEL_EVENT_TITLE_SHADOW_COLOR     ci::ColorAf(0,0,0,0.75f)
#define SESSION_LABEL_EVENT_TITLE_SHADOW_OFFSET    SESSION_LABEL_TITLE_SHADOW_OFFSET
#define SESSION_LABEL_EVENT_TITLE_SHADOW_STRENGTH  SESSION_LABEL_TITLE_SHADOW_STRENGTH


#define SESSION_LABEL_SESSION_TIME_FONT_SIZE 24 * SCALE


#endif
