#pragma once
#include "cinder/CinderResources.h"

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Fonts & Textures
//
////////////////////////////////////////////////////////////////////////////////////////////////

#define RES_FONT_TRANSCRIPT  CINDER_RESOURCE( ../resources/font, transcript_bold.otf, 129, FONT )
#define RES_FONT_APERCU_MONO CINDER_RESOURCE( ../resources/font, Apercu_Mono.otf, 130, FONT )

#define RES_TEXTURE_2x2    CINDER_RESOURCE( ../resources/texture, test_texture_2x2.png, 131, DATA )
#define RES_TEXTURE_NOISE  CINDER_RESOURCE( ../resources/texture, noise.png, 132, DATA )
#define RES_TEXTURE_RANDOM CINDER_RESOURCE( ../resources/texture, random.png, 133, DATA )
#define RES_JSON_TEST      CINDER_RESOURCE( ../resources/, test.json, 134, DATA );

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Testing
//
////////////////////////////////////////////////////////////////////////////////////////////////

#define RES_TEST_IMAGES_PATH "/Users/automat/Projects/next/itr_1/grid_01/resources/image/"
#define RES_TEST_IMAGE_NAME "test_person.jpg"



////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Abs
//
////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------*/
// glsl common
/*--------------------------------------------------------------------------------------------*/

#define RES_ABS_GLSL_PASS_THRU_VERT   "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/pass_thru.glsl"

/*--------------------------------------------------------------------------------------------*/
// glsl bg
/*--------------------------------------------------------------------------------------------*/

#define RES_ABS_GLSL_BG_GRADIENT_FRAG "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/bg/gradient_frag.glsl"
#define RES_ABS_GLSL_BG_MESH_VERT     "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/bg/mesh_vert.glsl"
#define RES_ABS_GLSL_BG_MESH_FRAG     "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/bg/mesh_frag.glsl"
#define RES_ABS_GLSL_BG_MIX_FRAG      "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/bg/mix_frag.glsl"

/*--------------------------------------------------------------------------------------------*/
// glsl fields
/*--------------------------------------------------------------------------------------------*/


#define RES_ABS_GLSL_BOARD_QUOTE_FIELD_VERT "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/material/quote_field_vert.glsl"
#define RES_ABS_GLSL_BOARD_QUOTE_FIELD_FRAG "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/material/quote_field_frag.glsl"
#define RES_ABS_GLSL_BOARD_DIVER_FIELD_VERT "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/material/diver_field_vert.glsl"
#define RES_ABS_GLSL_BOARD_DIVER_FIELD_FRAG "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/material/diver_field_frag.glsl"


/*--------------------------------------------------------------------------------------------*/
// glsl fx
/*--------------------------------------------------------------------------------------------*/

#define RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/normal_depth_frag.glsl"
#define RES_ABS_GLSL_WORLD_FX_NORMAL_DEPTH_VERT "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/normal_depth_vert.glsl"
#define RES_ABS_GLSL_WORLD_FX_SSAO_FRAG         "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/ssao_frag.glsl"
#define RES_ABS_GLSL_WORLD_FX_SSAO_VERT         "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/ssao_vert.glsl"

#define RES_ABS_GLSL_WORLD_FX_BLUR_VERT         "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/blur_vert.glsl"
#define RES_ABS_GLSL_WORLD_FX_BLUR_H_FRAG       "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/blur_h_frag.glsl"
#define RES_ABS_GLSL_WORLD_FX_BLUR_V_FRAG       "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/blur_v_frag.glsl"
#define RES_ABS_GLSL_WORLD_FX_MIX_VERT          "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/mix_vert.glsl"
#define RES_ABS_GLSL_WORLD_FX_MIX_FRAG          "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/mix_frag.glsl"

#define RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_VERT   "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/radial_mix_vert.glsl"
#define RES_ABS_GLSL_WORLD_FX_RADIAL_MIX_FRAG   "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/fx/radial_mix_frag.glsl"

#define RES_ABS_GLSL_WORLD_NORMAL_VERT          "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/material/normal_vert.glsl"
#define RES_ABS_GLSL_WORLD_NORMAL_FRAG          "/Users/automat/Projects/next/itr_1/grid_01/resources/glsl/material/normal_frag.glsl"


////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Relative
//
////////////////////////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------------------------*/
// glsl common
/*--------------------------------------------------------------------------------------------*/

#define RES_GLSL_PASS_THRU CINDER_RESOURCE( ../resources/glsl/, pass_thru.glsl, 135, GLSL )

