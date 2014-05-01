#include "Config.h"

#include "util/FileWatcher.h"

// (with default values, in case the initial config.json is not initially parsable)
int       WINDOW_WIDTH(3840);
int       WINDOW_HEIGHT(1105);
bool      WINDOW_BORDERLESS(false);
int       WINDOW_DISPLAY(0);
bool      WINDOW_ALWAYS_ON_TOP(false);
bool      WINDOW_FIXED_POSITION(false);
ci::Vec2i WINDOW_POSITION(0,0);
float     WINDOW_FPS(36.0f);

float PROJECTION_SCALE;
bool  PROJECTION_OVERLAP;
int   PROJECTION_OVERLAP_EDGE;

float STAGE_FX_SHADER_BLUR_SCALE;
float STAGE_FX_SHADER_BLUR_RADIAL_SCALE;
float STAGE_FX_SHADER_BLUR_RADIAL_RADIUS_SCALE;

ci::ColorAf PATH_SURFACE_COLOR;

ci::ColorAf DIVER_FIELD_MATERIAL_AMBIENT;
ci::ColorAf DIVER_FIELD_MATERIAL_DIFFUSE;
ci::ColorAf DIVER_FIELD_MATERIAL_SPECULAR;
float       DIVER_FIELD_MATERIAL_SHININESS;

ci::ColorAf QUOTE_FIELD_MATERIAL_AMBIENT;
ci::ColorAf QUOTE_FIELD_MATERIAL_DIFFUSE;
ci::ColorAf QUOTE_FIELD_MATERIAL_SPECULAR;
float       QUOTE_FIELD_MATERIAL_SHININESS;


ci::Vec3f  STAGE_LANTERN_0_DIRECTION;
ci::Colorf STAGE_LANTERN_0_COLOR_AMBIENT;
ci::Colorf STAGE_LANTERN_0_COLOR_DIFFUSE;
ci::Colorf STAGE_LANTERN_0_COLOR_SPECULAR;
float      STAGE_LANTERN_0_ATTENUATION;
float      STAGE_LANTERN_0_CONSTANT_ATTENUATION;
float      STAGE_LANTERN_0_LINEAR_ATTENUATION;
float      STAGE_LANTERN_0_QUADRIC_ATTENUATION;
bool       STAGE_LANTERN_0_DEBUG_DRAW;

ci::Vec3f  STAGE_LANTERN_1_DIRECTION;
ci::Colorf STAGE_LANTERN_1_COLOR_AMBIENT;
ci::Colorf STAGE_LANTERN_1_COLOR_DIFFUSE;
ci::Colorf STAGE_LANTERN_1_COLOR_SPECULAR;
float      STAGE_LANTERN_1_ATTENUATION;
float      STAGE_LANTERN_1_CONSTANT_ATTENUATION;
float      STAGE_LANTERN_1_LINEAR_ATTENUATION;
float      STAGE_LANTERN_1_QUADRIC_ATTENUATION;
bool       STAGE_LANTERN_1_DEBUG_DRAW;

namespace next{

//  naaak
    JsonTree::ParseOptions GetParseOptions(){
        JsonTree::ParseOptions options;
        options.ignoreErrors(false);
        return options;
    }

// lifetime = application time
    string __filePath("");
    bool   __isValid(false);
    bool   __isValidInit(false);

    JsonTree::ParseOptions __parseOptions      = GetParseOptions();
    FileWatcherRef __sharedFileWatcher = FileWatcher::Get();

/*--------------------------------------------------------------------------------------------*/
//	Helper
/*--------------------------------------------------------------------------------------------*/

    bool GetChild(const JsonTree &parent, const string& key, JsonTree* child, string* msg){
        JsonTree _child;
        try {
            _child = parent.getChild(key);
        } catch (JsonTree::Exception& exc) {
            *msg = exc.what();
            return false;
        }
        *child = _child;
        return true;
    }

