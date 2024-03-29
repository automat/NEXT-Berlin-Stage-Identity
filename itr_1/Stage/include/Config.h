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
#include "cinder/Area.h"
#include "cinder/CinderResources.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Overall
//
////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------*/
// App
/*--------------------------------------------------------------------------------------------*/

#define          APP_SCALE     1.0f
#define          APP_SCALE_INV 1.0f / APP_SCALE

#define          APP_WIDTH     3840 / APP_SCALE   // 2x Full HD
#define          APP_WIDTH_2   APP_WIDTH / 2.0f   // 1x Full HD
#define          APP_HEIGHT    1200 / APP_SCALE
#define          APP_SIZE      ci::Vec2f(APP_WIDTH, APP_HEIGHT)

extern bool      APP_BORDERLESS;
extern int       APP_DISPLAY;
extern bool      APP_ALWAYS_ON_TOP;
extern bool      APP_FIXED_POSITION;
extern ci::Vec2i APP_POSITION;
extern float     APP_FPS;
extern bool      APP_HIDE_MOUSE;
extern bool      APP_USE_V_SYNC;


/*--------------------------------------------------------------------------------------------*/
// Stage
/*--------------------------------------------------------------------------------------------*/

#define STAGE_WIDTH        3552 / APP_SCALE
#define STAGE_HEIGHT       1200 / APP_SCALE
#define STAGE_SIZE         ci::Vec2f(STAGE_WIDTH,STAGE_HEIGHT)
#define STAGE_BOUNDS       ci::Area(0,0,STAGE_WIDTH,STAGE_HEIGHT)

/*--------------------------------------------------------------------------------------------*/
// Projection
/*--------------------------------------------------------------------------------------------*/

extern bool  PROJECTION_BLEND;
extern int   PROJECTION_BLEND_EDGE;
extern bool  PROJECTION_BLEND_DEBUG;


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Stage
//
////////////////////////////////////////////////////////////////////////////////////////////////

#define COLOR_NEXT_FUCHSIA ci::Color::hex(0xDE3865)
#define COLOR_NEXT_BLUE ci::Color::hex(0x00ADE2)

#define LOGO_SIZE 180
#define LOGO_MARGIN_TOP 100
#define LOGO_MARGIN_RIGHT 100 - LOGO_SIZE

#define TWITTER_CTA_LOGO_SIZE 120
#define TWITTER_CTA_LOGO_MARGIN_LEFT 100
#define TWITTER_CTA_LOGO_MARGIN_BOTTOM 100 - TWITTER_CTA_LOGO_SIZE

#define TWITTER_CTA_FONT_SIZE 38
#define TWITTER_CTA_FONT_SCALAR 8
#define TWITTER_CTA_FONT_COLOR_0 ci::Color::white()
#define TWITTER_CTA_FONT_COLOR_1 COLOR_NEXT_BLUE
#define TWITTER_CTA_TEXT_MARGIN_LEFT 20
#define TWITTER_CTA_TEXT_MARGIN_RIGHT 20
#define TWITTER_CTA_TEXT_MARGIN_TOP 8
#define TWITTER_CTA_TEXT_HASHTAG_MARGIN_LEFT 5
#define TWITTER_CTA_TEXT_STRING_0 "What is your New Normal?"
#define TWITTER_CTA_TEXT_STRING_1 "Selected tweets from"
#define TWITTER_CTA_TEXT_STRING_2 "#next14"



#define STAGE_MODEL_SCALE_MIN 0.65f
#define STAGE_MODEL_SCALE STAGE_MODEL_SCALE_MIN
#define STAGE_MODEL_SCALE_MAX STAGE_MODEL_SCALE_MIN * 4
#define STAGE_MODEL_CAM_ZOOM  1
#define STAGE_MODEL_CAM_NEAR_CLIP -5.0f
#define STAGE_MODEL_CAM_FAR_CLIP   5.0f


extern int  STAGE_PLAY_FIRST;
extern bool STAGE_LOOP_FIRST;

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Session View
//
////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------*/
//  Views
/*--------------------------------------------------------------------------------------------*/

#define SESSION_VIEW_SLIDE_LENGTH 5.5f
#define SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET 0.6f

