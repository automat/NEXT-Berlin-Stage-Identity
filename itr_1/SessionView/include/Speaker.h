//
// Created by Henryk Wollik on 14/04/14.
//

#ifndef __Speaker_H_
#define __Speaker_H_

#include "cinder/gl/Texture.h"
#include <string>
namespace next {
    using namespace std;
    using namespace ci;
    struct Speaker {
        gl::Texture imageRef;
        string companyName;
        string companyRole;
        string name;
        string twitterHandle;
    };
}

#endif
