#include "world/board/field/AbstractField.h"

AbstractField::AbstractField(const Index& index, const Vec3f& pos) :
Cell(index, pos) ,
mActive(true){
    
}

AbstractField::~AbstractField(){
    
}