#define SESSION_EVENT_ANIM_IN_OUT   1.35f
#define SESSION_EVENT_ANIM_TIME_OFF 2.0f
#define SESSION_EVENT_ANUM_TIME_ON  2.0f

#define SESSION_EVENT_TIME_MAX 2.0f    //10
#define SESSION_EVENT_TIME_SPEAKER_MIN 1.0f  //3

#define SESSION_VIEW_SPEAKER_SIZE ci::Vec2f(1.6f,0.05f)
#define SESSION_VIEW_SPEAKER_NAME_LABEL_OFFSET 0.065f

#define SESSION_VIEW_START_DELAY 0.5f
#define SESSION_VIEW_ON_FINISH_DELAY 1.0f

extern ci::Vec3f  SESSION_VIEW_LIGHT_EYE;
extern ci::Vec3f  SESSION_VIEW_LIGHT_TARGET;
extern ci::Colorf SESSION_VIEW_LIGHT_COLOR_AMBIENT;
extern ci::Colorf SESSION_VIEW_LIGHT_COLOR_DIFFUSE;
extern ci::Colorf SESSION_VIEW_LIGHT_COLOR_SPECULAR;
extern float      SESSION_VIEW_LIGHT_LINEAR_ATTENUATION;
extern float      SESSION_VIEW_LIGHT_QUADRIC_ATTENUATION;
extern bool       SESSION_VIEW_LIGHT_DEBUG_DRAW;

extern bool       SESSION_VIEW_USE_DYNAMIC_LIGHTING;

//
//  SpeakerStack
//

#define SESSION_SPEAKER_STACK_STEP ci::Vec3f(0,0.075f,0)
#define SESSION_SPEAKER_STACK_ANIM_OUT 0.35f
#define SESSION_SPEAKER_STACK_ANIM_MOVE 0.5f
#define SESSION_SPEAKER_STACK_ANIM_IN_SCALE 0.75f
#define SESSION_SPEAKER_STACK_ANIM_IN_TRANSLATION 1.5f

//
//  Speaker
//
#define SESSION_SPEAKER_VIEW_COLOR_ACTIVE       ci::Colorf(0.87450980392157f, 0.06274509803922f, 0.39607843137255f)
#define SESSION_SPEAKER_VIEW_COLOR_INACTIVE     ci::Colorf(0.0f, 0.39607843137255f, 0.89019607843137f)
#define SESSION_SPEAKER_VIEW_ANIM_FOCUS         1.0f
#define SESSION_SPEAKER_VIEW_ANIM_UNFOCUS       2.5f
#define SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_OUT   2.5f
#define SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_IN    2.5f
#define SESSION_SPEAKER_VIEW_ANIM_SHOW          8.0f
#define SESSION_SPEAKER_VIEW_ANIM_HIDE          0.35f
#define SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_IMAGE 0.35f

extern float SESSION_SPEAKER_VIEW_MATERIAL_SHININESS;

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
#define SESSION_LABEL_META_OFFSET_Y -13
#define SESSION_LABEL_META_CLOCK_MARGIN_RIGHT 65
#define SESSION_LABEL_META_CLOCK_MARGIN_TOP -1
#define SESSION_LABEL_META_TRAPEZOUD_MARGIN_TOP -2

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
#define SESSION_LABEL_SPEAKER_BOX_HEIGHT 512
#define SESSION_LABEL_SPEAKER_FONT_SIZE SESSION_LABEL_TITLE_DEFAULT_FONT_SIZE
#define SESSION_LABEL_SPEAKER_FONT_SCALE 6
#define SESSION_LABEL_SPEAKER_LINE_HEIGHT 1.15f

#define SESSION_LABEL_SPEAKER_NAME_BOX_WIDTH 2000
#define SESSION_LABEL_SPEAKER_FONT_SIZE SESSION_LABEL_TITLE_DEFAULT_FONT_SIZE
#define SESSION_LABEL_SPEAKER_NAME_FONT_COLOR ci::ColorAf::white()
#define SESSION_LABEL_SPEAKER_COMPANY_FONT_COLOR ci::Color::hex(0xDE3865)
#define SESSION_LABEL_SPEAKER_COMPANY_LINE_HEIGHT 1.095f


