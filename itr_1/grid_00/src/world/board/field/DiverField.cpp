#include "world/board/field/DiverField.h"



DiverField::DiverField(const Index& index, const Vec3f& pos) :
AbstractField(index, pos){
    
}

void DiverField::debugDrawArea(){
    glColor3f(1,0,0);
    debugDrawArea_Internal();
}