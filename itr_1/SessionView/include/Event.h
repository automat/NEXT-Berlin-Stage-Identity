//
//  Event.h
//  EventsDeck
//
//  Created by Henryk Wollik on 15/04/14.
//
//

#ifndef EventsDeck_Event_h
#define EventsDeck_Event_h

#include <vector>
#include "Speaker.h"
#include <string>

namespace next {
    using namespace std;
   
    struct Event{
        string endHourString;
        time_t endTimeStamp;
        string startTimeStamp;
        string title;
        string type;
        
        vector<Speaker*> speakers;
    };
}
#endif
