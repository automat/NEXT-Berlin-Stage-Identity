//
//  Speaker.h
//  grid_01
//
//  Created by Henryk Wollik on 10/04/14.
//
//

#ifndef grid_01_Speaker_h
#define grid_01_Speaker_h

#include <string>
#include "cinder/gl/Texture.h"

using namespace std;

class Person {
    uint32_t mId;
    string   mTitle;
    string   mForName;
    string   mLastName;
    string   mJob;
    string   mCompany;
    
public:
    inline uint32_t getId(){return mId;}
    inline string& getTitle(){return mTitle;}
    inline string& getForName(){return mForName;}
    inline string& getLastName(){return mLastName;}
    inline string& getJob(){return mJob;}
    inline string& getCompany(){return mCompany;}
    
    inline static Person Create(uint32_t id_,
                                const string& title,
                                const string& forname,
                                const string& lastname,
                                const string& job,
                                const string& company){
        Person person;
        person.mId = id_;
        person.mTitle = title;
        person.mForName = forname;
        person.mLastName = lastname;
        person.mJob = job;
        person.mCompany = company;
        
        return person;
    }
};

#endif
