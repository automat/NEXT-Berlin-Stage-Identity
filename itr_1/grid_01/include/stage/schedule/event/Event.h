//
//  Event.h
//  grid_01
//
//  Created by Henryk Wollik on 10/04/14.
//
//

#ifndef grid_01_Event_h
#define grid_01_Event_h

#include <string>
#include <vector>
#include "stage/schedule/person/Person.h"

using namespace std;
class Event {
    uint32_t mId;
    string   mTitle;
    vector<Person*> mPersons;
public:
    inline const string& getTitle(){
        return mTitle;
    }
    
    inline const vector<Person*>& getPersons(){
        return mPersons;
    }
    
    inline static Event Create(uint32_t ID, const string& title, const vector<Person*>& persons){
        Event event;
        event.mId = ID;
        event.mTitle = title;
        event.mPersons = persons;
        
        return event;
    }

};

#endif
