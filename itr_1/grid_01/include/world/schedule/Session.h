//
//  Panel.h
//  grid_01
//
//  Created by Henryk Wollik on 10/04/14.
//
//

#ifndef grid_01_Panel_h
#define grid_01_Panel_h

#include <string>
#include <vector>

#include "World/schedule/Person.h"
#include "World/schedule/Session.h"
#include "world/schedule/Event.h"

#include <map>

using namespace std;

class Session {
    string  mTitle;
    time_t  mTimeStart;
    time_t  mTimeEnd;
    time_t  mTimeDuration;
    vector<Event*> mEvents;
    
    map<uint32_t,Person> mPersonMap;
    map<uint32_t,Event>  mEventMap;
public:


};



#endif
