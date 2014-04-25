//
//  ShaderUtil.h
//  grid_00
//
//  Created by Henryk Wollik on 29/03/14.
//
//

#ifndef grid_00_ShaderUtil_h
#define grid_00_ShaderUtil_h

#include <string>

#include "cinder/DataSource.h"
#include "cinder/gl/GlslProg.h"

#include "util/FileWatcher.h"

namespace util {
    using namespace ci;
    using namespace std;
    
    void loadShader(DataSourceRef refVertGLSL, DataSourceRef refFragGLSL, gl::GlslProg *prog);
    bool reloadShader(DataSourceRef refVertGLSL, DataSourceRef refFragGLSL, gl::GlslProg* prog);
    bool watchShaderSource(FileWatcher& fileWatcher, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL, gl::GlslProg* prog);
    bool watchShaderSource(const std::shared_ptr<FileWatcher>& fileWatcher, DataSourceRef refVertGLSL, DataSourceRef refFragGLSL, gl::GlslProg* prog);
}



#endif
