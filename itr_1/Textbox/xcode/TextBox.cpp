#include "TextBox.h"
#include "cinder/app/App.h"
#include "Resources.h"

namespace next{
    gl::GlslProg     TextBox::_blurShaderH;
    gl::GlslProg     TextBox::_blurShaderV;
    gl::Fbo::Format  TextBox::_fboFormat;
}

