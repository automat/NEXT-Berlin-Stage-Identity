#include "Config.h"
#include "util/SharedFileWatcher.h"
#include <map>
#include <boost/variant.hpp>


//  naaak
JsonTree::ParseOptions GetParseOptions(){
    JsonTree::ParseOptions options;
    options.ignoreErrors(false);
    return options;
}

// lifetime = application time

bool   __init;
string __filePath("");
bool   __isValid(false);
bool   __isValidInit(false);

JsonTree::ParseOptions __parseOptions      = GetParseOptions();
SharedFileWatcherRef   __sharedFileWatcher = SharedFileWatcher::Get();

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

bool Parse(const cinder::JsonTree &parent, const string &key, Colorf *color, string *msg){
    JsonTree node;
    if(!GetChild(parent, key, &node, msg)){
        return false;
    }
    Colorf _color;
    
    try{
        _color.r = node[0].getValue<float>();
        _color.g = node[1].getValue<float>();
        _color.b = node[2].getValue<float>();
    } catch( JsonTree::Exception& exc){
        *msg = exc.what();
        return false;
    }
    
    *color = _color;
    return true;
}

bool Parse(const cinder::JsonTree &parent, const string &key, ColorAf *color, string *msg){
    JsonTree node;
    if(!GetChild(parent, key, &node, msg)){
        return false;
    }
    ColorAf _color;
    
    try{
        _color.r = node[0].getValue<float>();
        _color.g = node[1].getValue<float>();
        _color.b = node[2].getValue<float>();
        _color.a = node[3].getValue<float>();
    } catch( JsonTree::Exception& exc){
        *msg = exc.what();
        return false;
    }
    color->set(_color);
    return true;
}

bool Parse(const cinder::JsonTree &parent, const string &key, Vec3f *vec3f, string *msg){
    JsonTree node;
    if(!GetChild(parent, key, &node, msg)){
        return false;
    }
    Vec3f _vec3f;
    
    try {
        _vec3f.x = node[0].getValue<float>();
        _vec3f.y = node[1].getValue<float>();
        _vec3f.z = node[2].getValue<float>();
    } catch (JsonTree::Exception& exc) {
        *msg = exc.what();
        return false;
    }
    
    *vec3f = _vec3f;
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

float   WORLD_FX_SHADER_BLUR_SCALE;
float   WORLD_FX_SHADER_BLUR_RADIAL_SCALE;
float   WORLD_FX_SHADER_BLUR_RADIAL_RADIUS_SCALE;

ColorAf PATH_SURFACE_COLOR;

ColorAf DIVER_FIELD_MATERIAL_AMBIENT;
ColorAf DIVER_FIELD_MATERIAL_DIFFUSE;
ColorAf DIVER_FIELD_MATERIAL_SPECULAR;
float   DIVER_FIELD_MATERIAL_SHININESS;

Vec3f   WORLD_LANTERN_0_DIRECTION;
Colorf  WORLD_LANTERN_0_COLOR_AMBIENT;
Colorf  WORLD_LANTERN_0_COLOR_DIFFUSE;
Colorf  WORLD_LANTERN_0_COLOR_SPECULAR;
float   WORLD_LANTERN_0_ATTENUATION;
float   WORLD_LANTERN_0_CONSTANT_ATTENUATION;
float   WORLD_LANTERN_0_LINEAR_ATTENUATION;
float   WORLD_LANTERN_0_QUADRIC_ATTENUATION;
bool    WORLD_LANTERN_0_DEBUG_DRAW;

bool Config::LoadJson(const string &filepath, string *msg){
    JsonTree configJson;
    try {
        configJson = JsonTree(loadFile(filepath), __parseOptions);
    } catch (JsonTree::ExcJsonParserError& exc) {
        *msg = exc.what();
        return __isValid = false;
    }
    
    JsonTree nodeWindow;
    JsonTree nodeWorld;

    try {
        nodeWindow = configJson.getChild("window");
        nodeWorld  = configJson.getChild("scene.layer_quote.world");
    } catch (JsonTree::Exception& exc) {
        *msg = exc.what();
        return __isValid = false;
    }
    
    //
    //  Parse Scene
    //
    
    if(!Parse(nodeWorld, "shader_fx.blur_scale",               &WORLD_FX_SHADER_BLUR_SCALE,               msg) ||
       !Parse(nodeWorld, "shader_fx.blur_scale_radial",        &WORLD_FX_SHADER_BLUR_RADIAL_SCALE,        msg) ||
       !Parse(nodeWorld, "shader_fx.blur_radial_radius_scale", &WORLD_FX_SHADER_BLUR_RADIAL_RADIUS_SCALE, msg) ||
       
       /*--------------------------------------------------------------------------------------------*/
       //	World Lantern 0
       /*--------------------------------------------------------------------------------------------*/
       !Parse(nodeWorld, "light.lantern_0.direction",              &WORLD_LANTERN_0_DIRECTION, msg) ||
       !Parse(nodeWorld, "light.lantern_0.ambient",               &WORLD_LANTERN_0_COLOR_AMBIENT,        msg) ||
       !Parse(nodeWorld, "light.lantern_0.diffuse",               &WORLD_LANTERN_0_COLOR_DIFFUSE,        msg) ||
       !Parse(nodeWorld, "light.lantern_0.specular",              &WORLD_LANTERN_0_COLOR_SPECULAR,       msg) ||
       !Parse(nodeWorld, "light.lantern_0.attenuation",           &WORLD_LANTERN_0_ATTENUATION,          msg) ||
       !Parse(nodeWorld, "light.lantern_0.constant_attenuation",  &WORLD_LANTERN_0_CONSTANT_ATTENUATION, msg) ||
       !Parse(nodeWorld, "light.lantern_0.linear_attenuation",    &WORLD_LANTERN_0_LINEAR_ATTENUATION,   msg) ||
       !Parse(nodeWorld, "light.lantern_0.quadric_attenuation",   &WORLD_LANTERN_0_QUADRIC_ATTENUATION,  msg) ||
       !Parse(nodeWorld, "light.lantern_0.debug_draw",            &WORLD_LANTERN_0_DEBUG_DRAW,           msg) ||
       
       /*--------------------------------------------------------------------------------------------*/
       //	Path Surface
       /*--------------------------------------------------------------------------------------------*/
       !Parse(nodeWorld, "board.path_surface.color", &PATH_SURFACE_COLOR, msg) ||
       
       /*--------------------------------------------------------------------------------------------*/
       //   Diver Field Material
       /*--------------------------------------------------------------------------------------------*/
       !Parse(nodeWorld, "board.diver_field.material.ambient",   &DIVER_FIELD_MATERIAL_AMBIENT,   msg) ||
       !Parse(nodeWorld, "board.diver_field.material.diffuse",   &DIVER_FIELD_MATERIAL_DIFFUSE,   msg) ||
       !Parse(nodeWorld, "board.diver_field.material.specular",  &DIVER_FIELD_MATERIAL_SPECULAR,  msg) ||
       !Parse(nodeWorld, "board.diver_field.material.shininess", &DIVER_FIELD_MATERIAL_SHININESS, msg)){
        return __isValid = false;
    }

    if(filepath != __filePath){
        if(!__filePath.empty() && __sharedFileWatcher->hasFile(__filePath)){
            __sharedFileWatcher->removeFile(__filePath);
        }
        __filePath = filepath;
        __sharedFileWatcher->addFile(__filePath);
        __isValidInit = true;
    }
    return __isValid = true;
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
