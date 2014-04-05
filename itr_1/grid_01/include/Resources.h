#pragma once
#include "cinder/CinderResources.h"

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )

#define RES_FONT_TRANSCRIPT  CINDER_RESOURCE( ../resources/, transcript_bold.otf, 128, FONT )
#define RES_FONT_APERCU_MONO CINDER_RESOURCE( ../resources/, Apercu_Mono.otf, 129, FONT )

#define RES_TEXTURE_2x2 CINDER_RESOURCE( ../resources/, test_texture_2x2.png, 130, DATA )

//
//  Abs
//

#define RES_ABS_GLSL_PASS_THRU_VERT "/Users/automat/Projects/next/itr_1/grid_01/resources/pass_thru.glsl"

#define RES_ABS_GLSL_BG_GRADIENT_FRAG "/Users/automat/Projects/next/itr_1/grid_01/resources/bg_gradient_frag.glsl"
#define RES_ABS_GLSL_BG_MESH_VERT "/Users/automat/Projects/next/itr_1/grid_01/resources/bg_mesh_vert.glsl"
#define RES_ABS_GLSL_BG_MESH_FRAG "/Users/automat/Projects/next/itr_1/grid_01/resources/bg_mesh_frag.glsl"
#define RES_ABS_GLSL_BG_MIX_FRAG "/Users/automat/Projects/next/itr_1/grid_01/resources/bg_mix_frag.glsl"

#define RES_ABS_GLSL_BOARD_QUOTE_FIELD_FRAG "/Users/automat/Projects/next/itr_1/grid_01/resources/quote_field_frag.glsl"
#define RES_ABS_GLSL_BOARD_QUOTE_FIELD_VERT "/Users/automat/Projects/next/itr_1/grid_01/resources/quote_field_vert.glsl"


//
//  Resource relative
//
#define RES_GLSL_PASS_THRU CINDER_RESOURCE( ../resources/, pass_thru.glsl, 131, GLSL )

#define RES_GLSL_NORMAL_VERT CINDER_RESOURCE( ../resources/, normal_vert.glsl, 132, GLSL )
#define RES_GLSL_NORMAL_FRAG CINDER_RESOURCE( ../resources/, normal_frag.glsl, 133, GLSL )

#define RES_GLSL_BG_GRADIENT_FRAG CINDER_RESOURCE( ../resources/, bg_gradient_frag.glsl, 134, GLSL )
#define RES_GLSL_BG_MESH_VERT CINDER_RESOURCE( ../resources/, bg_mesh_vert.glsl, 135, GLSL )
#define RES_GLSL_BG_MESH_FRAG CINDER_RESOURCE( ../resources/, bg_mesh_frag.glsl, 136, GLSL )
#define RES_GLSL_BG_MIX_FRAG CINDER_RESOURCE( ../resources/, bg_mix_frag.glsl, 137, GLSL )

#define RES_GLSL_BOARD_QUOTE_FIELD_FRAG CINDER_RESOURCE( ../resources/, quote_field_frag.glsl, 138, GLSL )
#define RES_GLSL_BOARD_QUOTE_FIELD_VERT CINDER_RESOURCE( ../resources/, quote_field_vert.glsl, 139, GLSL )


#define RES_JSON_TEST CINDER_RESOURCE( ../resources/, test.json, 138, DATA );