    bool Parse(const cinder::JsonTree &parent, const string &key, Colorf *value, string *msg){
        JsonTree node;
        if(!GetChild(parent, key, &node, msg)){
            return false;
        }
        Colorf _value;

        try{
            _value.r = node[0].getValue<float>();
            _value.g = node[1].getValue<float>();
            _value.b = node[2].getValue<float>();
        } catch( JsonTree::Exception& exc){
            *msg = exc.what();
            return false;
        }

        *value = _value;
        return true;
    }

    bool Parse(const cinder::JsonTree &parent, const string &key, ColorAf *color, string *msg){
        JsonTree node;
        if(!GetChild(parent, key, &node, msg)){
            return false;
        }
        ColorAf _value;

        try{
            _value.r = node[0].getValue<float>();
            _value.g = node[1].getValue<float>();
            _value.b = node[2].getValue<float>();
            _value.a = node[3].getValue<float>();
        } catch( JsonTree::Exception& exc){
            *msg = exc.what();
            return false;
        }
        *color = _value;
        return true;
    }

    bool Parse(const cinder::JsonTree &parent, const string &key, Vec3f *value, string *msg){
        JsonTree node;
        if(!GetChild(parent, key, &node, msg)){
            return false;
        }
        Vec3f _value;

        try {
            _value.x = node[0].getValue<float>();
            _value.y = node[1].getValue<float>();
            _value.z = node[2].getValue<float>();
        } catch (JsonTree::Exception& exc) {
            *msg = exc.what();
            return false;
        }

        *value = _value;
        return true;
    }

    bool Parse(const cinder::JsonTree &parent, const string &key, Vec2i *value, string *msg){
        JsonTree node;
        if(!GetChild(parent, key, &node, msg)){
            return false;
        }
        Vec2i _value;

        try {
            _value.x = node[0].getValue<float>();
            _value.y = node[1].getValue<float>();
        } catch (JsonTree::Exception& exc) {
            *msg = exc.what();
            return false;
        }

        *value = _value;
        return true;
    }

    bool Parse(const cinder::JsonTree & parent, const string& key, float* value, string* msg){
        JsonTree node;
        if(!GetChild(parent, key, &node, msg)){
            return false;
        }
        float _value;
        try {
            _value = node.getValue<float>();
        } catch (JsonTree::Exception& exc) {
            *msg = exc.what();
            return false;
        }

        *value = _value;
        return true;
    }

    bool Parse(const cinder::JsonTree& parent, const string& key, int* value, string* msg){
        JsonTree node;
        if(!GetChild(parent, key, &node, msg)){
            return false;
        }
        int _value;
        try {
            _value = node.getValue<int>();
        } catch (JsonTree::Exception& exc) {
            *msg = exc.what();
            return false;
        }

        *value = _value;
        return true;
    }

    bool Parse(const cinder::JsonTree& parent, const string& key, bool* value, string* msg){
        JsonTree node;
        if(!GetChild(parent, key, &node, msg)){
            return false;
        }
        bool _value;
        try {
            _value = node.getValue<bool>();
        } catch (JsonTree::Exception& exc) {
            *msg = exc.what();
            return false;
        }

        *value = _value;
        return true;
    };


/*--------------------------------------------------------------------------------------------*/
//	Load
/*--------------------------------------------------------------------------------------------*/