#define SESSION_LABEL_SESSION_TIME_FONT_SIZE 24 * SCALE



////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Theme
//
////////////////////////////////////////////////////////////////////////////////////////////////

#define STAGE_GRID_NUM_CELLS_XY 13
#define STAGE_TYPESETTER_FONT_SIZE 400.0f
#define STAGE_TYPESETTER_FONT_SCALE 0.7f

//#define STAGE_SKIP_FX_SHADER
#define STAGE_LIVE_EDIT_FX_SHADER

extern float STAGE_FX_SHADER_BLUR_SCALE;
extern float STAGE_FX_SHADER_BLUR_RADIAL_SCALE;
extern float STAGE_FX_SHADER_BLUR_RADIAL_RADIUS_SCALE;

extern ci::Vec3f  STAGE_LANTERN_0_DIRECTION;
extern ci::Colorf STAGE_LANTERN_0_COLOR_AMBIENT;
extern ci::Colorf STAGE_LANTERN_0_COLOR_DIFFUSE;
extern ci::Colorf STAGE_LANTERN_0_COLOR_SPECULAR;
extern float      STAGE_LANTERN_0_ATTENUATION;
extern float      STAGE_LANTERN_0_CONSTANT_ATTENUATION;
extern float      STAGE_LANTERN_0_LINEAR_ATTENUATION;
extern float      STAGE_LANTERN_0_QUADRIC_ATTENUATION;

extern ci::Vec3f  STAGE_LANTERN_1_DIRECTION;
extern ci::Colorf STAGE_LANTERN_1_COLOR_AMBIENT;
extern ci::Colorf STAGE_LANTERN_1_COLOR_DIFFUSE;
extern ci::Colorf STAGE_LANTERN_1_COLOR_SPECULAR;
extern float      STAGE_LANTERN_1_ATTENUATION;
extern float      STAGE_LANTERN_1_CONSTANT_ATTENUATION;
extern float      STAGE_LANTERN_1_LINEAR_ATTENUATION;
extern float      STAGE_LANTERN_1_QUADRIC_ATTENUATION;


//#define DEBUG_STAGE_TYPESETTER
//#define DEBUG_STAGE_TYPESETTER_TEXTURE


/*--------------------------------------------------------------------------------------------*/
// Theme
/*--------------------------------------------------------------------------------------------*/

#define THEME_LIVE_EDIT_MATERIAL_SHADER

//
//  Quote Field Manager
//

#define THEME_FIELD_QUOTE_MANAGER_ANUM_IN_FROM_MIN 0.0f
#define THEME_FIELD_QUOTE_MANAGER_ANUM_IN_FROM_MAX 0.5f
#define THEME_FIELD_QUOTE_MANAGER_ANIM_IN_DELAY_STEP_MIN 0.5f//0.5f//2.0f//2.0f
#define THEME_FIELD_QUOTE_MANAGER_ANIM_IN_DELAY_STEP_MAX 1.0f//1.0f//2.5f//2.5f
#define THEME_FIELD_QUOTE_MANAGER_ANIM_IN_TIME_MIN  5.0f//6.00f//6.0f//6.0f
#define THEME_FIELD_QUOTE_MANAGER_ANIM_IN_TIME_MAX  6.0f//8.00f//8.0f//8.0f

#define THEME_FIELD_QUOTE_MANAGER_ANUM_OUT_TO_MIN 3.0f
#define THEME_FIELD_QUOTE_MANAGER_ANUM_OUT_TO_MAX 3.25f
#define THEME_FIELD_QUOTE_MANAGER_ANIM_OUT_DELAY_STEP_MIN 0.6f//0.6f//0.5f
#define THEME_FIELD_QUOTE_MANAGER_ANIM_OUT_DELAY_STEP_MAX 1.0f//1.0f//1.0f
#define THEME_FIELD_QUOTE_MANAGER_ANIM_OUT_TIME_MIN 3.0f//5.0f//5.0f
#define THEME_FIELD_QUOTE_MANAGER_ANIM_OUT_TIME_MAX 4.0f//6.0f//6.0f

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
extern ci::ColorAf PATH_SURFACE_COLOR;

/*--------------------------------------------------------------------------------------------*/
// DiverField
/*--------------------------------------------------------------------------------------------*/

