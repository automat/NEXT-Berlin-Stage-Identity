//
//  PersonStack.h
//  grid_01
//
//  Created by Henryk Wollik on 11/04/14.
//
//

#ifndef grid_01_PersonStack_h
#define grid_01_PersonStack_h

#include <vector>
#include "stage/schedule/person/PersonView.h"
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>



using namespace std;
using namespace boost::assign;
class PersonViewStack {
    vector<PersonView*> mViews;
    
public:
    PersonViewStack(const vector<Person*> persons);
    ~PersonViewStack();
    
    void draw();
    void update();
    
    void debugDraw();
    
};



#endif
