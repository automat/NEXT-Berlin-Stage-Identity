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

#include "stage/schedule/person/Person.h"
#include "stage/schedule/session/Session.h"
#include "stage/schedule/event/Event.h"

#include <map>

using namespace std;

class Session {
    uint32_t mId;
    string   mTitle;
    time_t   mTimeStart;
    time_t   mTimeEnd;
    time_t   mTimeDuration;
    
    vector<Event*> mEvents;
public:
    
    ~Session();
    
    inline static Session Create(uint32_t ID, const string& title, time_t timeStart, time_t timeEnd, time_t timeDuration, const vector<Event*> events){
        Session session;
        session.mId = ID;
        session.mTitle = title;
        session.mTimeStart = timeStart;
        session.mTimeEnd = timeEnd;
        session.mTimeDuration = timeDuration;
        session.mEvents = events;
        return session;
    }
    
    


};



#endif