#define RES_GLSL_NORMAL_VERT CINDER_RESOURCE( ../resources/glsl/material/, normal_vert.glsl, 136, GLSL )
#define RES_GLSL_NORMAL_FRAG CINDER_RESOURCE( ../resources/glsl/material/, normal_frag.glsl, 137, GLSL )

/*--------------------------------------------------------------------------------------------*/
// glsl bg
/*--------------------------------------------------------------------------------------------*/

#define RES_GLSL_BG_GRADIENT_FRAG CINDER_RESOURCE( ../resources/glsl/bg/, bg_gradient_frag.glsl, 138, GLSL )
#define RES_GLSL_BG_MESH_VERT     CINDER_RESOURCE( ../resources/glsl/bg/, bg_mesh_vert.glsl,     139, GLSL )
#define RES_GLSL_BG_MESH_FRAG     CINDER_RESOURCE( ../resources/glsl/bg/, bg_mesh_frag.glsl,     140, GLSL )
#define RES_GLSL_BG_MIX_FRAG      CINDER_RESOURCE( ../resources/glsl/bg/, bg_mix_frag.glsl,      141, GLSL )

/*--------------------------------------------------------------------------------------------*/
// glsl fields
/*--------------------------------------------------------------------------------------------*/

#define RES_GLSL_BOARD_QUOTE_FIELD_VERT CINDER_RESOURCE( ../resources/glsl/material/, quote_field_vert.glsl, 142, GLSL )
#define RES_GLSL_BOARD_QUOTE_FIELD_FRAG CINDER_RESOURCE( ../resources/glsl/material/, quote_field_frag.glsl, 143, GLSL )
#define RES_GLSL_BOARD_DIVER_FIELD_VERT CINDER_RESOURCE( ../resources/glsl/material/, diver_field_vert.glsl, 144, GLSL )
#define RES_GLSL_BOARD_DIVER_FIELD_FRAG CINDER_RESOURCE( ../resources/glsl/material/, diver_field_vert.glsl, 145, GLSL )

/*--------------------------------------------------------------------------------------------*/
// glsl fx
/*--------------------------------------------------------------------------------------------*/

#define RES_GLSL_WORLD_FX_NORMAL_DEPTH_VERT CINDER_RESOURCE( ../resources/glsl/fx, normal_depth_vert.glsl, 146, GLSL )
#define RES_GLSL_WORLD_FX_NORMAL_DEPTH_FRAG CINDER_RESOURCE( ../resources/glsl/fx, normal_depth_frag.glsl, 147, GLSL )
#define RES_GLSL_WORLD_FX_SSAO_VERT         CINDER_RESOURCE( ../resources/glsl/fx, ssao_vert.glsl, 148, GLSL )
#define RES_GLSL_WORLD_FX_SSAO_FRAG         CINDER_RESOURCE( ../resources/glsl/fx, ssao_frag.glsl, 149, GLSL )
#define RES_GLSL_WORLD_FX_BLUR_VERT         CINDER_RESOURCE( ../resources/glsl/fx, blur_vert.glsl, 150, GLSL )
#define RES_GLSL_WORLD_FX_BLUR_H_FRAG       CINDER_RESOURCE( ../resources/glsl/fx, blur_h_frag.glsl, 151, GLSL )
#define RES_GLSL_WORLD_FX_BLUR_V_FRAG       CINDER_RESOURCE( ../resources/glsl/fx, blur_v_frag.glsl, 152, GLSL )
#define RES_GLSL_WORLD_FX_MIX_VERT          CINDER_RESOURCE( ../resources/glsl/fx, mix_vert.glsl, 153, GLSL )
#define RES_GLSL_WORLD_FX_MIX_FRAG          CINDER_RESOURCE( ../resources/glsl/fx, mix_frag.glsl, 154, GLSL )

#define RES_GLSL_WORLD_FX_RADIAL_MIX_VERT   CINDER_RESOURCE( ../resources/glsl/fx, radial_mix_vert.glsl, 155, GLSL )
#define RES_GLSL_WORLD_FX_RADIAL_MIX_FRAG   CINDER_RESOURCE( ../resources/glsl/fx, radial_mix_frag.glsl, 156, GLSL )

#define RES_GLSL_WORLD_NORMAL_VERT          CINDER_RESOURCE( ../resources/glsl/material, normal_vert.glsl, 157, GLSL )
#define RES_GLSL_WORLD_NORMAL_FRAG          CINDER_RESOURCE( ../resources/glsl/material, normal_frag.glsl, 158, GLSL )