#define DIVER_FIELD_NUM_DIVERS_MIN 10
#define DIVER_FIELD_NUM_DIVERS_MAX 35
#define DIVER_FIELD_SURFACE_PATH_DENSITY 1.25f
#define DIVER_FIELD_SURFACE_PATH_OFFSET_SPEED  0.001f //0.0125f
#define DIVER_FIELD_SURFACE_PATH_AMPLITUDE 0.65f//0.325f

#define DIVER_FIELD_DIVER_NUM_POINTS    8
#define DIVER_FIELD_DIVER_MIN_HEIGHT    0.015f
#define DIVER_FIELD_DIVER_MAX_HEIGHT    0.045f
#define DIVER_FIELD_DIVER_MIN_OFFSET_X -1.0f//1.0f
#define DIVER_FIELD_DIVER_MAX_OFFSET_X  1.0f//-1.0f
#define DIVER_FIELD_DIVER_MIN_OFFSET_Y  0.0f
#define DIVER_FIELD_DIVER_MAX_OFFSET_Y  0.0f
#define DIVER_FIELD_DIVER_MIN_SPEED     0.0015f
#define DIVER_FIELD_DIVER_MAX_SPEED     0.0045f
#define DIVER_FIELD_DIVER_MIN_LENGTH    0.1f//0.1f
#define DIVER_FIELD_DIVER_MAX_LENGTH    0.275f//0.275f

extern ci::ColorAf DIVER_FIELD_MATERIAL_AMBIENT;
extern ci::ColorAf DIVER_FIELD_MATERIAL_DIFFUSE;
extern ci::ColorAf DIVER_FIELD_MATERIAL_SPECULAR;
extern float       DIVER_FIELD_MATERIAL_SHININESS;

/*--------------------------------------------------------------------------------------------*/
// QuoteField
/*--------------------------------------------------------------------------------------------*/

#define QUOTE_FIELD_NUM_DIVERS_MIN 10//10//10//10//10
#define QUOTE_FIELD_NUM_DIVERS_MAX 35//50//50//50//50
#define QUOTE_FIELD_SURFACE_PATH_DENSITY 0.75f
#define QUOTE_FIELD_SURFACE_PATH_OFFSET_SPEED   0.0025f
#define QUOTE_FIELD_SURFACE_PATH_AMPLITUDE 0.55f//0.25f//0.125f//0.325f


#define QUOTE_FIELD_DIVER_NUM_POINTS    8//20
#define QUOTE_FIELD_DIVER_MIN_HEIGHT    0.015f
#define QUOTE_FIELD_DIVER_MAX_HEIGHT    0.05f
#define QUOTE_FIELD_DIVER_MIN_OFFSET_X  -1.0f
#define QUOTE_FIELD_DIVER_MAX_OFFSET_X  -0.85f
#define QUOTE_FIELD_DIVER_MIN_OFFSET_Y -0.0185f
#define QUOTE_FIELD_DIVER_MAX_OFFSET_Y  0.0185f
#define QUOTE_FIELD_DIVER_MIN_SPEED     0.015f
#define QUOTE_FIELD_DIVER_MAX_SPEED     0.0151f
#define QUOTE_FIELD_DIVER_MIN_LENGTH    1.0f//0.1f
#define QUOTE_FIELD_DIVER_MAX_LENGTH    1.0f//0.275f

extern ci::ColorAf QUOTE_FIELD_MATERIAL_AMBIENT;
extern ci::ColorAf QUOTE_FIELD_MATERIAL_DIFFUSE;
extern ci::ColorAf QUOTE_FIELD_MATERIAL_SPECULAR;
extern float       QUOTE_FIELD_MATERIAL_SHININESS;

//#define QUOTE_FIELD_PUT_NORMAL_COLORS

/*--------------------------------------------------------------------------------------------*/
// JSON
/*--------------------------------------------------------------------------------------------*/

#include <string>
#include "cinder/Json.h"

namespace next{

    using namespace std;
    using namespace ci;

    class Config {
    public:
        static bool LoadJson(const string& filepath, string* msg);
        static bool Reload(string* msg);
        static bool DidChange();
        static bool IsValid();
    };
}

