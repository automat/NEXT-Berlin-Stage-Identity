//
//  Config.h
//  grid_07
//
//  Created by Henryk Wollik on 08/03/14.
//
//

#ifndef grid_07_Config_h
#define grid_07_Config_h

#include <iostream>
#include <string>
#include "cinder/Json.h"
#include "cinder/Color.h"
#include "cinder/gl/Material.h"

using namespace ci;
using namespace std;
using namespace gl;

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

//
//  World
//

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

//
//  Draw / Debug
//

//#define WORLD_DEBUG_DRAW_CELL_AREA
//#define WORLD_DEBUG_DRAW_CELL
#define WORLD_DRAW_CELL
//#define APP_USE_NORMAL_DEBUG_SHADER

#define CELL_CALCULATE_NORMALS

//#define CELL_DEBUG_DRAW_DIVER
//#define CELL_DEBUG_DRAW_PATH


/*--------------------------------------------------------------------------------------------*/
// WORLD
/*--------------------------------------------------------------------------------------------*/

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
static Material CELL_MATERIAL1;

static int      CELL_MIN_NUM_DIVERS;
static int      CELL_MAX_NUM_DIVERS;
static float    CELL_OFFSET_SPEED;

static int      PATH_NUM_POINTS;

static int      DIVER_NUM_POINTS;
static float    DIVER_MIN_HEIGHT;
static float    DIVER_MAX_HEIGHT;
static float    DIVER_MIN_OFFSET;
static float    DIVER_MAX_OFFSET;
static float    DIVER_MIN_SPEED;
static float    DIVER_MAX_SPEED;
static float    DIVER_MIN_LENGTH;
static float    DIVER_MAX_LENGTH;


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
            Material cell_material1;
            
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
            
            int      path_num_points;
            
            int      diver_num_points;
            float    diver_min_height;
            float    diver_max_height;
            float    diver_min_offset;
            float    diver_max_offset;
            float    diver_min_speed;
            float    diver_max_speed;
            float    diver_min_length;
            float    diver_max_length;
            
            try {
                parse::SetMaterial(jsonData.getChild("Scene.Cell.Material0"), &cell_material0);
                parse::SetMaterial(jsonData.getChild("Scene.Cell.Material1"), &cell_material1);
                
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
                
                cell_min_num_divers = jsonData.getChild("Scene.Cell.MinNumDivers").getValue<int>();
                cell_max_num_divers = jsonData.getChild("Scene.Cell.MaxNumDivers").getValue<int>();
                cell_offset_speed   = jsonData.getChild("Scene.Cell.OffsetSpeed").getValue<float>();
                
                path_num_points  = jsonData.getChild("Scene.Path.NumPoints").getValue<int>();
                
                diver_num_points = jsonData.getChild("Scene.Diver.NumPoints").getValue<int>();
                diver_min_height = jsonData.getChild("Scene.Diver.MinHeight").getValue<float>();
                diver_max_height = jsonData.getChild("Scene.Diver.MaxHeight").getValue<float>();
                diver_min_offset = jsonData.getChild("Scene.Diver.MinOffset").getValue<float>();
                diver_max_offset = jsonData.getChild("Scene.Diver.MaxOffset").getValue<float>();
                diver_min_speed  = jsonData.getChild("Scene.Diver.MinSpeed").getValue<float>();
                diver_max_speed  = jsonData.getChild("Scene.Diver.MaxSpeed").getValue<float>();
                diver_min_length = jsonData.getChild("Scene.Diver.MinLength").getValue<float>();
                diver_max_length = jsonData.getChild("Scene.Diver.MaxLength").getValue<float>();
                
                
            } catch (JsonTree::Exception& exc) {
                cout << exc.what() << endl;
                setSuccess = false;
            }
            
            if(setSuccess){
                CELL_MATERIAL0  = cell_material0;
                CELL_MATERIAL1  = cell_material1;
                
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
                
                CELL_MIN_NUM_DIVERS = cell_min_num_divers;
                CELL_MAX_NUM_DIVERS = cell_max_num_divers;
                CELL_OFFSET_SPEED   = cell_offset_speed;
                
                PATH_NUM_POINTS = path_num_points;
                
                DIVER_NUM_POINTS = diver_num_points;
                DIVER_MIN_HEIGHT = diver_min_height;
                DIVER_MAX_HEIGHT = diver_max_height;
                DIVER_MIN_OFFSET = diver_min_offset;
                DIVER_MAX_OFFSET = diver_max_offset;
                DIVER_MIN_SPEED  = diver_min_speed;
                DIVER_MAX_SPEED  = diver_max_speed;
                DIVER_MIN_LENGTH = diver_min_length;
                DIVER_MAX_LENGTH = diver_max_length;
                
                cout << path << " updated." << endl;
            }
        }
        
        
    }
}





#endif