    bool Config::LoadJson(const string &filepath, string *msg){
        JsonTree configJson;
        try {
            configJson = JsonTree(loadFile(filepath), __parseOptions);
        } catch (JsonTree::ExcJsonParserError& exc) {
            *msg = exc.what();
            __isValid = false;
            return __isValid;
        }

        JsonTree nodeWindow;
        JsonTree nodeProjection;
        JsonTree nodeStage;
        JsonTree nodeStageTheme;
        JsonTree nodeStageSchedule;

        try {
            nodeWindow        = configJson.getChild("window");
            nodeProjection    = configJson.getChild("projection");
            nodeStage         = configJson.getChild("scene.stage");
            nodeStageTheme    = nodeStage.getChild("theme");
            nodeStageSchedule = nodeStage.getChild("schedule");
        } catch (JsonTree::Exception& exc) {
            *msg = exc.what();
            __isValid = false;
            return __isValid;
        }

        //
        //  Parse Scene
        //

        if(/*--------------------------------------------------------------------------------------------*/
                //	Window
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeWindow, "width", &WINDOW_WIDTH, msg) ||
                        !Parse(nodeWindow, "height", &WINDOW_HEIGHT, msg) ||
                        !Parse(nodeWindow, "borderless", &WINDOW_BORDERLESS, msg) ||
                        !Parse(nodeWindow, "display", &WINDOW_DISPLAY, msg) ||
                        !Parse(nodeWindow, "always_on_top", &WINDOW_ALWAYS_ON_TOP, msg) ||
                        !Parse(nodeWindow, "fixed_position", &WINDOW_FIXED_POSITION, msg) ||
                        !Parse(nodeWindow, "position", &WINDOW_POSITION, msg) ||
                        !Parse(nodeWindow, "fps", &WINDOW_FPS, msg) ||

                        /*--------------------------------------------------------------------------------------------*/
                        //	Projection
                        /*--------------------------------------------------------------------------------------------*/
                        !Parse(nodeProjection, "scale", &PROJECTION_SCALE, msg) ||
                        !Parse(nodeProjection, "overlap", &PROJECTION_OVERLAP, msg) ||
                        !Parse(nodeProjection, "overlap_edge", &PROJECTION_OVERLAP_EDGE, msg) ||

                        /*--------------------------------------------------------------------------------------------*/
                        //	World
                        /*--------------------------------------------------------------------------------------------*/
                        !Parse(nodeStage, "shader_fx.blur_scale", &STAGE_FX_SHADER_BLUR_SCALE, msg) ||
                        !Parse(nodeStage, "shader_fx.blur_scale_radial", &STAGE_FX_SHADER_BLUR_RADIAL_SCALE,msg) ||
                        !Parse(nodeStage, "shader_fx.blur_radial_radius_scale", &STAGE_FX_SHADER_BLUR_RADIAL_RADIUS_SCALE, msg) ||

                        /*--------------------------------------------------------------------------------------------*/
                        //	World Lantern 0
                        /*--------------------------------------------------------------------------------------------*/
                        !Parse(nodeStage, "light.lantern_0.direction", &STAGE_LANTERN_0_DIRECTION, msg) ||
                        !Parse(nodeStage, "light.lantern_0.ambient", &STAGE_LANTERN_0_COLOR_AMBIENT, msg) ||
                        !Parse(nodeStage, "light.lantern_0.diffuse", &STAGE_LANTERN_0_COLOR_DIFFUSE, msg) ||
                        !Parse(nodeStage, "light.lantern_0.specular", &STAGE_LANTERN_0_COLOR_SPECULAR, msg) ||
                        !Parse(nodeStage, "light.lantern_0.attenuation", &STAGE_LANTERN_0_ATTENUATION, msg) ||
                        !Parse(nodeStage, "light.lantern_0.constant_attenuation", &STAGE_LANTERN_0_CONSTANT_ATTENUATION, msg) ||
                        !Parse(nodeStage, "light.lantern_0.linear_attenuation", &STAGE_LANTERN_0_LINEAR_ATTENUATION, msg) ||
                        !Parse(nodeStage, "light.lantern_0.quadric_attenuation", &STAGE_LANTERN_0_QUADRIC_ATTENUATION, msg) ||
                        !Parse(nodeStage, "light.lantern_0.debug_draw", &STAGE_LANTERN_0_DEBUG_DRAW, msg) ||


                        /*--------------------------------------------------------------------------------------------*/
                        //	World Lantern 1
                        /*--------------------------------------------------------------------------------------------*/
                        !Parse(nodeStage, "light.lantern_1.direction", &STAGE_LANTERN_1_DIRECTION, msg) ||
                        !Parse(nodeStage, "light.lantern_1.ambient", &STAGE_LANTERN_1_COLOR_AMBIENT, msg) ||
                        !Parse(nodeStage, "light.lantern_1.diffuse", &STAGE_LANTERN_1_COLOR_DIFFUSE, msg) ||
                        !Parse(nodeStage, "light.lantern_1.specular", &STAGE_LANTERN_1_COLOR_SPECULAR, msg) ||
                        !Parse(nodeStage, "light.lantern_1.attenuation", &STAGE_LANTERN_1_ATTENUATION, msg) ||
                        !Parse(nodeStage, "light.lantern_1.constant_attenuation", &STAGE_LANTERN_1_CONSTANT_ATTENUATION, msg) ||
                        !Parse(nodeStage, "light.lantern_1.linear_attenuation", &STAGE_LANTERN_1_LINEAR_ATTENUATION, msg) ||
                        !Parse(nodeStage, "light.lantern_1.quadric_attenuation", &STAGE_LANTERN_1_QUADRIC_ATTENUATION, msg) ||
                        !Parse(nodeStage, "light.lantern_1.debug_draw", &STAGE_LANTERN_1_DEBUG_DRAW, msg) ||


                        /*--------------------------------------------------------------------------------------------*/
                        //	Path Surface
                        /*--------------------------------------------------------------------------------------------*/
                        !Parse(nodeStageTheme, "path_surface.color", &PATH_SURFACE_COLOR, msg) ||


                        /*--------------------------------------------------------------------------------------------*/
                        //   Diver Field Material
                        /*--------------------------------------------------------------------------------------------*/
                        !Parse(nodeStageTheme, "diver_field.material.ambient", &DIVER_FIELD_MATERIAL_AMBIENT, msg) ||
                        !Parse(nodeStageTheme, "diver_field.material.diffuse", &DIVER_FIELD_MATERIAL_DIFFUSE, msg) ||
                        !Parse(nodeStageTheme, "diver_field.material.specular", &DIVER_FIELD_MATERIAL_SPECULAR, msg) ||
                        !Parse(nodeStageTheme, "diver_field.material.shininess", &DIVER_FIELD_MATERIAL_SHININESS, msg) ||


                        /*--------------------------------------------------------------------------------------------*/
                        //   Quote Field Material
                        /*--------------------------------------------------------------------------------------------*/
                        !Parse(nodeStageTheme, "quote_field.material.ambient", &QUOTE_FIELD_MATERIAL_AMBIENT, msg) ||
                        !Parse(nodeStageTheme, "quote_field.material.diffuse", &QUOTE_FIELD_MATERIAL_DIFFUSE, msg) ||
                        !Parse(nodeStageTheme, "quote_field.material.specular", &QUOTE_FIELD_MATERIAL_SPECULAR, msg) ||
                        !Parse(nodeStageTheme, "quote_field.material.shininess", &QUOTE_FIELD_MATERIAL_SHININESS, msg)){
            __isValid = false;
            return __isValid;
        }

        if(filepath != __filePath){
            if(!__filePath.empty() && __sharedFileWatcher->hasFile(__filePath)){
                __sharedFileWatcher->removeFile(__filePath);
            }
            __filePath = filepath;
            __sharedFileWatcher->addFile(__filePath);
            __isValidInit = true;
        }
        __isValid = true;
        return __isValid;
    }

    bool Config::Reload(string* msg){
        return LoadJson(__filePath, msg);
    }


/*--------------------------------------------------------------------------------------------*/
//	Check state
/*--------------------------------------------------------------------------------------------*/

    bool Config::DidChange(){
        if(!__isValidInit){
            return false;
        }
        return __sharedFileWatcher->fileDidChange(__filePath);
    }

    bool Config::IsValid(){
        return __isValid;
    }
}
