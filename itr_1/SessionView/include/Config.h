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

#define COLOR_NEXT_FUCHSIA ci::Color::hex(0xDE3865)
#define COLOR_NEXT_BLUE

#define RES_PATH_DATA_JSON      "/Users/automat/Projects/next/data/data.json"
#define RES_PATH_IMAGES_SPEAKER "/Users/automat/Projects/next/data/images_dome_stage"
#define RES_PATH_IMAGES_CLOCK   "/Users/automat/Projects/next/data/clocks"


#define LOGO_SIZE 180
#define LOGO_MARGIN_TOP 100
#define LOGO_MARGIN_RIGHT 100 - LOGO_SIZE

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Session View
//
////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------*/
//  Views
/*--------------------------------------------------------------------------------------------*/

//#define SESSION_VIEW_DEBUG_STATE
//#define SESSION_VIEW_DEBUG_DRAW

//#define SESSION_VIEW_LABEL_SESSION_META_DEBUG_DRAW
//#define SESSION_VIEW_LABEL_EVENT_META_DEBUG_DRAW
//#define SESSION_VIEW_LABEL_EVENT_TITLE_DEBUG_DRAW
//#define SESSION_VIEW_LABEL_SESSION_TITLE_DEBUG_DRAW
//#define SESSION_VIEW_LABEL_SPEAKER_DEBUG_DRAW

#define SESSION_VIEW_SLIDE_LENGTH 3.5f
#define SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET 0.5f

#define SESSION_EVENT_ANIM_IN_OUT   1.35f
#define SESSION_EVENT_ANIM_TIME_OFF 2.0f
#define SESSION_EVENT_ANUM_TIME_ON  2.0f

#define SESSION_EVENT_TIME_MAX 2.0f    //10
#define SESSION_EVENT_TIME_SPEAKER_MIN 1.0f  //3

#define SESSION_VIEW_SPEAKER_SIZE ci::Vec2f(0.85f,0.025f)
#define SESSION_VIEW_SPEAKER_NAME_LABEL_OFFSET 0.065f

#define SESSION_VIEW_START_DELAY 0.5f
#define SESSION_VIEW_ON_FINISH_DELAY 1.0f

//
//  SpeakerStack
//

#define SESSION_SPEAKER_STACK_ANIM_OUT 0.35f
#define SESSION_SPEAKER_STACK_ANIM_MOVE 1.5f
#define SESSION_SPEAKER_STACK_ANIM_IN_SCALE 0.75f
#define SESSION_SPEAKER_STACK_ANIM_IN_TRANSLATION 1.5f

//
//  Speaker
//

#define SESSION_SPEAKER_VIEW_ANIM_FOCUS 0.35f
#define SESSION_SPEAKER_VIEW_ANIM_UNFOCUS 2.5f
#define SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_OUT 2.5f
#define SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_IN 2.5f
#define SESSION_SPEAKER_VIEW_ANIM_SHOW 8.0f
#define SESSION_SPEAKER_VIEW_ANIM_HIDE 0.35f
#define SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_IMAGE 0.35f

/*--------------------------------------------------------------------------------------------*/
//  Title Labels
/*--------------------------------------------------------------------------------------------*/

//
//  Shared font
//

#define SESSION_LABEL_TITLE_FONT_SCALAR 4

#define SESSION_LABEL_TITLE_DEFAULT_FONT_SIZE 118
#define SESSION_LABEL_TITLE_DEFAULT_LINE_HEIGHT 1.275f
#define SESSION_LABEL_TITLE_DEFAULT_UNDERLINE_HEIGHT 32

#define SESSION_LABEL_TITLE_SHADOW_STRENGTH 0.05f
#define SESSION_LABEL_TITLE_SHADOW_OFFSET Vec2f(3,3)

//
//  Session
//

