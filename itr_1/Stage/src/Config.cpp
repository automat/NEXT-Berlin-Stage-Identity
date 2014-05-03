#include "Config.h"

#include "util/FileWatcher.h"

bool      APP_BORDERLESS;
int       APP_DISPLAY;
bool      APP_ALWAYS_ON_TOP;
bool      APP_FIXED_POSITION;
ci::Vec2i APP_POSITION;
float     APP_FPS;
bool      APP_HIDE_MOUSE;
bool      APP_USE_V_SYNC;

bool  PROJECTION_BLEND;
int   PROJECTION_BLEND_EDGE;
bool  PROJECTION_BLEND_DEBUG;

int   STAGE_PLAY_FIRST;


ci::Vec3f  SESSION_VIEW_LIGHT_EYE;
ci::Vec3f  SESSION_VIEW_LIGHT_TARGET;
ci::Colorf SESSION_VIEW_LIGHT_COLOR_AMBIENT;
ci::Colorf SESSION_VIEW_LIGHT_COLOR_DIFFUSE;
ci::Colorf SESSION_VIEW_LIGHT_COLOR_SPECULAR;
float      SESSION_VIEW_LIGHT_LINEAR_ATTENUATION;
float      SESSION_VIEW_LIGHT_QUADRIC_ATTENUATION;
bool       SESSION_VIEW_LIGHT_DEBUG_DRAW;

bool       SESSION_VIEW_USE_DYNAMIC_LIGHTING;

float      SESSION_SPEAKER_VIEW_MATERIAL_SHININESS;


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


