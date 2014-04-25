//
//  PersonStackItem.h
//  grid_01
//
//  Created by Henryk Wollik on 11/04/14.
//
//

#ifndef grid_01_PersonStackItem_h
#define grid_01_PersonStackItem_h

#include "stage/schedule/person/Person.h"

class PersonView {
    Person* mData;
public:
    PersonView(Person* data);
    void draw();
    void update();
    void focus();
    void unfocus();
    
    void debugDrawArea();
    
    
    
};



#endif