#define SESSION_LABEL_SESSION_TITLE_POS              ci::Vec2f(255,320)
#define SESSION_LABEL_SESSION_TITLE_FONT_SIZE        SESSION_LABEL_TITLE_DEFAULT_FONT_SIZE
#define SESSION_LABEL_SESSION_TITLE_FONT_SCALAR      SESSION_LABEL_TITLE_FONT_SCALAR
#define SESSION_LABEL_SESSION_TITLE_FONT_COLOR       ci::Color::hex(0x00ADE2)
#define SESSION_LABEL_SESSION_TITLE_UNDERLINE_COLOR  ci::Color::hex(0x042B3B)
#define SESSION_LABEL_SESSION_TITLE_UNDERLINE_HEIGHT SESSION_LABEL_TITLE_DEFAULT_UNDERLINE_HEIGHT
#define SESSION_LABEL_SESSION_TITLE_LINE_HEIGHT      SESSION_LABEL_TITLE_DEFAULT_LINE_HEIGHT
#define SESSION_LABEL_SESSION_TITLE_BOX_WIDTH        580
#define SESSION_LABEL_SESSION_TITLE_SHADOW_COLOR     ci::ColorAf(0,0,0,0.5f)
#define SESSION_LABEL_SESSION_TITLE_SHADOW_OFFSET    SESSION_LABEL_TITLE_SHADOW_OFFSET
#define SESSION_LABEL_SESSION_TITLE_SHADOW_STRENGTH  SESSION_LABEL_TITLE_SHADOW_STRENGTH


//  anim
#define SESSION_LABEL_SESSION_TITLE_ANIM_TIME_ALPHA_OFF  0.9f



//
//  Event
//

#define SESSION_LABEL_EVENT_TITLE_MAX_LINES 4
#define SESSION_LABEL_EVENT_TITLE_EXCEED_FONT_SIZE 96
#define SESSION_LABEL_EVENT_TITLE_EXCEED_LINE_HEIGHT 1.275f
#define SESSION_LABEL_EVENT_TITLE_EXCEED_UNDERLINE_HEIGHT 32

#define SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_START ci::Color::hex(0xDE3865)
#define SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR_END   ci::Color::hex(0x87184E)

#define SESSION_LABEL_EVENT_TITLE_EXCEED_BOX_WIDTH  900
#define SESSION_LABEL_EVENT_TITLE_DEFAULT_BOX_WIDTH 1000

#define SESSION_LABEL_EVENT_TITLE_POS                ci::Vec2f(1160,320)
#define SESSION_LABEL_EVENT_TITLE_FONT_SIZE          SESSION_LABEL_TITLE_DEFAULT_FONT_SIZE
#define SESSION_LABEL_EVENT_TITLE_FONT_SCALAR        SESSION_LABEL_TITLE_FONT_SCALAR
#define SESSION_LABEL_EVENT_TITLE_FONT_COLOR         ci::Color::white()
#define SESSION_LABEL_EVENT_TITLE_UNDERLINE_COLOR    ci::Color::hex(0xDE3865)
#define SESSION_LABEL_EVENT_TITLE_UNDERLINE_HEIGHT   SESSION_LABEL_TITLE_DEFAULT_UNDERLINE_HEIGHT
#define SESSION_LABEL_EVENT_TITLE_LINE_HEIGHT        SESSION_LABEL_TITLE_DEFAULT_LINE_HEIGHT
#define SESSION_LABEL_EVENT_TITLE_SHADOW_COLOR       ci::ColorAf(0,0,0,0.5f)
#define SESSION_LABEL_EVENT_TITLE_SHADOW_OFFSET      SESSION_LABEL_TITLE_SHADOW_OFFSET
#define SESSION_LABEL_EVENT_TITLE_SHADOW_STRENGTH    SESSION_LABEL_TITLE_SHADOW_STRENGTH

