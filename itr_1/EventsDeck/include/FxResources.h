//
//  FxShader.h
//  SpeakerImageFilter
//
//  Created by Henryk Wollik on 15/04/14.
//
//

#ifndef SpeakerImageFilter_FxShader_h
#define SpeakerImageFilter_FxShader_h

#include "cinder/gl/GlslProg.h"

using namespace std;
using namespace ci;

class FxResources {
    static gl::GlslProgRef sShaderBlurH;
    static gl::GlslProgRef sShaderBlurV;
    
public:
    static gl::GlslProgRef GetBlurH();
    static gl::GlslProgRef GetBlurV();
    static void Delete();
};


#endif
