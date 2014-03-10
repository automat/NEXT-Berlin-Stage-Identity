//
//  Config.h
//  grid_07
//
//  Created by Henryk Wollik on 08/03/14.
//
//

#ifndef grid_07_Config_h
#define grid_07_Config_h

/*--------------------------------------------------------------------------------------------*/
// DEFINES
/*--------------------------------------------------------------------------------------------*/

//
//  Overall
//

#define STAGE_WIDTH  3552
#define STAGE_HEIGHT 1105
#define STAGE_SCALE  2

#define APP_WIDTH  STAGE_WIDTH / STAGE_SCALE
#define APP_HEIGHT STAGE_HEIGHT / STAGE_SCALE
#define APP_FPS 36.0f

//
//  Threads currently dont get syncronized, threrefore we keep the update
//  rate quite high to hide lags
//

#define APP_USE_THREADS
#define APP_CTRL_PATH_THREAD_FPS 240.0f//120.0f//240.0f

/*--------------------------------------------------------------------------------------------*/
// WORLD
/*--------------------------------------------------------------------------------------------*/

#define WORLD_MODEL_ZOOM_MIN   1
#define WORLD_MODEL_ZOOM_MAX   1
#define WORLD_MODEL_SCALE_MIN  WORLD_MODEL_SCALE_STEP
#define WORLD_MODEL_SCALE_MAX  2
#define WORLD_MODEL_SCALE_STEP 0.05f

#define WORLD_MODEL_SCALE_INITIAL 0.65f
#define WORLD_MODEL_ZOOM_INITIAL  1

#define WORLD_NUM_CELLS_XY 11

#define OSCILLATOR_NUM_SOURCES_XY WORLD_NUM_CELLS_XY
#define OSCILLATOR_OCTAVES 1
#define OSCILLATOR_SEED    clock() & 65535

/*--------------------------------------------------------------------------------------------*/
// DRAW / DEBUG
/*--------------------------------------------------------------------------------------------*/

//#define WORLD_DEBUG_DRAW_CELL_AREA
#define WORLD_DEBUG_DRAW_CELL
#define WORLD_DRAW_CELL
//#define APP_USE_NORMAL_DEBUG_SHADER


//#define CELL_DEBUG_DRAW_DIVER
//#define CELL_DEBUG_DRAW_PATH


