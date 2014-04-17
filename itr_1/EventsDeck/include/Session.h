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

#include "Event.h"
#include <map>

using namespace std;
namespace next {
    
    class Session {
        uint32_t mId;
        string   mTitle;
        time_t   mTimeStart;
        time_t   mTimeEnd;
        
        vector<Event>* mEvents;
    public:
        
        inline static Session Create(uint32_t ID, const string& title, time_t timeStart, time_t timeEnd, vector<Event>* events){
            Session session;
            session.mId = ID;
            session.mTitle = title;
            session.mTimeStart = timeStart;
            session.mTimeEnd = timeEnd;
            session.mEvents = events;
            return session;
        }
        
        inline vector<Event>* getEvents(){
            return mEvents;
        }
    };
}



#endif
