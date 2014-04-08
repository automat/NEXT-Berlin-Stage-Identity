#include "Config.h"
#include "util/SharedFileWatcher.h"


//  naaak
JsonTree::ParseOptions GetParseOptions(){
    JsonTree::ParseOptions options;
    options.ignoreErrors(false);
    return options;
}

// lifetime = application time

bool __init;
string __filePath;
JsonTree::ParseOptions __parseOptions = GetParseOptions();
SharedFileWatcherRef __sharedFileWatcher = SharedFileWatcher::Get();

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

bool ParseColor(const cinder::JsonTree &parent, const string &key, Colorf *color, string *msg){
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

bool ParseColor(const cinder::JsonTree &parent, const string &key, ColorAf *color, string *msg){
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
    
    *color = _color;
    return true;
}

bool ParseVec3f(const cinder::JsonTree &parent, const string &key, Vec3f *vec3f, string *msg){
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

bool ParseFloat(const cinder::JsonTree & parent, const string& key, float* value, string* msg){
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


bool Config::LoadJson(const string &filepath, string *msg){
    JsonTree configJson;
    try {
        configJson = JsonTree(loadFile(filepath), __parseOptions);
    } catch (JsonTree::ExcJsonParserError& exc) {
        *msg = exc.what();
        return false;
    }
    
    JsonTree nodeWindow;
    JsonTree nodeWorld;

    try {
        nodeWindow = configJson.getChild("window");
        nodeWorld  = configJson.getChild("scene.layer_quote.world");
    } catch (JsonTree::Exception& exc) {
        *msg = exc.what();
        return false;
    }
    
    //
    //  Scene
    //
    
    
    if(!ParseColor(nodeWorld, "board.diver_field.material.ambient",  &DIVER_FIELD_MATERIAL_AMBIENT, msg) ||
       !ParseColor(nodeWorld, "board.diver_field.material.diffuse",   &DIVER_FIELD_MATERIAL_DIFFUSE, msg) ||
       !ParseColor(nodeWorld, "board.diver_field.material.specular",  &DIVER_FIELD_MATERIAL_SPECULAR, msg) ||
       !ParseFloat(nodeWorld, "board.diver_field.material.shininess", &DIVER_FIELD_MATERIAL_SHININESS, msg)){
        return false;
    }
    
    if(filepath != __filePath){
        if(!__filePath.empty() && __sharedFileWatcher->hasFile(__filePath)){
            __sharedFileWatcher->removeFile(__filePath);
        }
        __filePath = filepath;
        __sharedFileWatcher->addFile(__filePath);
    }
    
    return true;
}


bool Config::DidChange(string* msg){
    if(!__filePath.empty() && __sharedFileWatcher->fileDidChange(__filePath)){
        return Config::LoadJson(__filePath, msg);
    }
    return false;
}