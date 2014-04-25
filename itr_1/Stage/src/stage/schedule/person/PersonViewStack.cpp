//
//  PersonStack.cpp
//  grid_01
//
//  Created by Henryk Wollik on 11/04/14.
//
//

#include "stage/schedule/person/PersonViewStack.h"

PersonViewStack::PersonViewStack(const vector<Person*> person){
    
    
}

PersonViewStack::~PersonViewStack(){
    while (!mViews.empty()) delete mViews.back(), mViews.pop_back();
}

void PersonViewStack::draw(){
    for(vector<PersonView*>::const_iterator itr = mViews.begin(); itr != mViews.end(); itr++){
        (*itr)->draw();
    }
}

void PersonViewStack::update(){
    for(vector<PersonView*>::const_iterator itr = mViews.begin(); itr != mViews.end(); itr++){
        (*itr)->update();
    }
}

void PersonViewStack::debugDraw(){
    for(vector<PersonView*>::const_iterator itr = mViews.begin(); itr != mViews.end(); itr++){
        (*itr)->debugDrawArea();
    }
}