/*--------------------------------------------------------------------------------------------*/
// CONFIGURABLES
/*--------------------------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include "cinder/Json.h"
#include "cinder/Color.h"
#include "cinder/gl/Material.h"
#include "cinder/Font.h"

using namespace ci;
using namespace std;
using namespace gl;


static Vec3f    LIGHT0_EYE;
static Vec3f    LIGHT0_TARGET;
static Colorf   LIGHT0_AMBIENT;
static Colorf   LIGHT0_DIFFUSE;
static Colorf   LIGHT0_SPECULAR;

static Vec3f    LIGHT1_EYE;
static Vec3f    LIGHT1_TARGET;
static Colorf   LIGHT1_AMBIENT;
static Colorf   LIGHT1_DIFFUSE;
static Colorf   LIGHT1_SPECULAR;

static Material CELL_MATERIAL0;
static Material STRING_CELL_MATERIAL0;

static int      CELL_MIN_NUM_DIVERS;
static int      CELL_MAX_NUM_DIVERS;
static float    CELL_OFFSET_SPEED;
static float    CELL_PATH_AMPLITUDE;
static int      CELL_DIVER_NUM_POINTS;
static float    CELL_DIVER_MIN_HEIGHT;
static float    CELL_DIVER_MAX_HEIGHT;
static float    CELL_DIVER_MIN_OFFSET;
static float    CELL_DIVER_MAX_OFFSET;
static float    CELL_DIVER_MIN_SPEED;
static float    CELL_DIVER_MAX_SPEED;
static float    CELL_DIVER_MIN_LENGTH;
static float    CELL_DIVER_MAX_LENGTH;

static int      STRING_CELL_MIN_NUM_DIVERS;
static int      STRING_CELL_MAX_NUM_DIVERS;
static float    STRING_CELL_OFFSET_SPEED;
static float    STRING_CELL_PATH_AMPLITUDE;
static int      STRING_CELL_DIVER_NUM_POINTS;
static float    STRING_CELL_DIVER_MIN_HEIGHT;
static float    STRING_CELL_DIVER_MAX_HEIGHT;
static float    STRING_CELL_DIVER_MIN_OFFSET;
static float    STRING_CELL_DIVER_MAX_OFFSET;
static float    STRING_CELL_DIVER_MIN_SPEED;
static float    STRING_CELL_DIVER_MAX_SPEED;
static float    STRING_CELL_DIVER_MIN_LENGTH;
static float    STRING_CELL_DIVER_MAX_LENGTH;

static int      PATH_NUM_POINTS;

static float    QUOTE_PADDING[4];
static ColorAf  QUOTE_FONT_COLOR;
static Font     QUOTE_FONT;
static Font     QUOTE_AUTHOR_FONT;

namespace config {
    namespace parse{
        inline static void SetVec3f(const JsonTree& node, Vec3f* vec){
            *vec = Vec3f(node[0].getValue<float>(),
                         node[1].getValue<float>(),
                         node[2].getValue<float>());
        }
        
        inline static void SetColor(const JsonTree& node, ColorAf* color){
            *color = ColorAf(node[0].getValue<float>(),
                             node[1].getValue<float>(),
                             node[2].getValue<float>(),
                             node[3].getValue<float>());
        }
        
        inline static void SetColor(const JsonTree& node, Colorf* color){
            *color = Colorf(node[0].getValue<float>(),
                            node[1].getValue<float>(),
                            node[2].getValue<float>());
        }
        
        inline static void SetMaterial(const JsonTree& node, Material* material){
            ColorAf ambient;
            ColorAf diffuse;
            ColorAf specular;
            
            SetColor(node.getChild("ambient"), &ambient);
            SetColor(node.getChild("diffuse"), &diffuse);
            SetColor(node.getChild("specular"), &specular);
            
            material->setAmbient( ambient);
            material->setDiffuse( diffuse);
            material->setSpecular(specular);
            material->setShininess(node.getChild("shininess").getValue<float>());
        }
    }
    
    
    
    
    inline static void Load(const string& path){
        bool loadSuccess = true;
        JsonTree jsonData;
        
        try{
            jsonData = JsonTree(loadFile(path));
        } catch (JsonTree::Exception& exc) {
            cout << exc.what() << endl;
            loadSuccess = false;
        }
        
        if(loadSuccess){
            bool setSuccess = true;

            Material cell_material0;
            Material string_cell_material0;
            
            Vec3f    light0_eye;
            Vec3f    light0_target;
            Colorf   light0_ambient;
            Colorf   light0_diffuse;
            Colorf   light0_specular;
            
            Vec3f    light1_eye;
            Vec3f    light1_target;
            Colorf   light1_ambient;
            Colorf   light1_diffuse;
            Colorf   light1_specular;
            
            int      cell_min_num_divers;
            int      cell_max_num_divers;
            float    cell_offset_speed;
            float    cell_path_amplitude;
            int      cell_diver_num_points;
            float    cell_diver_min_height;
            float    cell_diver_max_height;
            float    cell_diver_min_offset;
            float    cell_diver_max_offset;
            float    cell_diver_min_speed;
            float    cell_diver_max_speed;
            float    cell_diver_min_length;
            float    cell_diver_max_length;
            
            int      string_cell_min_num_divers;
            int      string_cell_max_num_divers;
            float    string_cell_offset_speed;
            float    string_cell_path_amplitude;
            int      string_cell_diver_num_points;
            float    string_cell_diver_min_height;
            float    string_cell_diver_max_height;
            float    string_cell_diver_min_offset;
            float    string_cell_diver_max_offset;
            float    string_cell_diver_min_speed;
            float    string_cell_diver_max_speed;
            float    string_cell_diver_min_length;
            float    string_cell_diver_max_length;
            
            int      path_num_points;
            
            float    quote_padding[4];
            string   quote_font_family;
            float    quote_font_size;
            ColorAf  quote_font_color;
            
            
            
            
            try {
                parse::SetMaterial(jsonData.getChild("Scene.Cell.Material0"), &cell_material0);
                parse::SetMaterial(jsonData.getChild("Scene.StringCell.Material0"), &string_cell_material0);
                
                parse::SetVec3f(jsonData.getChild("Scene.Light0.eye"),      &light0_eye);
                parse::SetVec3f(jsonData.getChild("Scene.Light0.target"),   &light0_target);
                parse::SetColor(jsonData.getChild("Scene.Light0.ambient"),  &light0_ambient);
                parse::SetColor(jsonData.getChild("Scene.Light0.diffuse"),  &light0_diffuse);
                parse::SetColor(jsonData.getChild("Scene.Light0.specular"), &light0_specular);
                
                parse::SetVec3f(jsonData.getChild("Scene.Light1.eye"),      &light1_eye);
                parse::SetVec3f(jsonData.getChild("Scene.Light1.target"),   &light1_target);
                parse::SetColor(jsonData.getChild("Scene.Light1.ambient"),  &light1_ambient);
                parse::SetColor(jsonData.getChild("Scene.Light1.diffuse"),  &light1_diffuse);
                parse::SetColor(jsonData.getChild("Scene.Light1.specular"), &light1_specular);
                
                cell_min_num_divers   = jsonData.getChild("Scene.Cell.MinNumDivers").getValue<int>();
                cell_max_num_divers   = jsonData.getChild("Scene.Cell.MaxNumDivers").getValue<int>();
                cell_offset_speed     = jsonData.getChild("Scene.Cell.OffsetSpeed").getValue<float>();
                cell_path_amplitude   = jsonData.getChild("Scene.Cell.PathAmplitude").getValue<float>();
                cell_diver_num_points = jsonData.getChild("Scene.Cell.Diver.NumPoints").getValue<int>();
                cell_diver_min_height = jsonData.getChild("Scene.Cell.Diver.MinHeight").getValue<float>();
                cell_diver_max_height = jsonData.getChild("Scene.Cell.Diver.MaxHeight").getValue<float>();
                cell_diver_min_offset = jsonData.getChild("Scene.Cell.Diver.MinOffset").getValue<float>();
                cell_diver_max_offset = jsonData.getChild("Scene.Cell.Diver.MaxOffset").getValue<float>();
                cell_diver_min_speed  = jsonData.getChild("Scene.Cell.Diver.MinSpeed").getValue<float>();
                cell_diver_max_speed  = jsonData.getChild("Scene.Cell.Diver.MaxSpeed").getValue<float>();
                cell_diver_min_length = jsonData.getChild("Scene.Cell.Diver.MinLength").getValue<float>();
                cell_diver_max_length = jsonData.getChild("Scene.Cell.Diver.MaxLength").getValue<float>();
                
                string_cell_min_num_divers   = jsonData.getChild("Scene.StringCell.MinNumDivers").getValue<int>();
                string_cell_max_num_divers   = jsonData.getChild("Scene.StringCell.MaxNumDivers").getValue<int>();
                string_cell_offset_speed     = jsonData.getChild("Scene.StringCell.OffsetSpeed").getValue<float>();
                string_cell_path_amplitude   = jsonData.getChild("Scene.StringCell.PathAmplitude").getValue<float>();
                string_cell_diver_num_points = jsonData.getChild("Scene.StringCell.Diver.NumPoints").getValue<int>();
                string_cell_diver_min_height = jsonData.getChild("Scene.StringCell.Diver.MinHeight").getValue<float>();
                string_cell_diver_max_height = jsonData.getChild("Scene.StringCell.Diver.MaxHeight").getValue<float>();
                string_cell_diver_min_offset = jsonData.getChild("Scene.StringCell.Diver.MinOffset").getValue<float>();
                string_cell_diver_max_offset = jsonData.getChild("Scene.StringCell.Diver.MaxOffset").getValue<float>();
                string_cell_diver_min_speed  = jsonData.getChild("Scene.StringCell.Diver.MinSpeed").getValue<float>();
                string_cell_diver_max_speed  = jsonData.getChild("Scene.StringCell.Diver.MaxSpeed").getValue<float>();
                string_cell_diver_min_length = jsonData.getChild("Scene.StringCell.Diver.MinLength").getValue<float>();
                string_cell_diver_max_length = jsonData.getChild("Scene.StringCell.Diver.MaxLength").getValue<float>();
                
                path_num_points  = jsonData.getChild("Scene.Path.NumPoints").getValue<int>();
                
                quote_padding[0] = jsonData.getChild("Scene.Quote.Padding")[0].getValue<float>();
                quote_padding[1] = jsonData.getChild("Scene.Quote.Padding")[1].getValue<float>();
                quote_padding[2] = jsonData.getChild("Scene.Quote.Padding")[2].getValue<float>();
                quote_padding[3] = jsonData.getChild("Scene.Quote.Padding")[3].getValue<float>();
                
                
                quote_font_family = jsonData.getChild("Scene.Quote.Font.Family").getValue<string>();
                quote_font_size   = jsonData.getChild("Scene.Quote.Font.Size").getValue<float>();
                quote_font_color  = ColorAf(jsonData.getChild("Scene.Quote.Font.Color")[0].getValue<float>(),
                                           jsonData.getChild("Scene.Quote.Font.Color")[1].getValue<float>(),
                                           jsonData.getChild("Scene.Quote.Font.Color")[2].getValue<float>(),
                                           jsonData.getChild("Scene.Quote.Font.Color")[3].getValue<float>());
                
                
                
                
            } catch (JsonTree::Exception& exc) {
                cout << exc.what() << endl;
                setSuccess = false;
            }
            
            if(setSuccess){
                CELL_MATERIAL0  = cell_material0;
                STRING_CELL_MATERIAL0  = string_cell_material0;
                
                LIGHT0_EYE      = light0_eye;
                LIGHT0_TARGET   = light0_target;
                LIGHT0_AMBIENT  = light0_ambient;
                LIGHT0_DIFFUSE  = light0_diffuse;
                LIGHT0_SPECULAR = light0_specular;
                
                LIGHT1_EYE      = light1_eye;
                LIGHT1_TARGET   = light1_target;
                LIGHT1_AMBIENT  = light1_ambient;
                LIGHT1_DIFFUSE  = light1_diffuse;
                LIGHT1_SPECULAR = light1_specular;
                
                CELL_MIN_NUM_DIVERS   = cell_min_num_divers;
                CELL_MAX_NUM_DIVERS   = cell_max_num_divers;
                CELL_OFFSET_SPEED     = cell_offset_speed;
                CELL_PATH_AMPLITUDE   = cell_path_amplitude;
                
                CELL_DIVER_NUM_POINTS = cell_diver_num_points;
                CELL_DIVER_MIN_HEIGHT = cell_diver_min_height;
                CELL_DIVER_MAX_HEIGHT = cell_diver_max_height;
                CELL_DIVER_MIN_OFFSET = cell_diver_min_offset;
                CELL_DIVER_MAX_OFFSET = cell_diver_max_offset;
                CELL_DIVER_MIN_SPEED  = cell_diver_min_speed;
                CELL_DIVER_MAX_SPEED  = cell_diver_max_speed;
                CELL_DIVER_MIN_LENGTH = cell_diver_min_length;
                CELL_DIVER_MAX_LENGTH = cell_diver_max_length;
                
                STRING_CELL_MIN_NUM_DIVERS   = string_cell_min_num_divers;
                STRING_CELL_MAX_NUM_DIVERS   = string_cell_max_num_divers;
                STRING_CELL_OFFSET_SPEED     = string_cell_offset_speed;
                STRING_CELL_PATH_AMPLITUDE   = string_cell_path_amplitude;
                
                STRING_CELL_DIVER_NUM_POINTS = string_cell_diver_num_points;
                STRING_CELL_DIVER_MIN_HEIGHT = string_cell_diver_min_height;
                STRING_CELL_DIVER_MAX_HEIGHT = string_cell_diver_max_height;
                STRING_CELL_DIVER_MIN_OFFSET = string_cell_diver_min_offset;
                STRING_CELL_DIVER_MAX_OFFSET = string_cell_diver_max_offset;
                STRING_CELL_DIVER_MIN_SPEED  = string_cell_diver_min_speed;
                STRING_CELL_DIVER_MAX_SPEED  = string_cell_diver_max_speed;
                STRING_CELL_DIVER_MIN_LENGTH = string_cell_diver_min_length;
                STRING_CELL_DIVER_MAX_LENGTH = string_cell_diver_max_length;
                
                PATH_NUM_POINTS = path_num_points;
                
                QUOTE_PADDING[0] = quote_padding[0];
                QUOTE_PADDING[1] = quote_padding[1];
                QUOTE_PADDING[2] = quote_padding[2];
                QUOTE_PADDING[3] = quote_padding[3];
                
                QUOTE_FONT_COLOR = quote_font_color;
                QUOTE_FONT = Font(quote_font_family,quote_font_size);
                
                
            
                
                cout << path << " updated." << endl;
            }
        }
        
        
    }
}





#endif
