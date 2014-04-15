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

namespace next {
    using namespace std;
    class Event {
        vector<Speaker>* mSpeakers;
    public:
        inline static Event Create(vector<Speaker>* speakers){
            Event event;
            event.mSpeakers = speakers;
            return event;
        }
        
        inline vector<Speaker>* getSpeakers(){
            return mSpeakers;
        }
    };
}
#endif