ci::Vec3f  STAGE_LANTERN_1_DIRECTION;
ci::Colorf STAGE_LANTERN_1_COLOR_AMBIENT;
ci::Colorf STAGE_LANTERN_1_COLOR_DIFFUSE;
ci::Colorf STAGE_LANTERN_1_COLOR_SPECULAR;
float      STAGE_LANTERN_1_ATTENUATION;
float      STAGE_LANTERN_1_CONSTANT_ATTENUATION;
float      STAGE_LANTERN_1_LINEAR_ATTENUATION;
float      STAGE_LANTERN_1_QUADRIC_ATTENUATION;


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

    JsonTree::ParseOptions __parseOptions = GetParseOptions();
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

        JsonTree nodeApp;
        JsonTree nodeProjection;
        JsonTree nodeStage;
        JsonTree nodeTheme;
        JsonTree nodeSession;
        

        try {
            nodeApp        = configJson.getChild("app");
            nodeProjection = configJson.getChild("projection");
            nodeStage      = configJson.getChild("stage");
            nodeTheme      = nodeStage.getChild( "theme");
            nodeSession    = nodeStage.getChild( "session");
        } catch (JsonTree::Exception& exc) {
            *msg = exc.what();
            __isValid = false;
            return __isValid;
        }

        //
        //  Parse Scene
        //

        if(     /*--------------------------------------------------------------------------------------------*/
                //	App
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeApp, "borderless", &APP_BORDERLESS, msg) ||
                !Parse(nodeApp, "display", &APP_DISPLAY, msg) ||
                !Parse(nodeApp, "always_on_top", &APP_ALWAYS_ON_TOP, msg) ||
                !Parse(nodeApp, "fixed_position", &APP_FIXED_POSITION, msg) ||
                !Parse(nodeApp, "position", &APP_POSITION, msg) ||
                !Parse(nodeApp, "fps", &APP_FPS, msg) ||
                !Parse(nodeApp, "hide_mouse", &APP_HIDE_MOUSE, msg) ||
                !Parse(nodeApp, "use_v_sync", &APP_USE_V_SYNC, msg) ||

                /*--------------------------------------------------------------------------------------------*/
                //	Projection
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeProjection, "blend", &PROJECTION_BLEND, msg) ||
                !Parse(nodeProjection, "blend_edge", &PROJECTION_BLEND_EDGE, msg) ||
                !Parse(nodeProjection, "debug", &PROJECTION_BLEND_DEBUG, msg) ||
           
                /*--------------------------------------------------------------------------------------------*/
                //	Stage
                /*--------------------------------------------------------------------------------------------*/
           
                !Parse(nodeStage, "play_first", &STAGE_PLAY_FIRST, msg) ||

                /*--------------------------------------------------------------------------------------------*/
                //	Session
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeSession, "light.eye", &SESSION_VIEW_LIGHT_EYE, msg) ||
                !Parse(nodeSession, "light.target", &SESSION_VIEW_LIGHT_TARGET, msg) ||
                !Parse(nodeSession, "light.ambient", &SESSION_VIEW_LIGHT_COLOR_AMBIENT, msg) ||
                !Parse(nodeSession, "light.diffuse", &SESSION_VIEW_LIGHT_COLOR_DIFFUSE, msg) ||
                !Parse(nodeSession, "light.specular", &SESSION_VIEW_LIGHT_COLOR_SPECULAR, msg) ||
                !Parse(nodeSession, "light.linear_attenuation", &SESSION_VIEW_LIGHT_LINEAR_ATTENUATION, msg) ||
                !Parse(nodeSession, "light.quadric_attenuation", &SESSION_VIEW_LIGHT_QUADRIC_ATTENUATION, msg) ||
                !Parse(nodeSession, "light.debug", &SESSION_VIEW_LIGHT_DEBUG_DRAW, msg) ||
           
                !Parse(nodeSession, "use_dynamic_lighting", &SESSION_VIEW_USE_DYNAMIC_LIGHTING, msg) ||
           
           
                !Parse(nodeSession, "speaker_view.material.shininess", &SESSION_SPEAKER_VIEW_MATERIAL_SHININESS, msg) ||

           
                /*--------------------------------------------------------------------------------------------*/
                //	Theme
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeTheme, "shader_fx.blur_scale", &STAGE_FX_SHADER_BLUR_SCALE, msg) ||
                !Parse(nodeTheme, "shader_fx.blur_scale_radial", &STAGE_FX_SHADER_BLUR_RADIAL_SCALE,msg) ||
                !Parse(nodeTheme, "shader_fx.blur_radial_radius_scale", &STAGE_FX_SHADER_BLUR_RADIAL_RADIUS_SCALE, msg) ||

                /*--------------------------------------------------------------------------------------------*/
                //	Theme Lantern 0
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeTheme, "light.lantern_0.direction", &STAGE_LANTERN_0_DIRECTION, msg) ||
                !Parse(nodeTheme, "light.lantern_0.ambient", &STAGE_LANTERN_0_COLOR_AMBIENT, msg) ||
                !Parse(nodeTheme, "light.lantern_0.diffuse", &STAGE_LANTERN_0_COLOR_DIFFUSE, msg) ||
                !Parse(nodeTheme, "light.lantern_0.specular", &STAGE_LANTERN_0_COLOR_SPECULAR, msg) ||
                !Parse(nodeTheme, "light.lantern_0.attenuation", &STAGE_LANTERN_0_ATTENUATION, msg) ||
                !Parse(nodeTheme, "light.lantern_0.constant_attenuation", &STAGE_LANTERN_0_CONSTANT_ATTENUATION, msg) ||
                !Parse(nodeTheme, "light.lantern_0.linear_attenuation", &STAGE_LANTERN_0_LINEAR_ATTENUATION, msg) ||
                !Parse(nodeTheme, "light.lantern_0.quadric_attenuation", &STAGE_LANTERN_0_QUADRIC_ATTENUATION, msg) ||


                /*--------------------------------------------------------------------------------------------*/
                //	Theme Lantern 1
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeTheme, "light.lantern_1.direction", &STAGE_LANTERN_1_DIRECTION, msg) ||
                !Parse(nodeTheme, "light.lantern_1.ambient", &STAGE_LANTERN_1_COLOR_AMBIENT, msg) ||
                !Parse(nodeTheme, "light.lantern_1.diffuse", &STAGE_LANTERN_1_COLOR_DIFFUSE, msg) ||
                !Parse(nodeTheme, "light.lantern_1.specular", &STAGE_LANTERN_1_COLOR_SPECULAR, msg) ||
                !Parse(nodeTheme, "light.lantern_1.attenuation", &STAGE_LANTERN_1_ATTENUATION, msg) ||
                !Parse(nodeTheme, "light.lantern_1.constant_attenuation", &STAGE_LANTERN_1_CONSTANT_ATTENUATION, msg) ||
                !Parse(nodeTheme, "light.lantern_1.linear_attenuation", &STAGE_LANTERN_1_LINEAR_ATTENUATION, msg) ||
                !Parse(nodeTheme, "light.lantern_1.quadric_attenuation", &STAGE_LANTERN_1_QUADRIC_ATTENUATION, msg) ||


                /*--------------------------------------------------------------------------------------------*/
                //	Path Surface
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeTheme, "path_surface.color", &PATH_SURFACE_COLOR, msg) ||


                /*--------------------------------------------------------------------------------------------*/
                //   Diver Field Material
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeTheme, "diver_field.material.ambient", &DIVER_FIELD_MATERIAL_AMBIENT, msg) ||
                !Parse(nodeTheme, "diver_field.material.diffuse", &DIVER_FIELD_MATERIAL_DIFFUSE, msg) ||
                !Parse(nodeTheme, "diver_field.material.specular", &DIVER_FIELD_MATERIAL_SPECULAR, msg) ||
                !Parse(nodeTheme, "diver_field.material.shininess", &DIVER_FIELD_MATERIAL_SHININESS, msg) ||


                /*--------------------------------------------------------------------------------------------*/
                //   Quote Field Material
                /*--------------------------------------------------------------------------------------------*/
                !Parse(nodeTheme, "quote_field.material.ambient", &QUOTE_FIELD_MATERIAL_AMBIENT, msg) ||
                !Parse(nodeTheme, "quote_field.material.diffuse", &QUOTE_FIELD_MATERIAL_DIFFUSE, msg) ||
                !Parse(nodeTheme, "quote_field.material.specular", &QUOTE_FIELD_MATERIAL_SPECULAR, msg) ||
                !Parse(nodeTheme, "quote_field.material.shininess", &QUOTE_FIELD_MATERIAL_SHININESS, msg)){
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
