#include "util/gl/ShaderUtil.h"
#include "cinder/app/App.h"

namespace util {
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
    
    bool reloadShader(DataSourceRef refVertGLSL, DataSourceRef refFragGLSL, gl::GlslProg* prog){
        gl::GlslProg temp;
        bool success = true;
        
        try {
            temp = gl::GlslProg(refVertGLSL,refFragGLSL);
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
    
    bool watchShaderSource(FileWatcher& fileWatcher, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL, gl::GlslProg* prog){
        string absPathVertGLSL = refVertGLSL.get()->getFilePath().string();
        string absPathFragGLSL = refFragGLSL.get()->getFilePath().string();
        bool didChange = false;
        if(!fileWatcher.hasFile(absPathVertGLSL)){
            fileWatcher.addFile(absPathVertGLSL);
        }
        
        if(!fileWatcher.hasFile(absPathFragGLSL)){
            fileWatcher.addFile(absPathFragGLSL);
        }
        
        if (fileWatcher.fileDidChange(absPathVertGLSL)) {
            cout << absPathVertGLSL << " modified." << endl;
            reloadShader(refVertGLSL, refFragGLSL, prog);
            didChange = true;
        }
        
        if (fileWatcher.fileDidChange(absPathFragGLSL)) {
            cout << absPathFragGLSL << " modified." << endl;
            reloadShader(refVertGLSL, refFragGLSL, prog);
            didChange = true;
        }
        
        return didChange;
    }
    
    
    bool watchShaderSource(const std::shared_ptr<FileWatcher>& fileWatcher, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL, gl::GlslProg* prog){
        return watchShaderSource(*fileWatcher.get(), refVertGLSL, refFragGLSL, prog);
    }
}