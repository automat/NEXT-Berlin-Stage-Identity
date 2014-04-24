//
//  Session.h
//  EventsDeck
//
//  Created by Henryk Wollik on 16/04/14.
//
//

#ifndef EventsDeck_Session_h
#define EventsDeck_Session_h

#include <string>
#include <vector>

#include "stage/session/model/Event.h"
#include "cinder/gl/Texture.h"
#include <map>

using namespace std;
namespace next {
    
    struct Session {
        uint32_t _id;
        string   title;
        string   endHourString;
        time_t   endTimeStamp;
        string   startHourClockFile;
        string   startHourString;
        time_t   startTimeStamp;
        string   type;
        
        map<uint32_t,Event>* events;
    
        ci::gl::Texture clockImageRef;
    };
}



#endif