#define SESSION_LABEL_EVENT_TITLE_ANIM_OFFSET_IN       50.0f
#define SESSION_LABEL_EVENT_TITLE_ANIM_OFFSET_OUT      0.0f
#define SESSION_LABEL_EVENT_TITLE_ANIM_TIME_OFFSET_IN  1.0f
#define SESSION_LABEL_EVENT_TITLE_ANIM_TIME_OFFSET_OUT 1.0f
#define SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_IN   2.5f
#define SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_OUT  0.5f
#define SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_ON   2.0f
#define SESSION_LABEL_EVENT_TITLE_ANIM_TIME_ALPHA_OFF  2.0f


/*--------------------------------------------------------------------------------------------*/
//  Meta Labels
/*--------------------------------------------------------------------------------------------*/

//
//  Shared
//

#define SESSION_LABEL_META_FONT_SIZE 50
#define SESSION_LABEL_META_FONT_SCALAR 4
#define SESSION_LABEL_META_SUB_BOX_WIDTH 400
#define SESSION_LABEL_META_BOX_HEIGHT 70
#define SESSION_LABEL_META_OFFSET_X -22
#define SESSION_LABEL_META_OFFSET_Y -11
#define SESSION_LABEL_META_CLOCK_MARGIN_RIGHT 65
#define SESSION_LABEL_META_CLOCK_MARGIN_TOP -1

//
//  Session
//

#define SESSION_LABEL_SESSION_META_POS ci::Vec2f(255,216)
#define SESSION_LABEL_SESSION_META_FONT_COLOR ci::Color::hex(0xDE3865)
#define SESSION_LABEL_SESSION_META_BOX_WIDTH 700
#define SESSION_LABEL_SESSION_META_TIME_ALPHA_ON  1.0f
#define SESSION_LABEL_SESSION_META_TIME_ALPHA_OFF 0.9f

//
//  Event
//

#define SESSION_LABEL_EVENT_META_POS ci::Vec2f(1163,216)
#define SESSION_LABEL_EVENT_META_FONT_COLOR ci::Color::white()
#define SESSION_LABEL_EVENT_META_TYPE_INDEX_SPACING 15
#define SESSION_LABEL_EVENT_BOX_WIDTH 700
#define SESSION_LABEL_EVENT_META_ANIM_OFFSET_IN -100
#define SESSION_LABEL_EVENT_META_ANIM_TIME_ALPHA_ON 1.0f
#define SESSION_LABEL_EVENT_META_ANIM_TIME_ALPHA_OFF 1.5f
#define SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_ALPHA_ON 2.0f
#define SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_ALPHA_OFF 1.5f
#define SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_OFFSET_SHOW 1.0f
#define SESSION_LABEL_EVENT_META_SUB_LABEL_ANIM_TIME_ALPHA_SHOW 2.0f
#define SESSION_LABEL_EVENT_META_SUB_LABEL_ANIN_TIME_ALPHA_HIDE 0.5f


/*--------------------------------------------------------------------------------------------*/
//  Speaker Labels
/*--------------------------------------------------------------------------------------------*/

#define SESSION_LABEL_SPEAKER_BOX_WIDTH 1500
#define SESSION_LABEL_SPEAKER_FONT_SIZE SESSION_LABEL_TITLE_DEFAULT_FONT_SIZE
#define SESSION_LABEL_SPEAKER_FONT_SCALE 6
#define SESSION_LABEL_SPEAKER_LINE_HEIGHT 1.15f

#define SESSION_LABEL_SPEAKER_NAME_BOX_WIDTH 1500
#define SESSION_LABEL_SPEAKER_FONT_SIZE SESSION_LABEL_TITLE_DEFAULT_FONT_SIZE
#define SESSION_LABEL_SPEAKER_NAME_FONT_COLOR ci::ColorAf::white()
#define SESSION_LABEL_SPEAKER_COMPANY_FONT_COLOR ci::Color::hex(0xDE3865)
#define SESSION_LABEL_SPEAKER_COMPANY_LINE_HEIGHT 1.095f


#define SESSION_LABEL_SESSION_TIME_FONT_SIZE 24 * SCALE


#endif
