#include "util/ShaderUtil.h"

namespace utils {
    using namespace ci;
    using namespace std;
    
    void loadShader(DataSourceRef refVertGLSL, DataSourceRef refFragGLSL, gl::GlslProg *prog){
        try {
            *prog = gl::GlslProg(refVertGLSL,refFragGLSL);
        } catch (gl::GlslProgCompileExc &exc) {
            cout << exc.what() << endl;
        } catch (...){
            throw "Can't load shader.";
        }
    }
    
    bool reloadShader(gl::GlslProg* prog,  const string& pathVertAbs, const string& pathFragAbs){
        gl::GlslProg temp;
        bool success = true;
        
        try {
            temp = gl::GlslProg(loadFile(pathVertAbs),loadFile(pathFragAbs));
        } catch (gl::GlslProgCompileExc &exc) {
            cout << exc.what() << endl;
            success = false;
        } catch (...){
            success = false;
            throw "Can´t load shader.";
        }
        
        if(success){
            cout << "Shader updated." << endl;
            *prog = temp;
        }
        
        return success;
    }
    
    void watchShaderSource(FileWatcher& fileWatcher, const string& pathVertAbs, const string& pathFragAbs, gl::GlslProg* shader){
        if(!fileWatcher.hasFile(pathVertAbs)){
            fileWatcher.addFile(pathVertAbs);
        }
        
        if(!fileWatcher.hasFile(pathFragAbs)){
            fileWatcher.addFile(pathFragAbs);
        }
        
        if (fileWatcher.fileDidChange(pathVertAbs)) {
            cout << pathVertAbs << " modified." << endl;
            reloadShader(shader, pathVertAbs, pathFragAbs);
        }
        
        if (fileWatcher.fileDidChange(pathFragAbs)) {
            cout << pathFragAbs << " modified." << endl;
            reloadShader(shader, pathVertAbs, pathFragAbs);
        }
    }
}