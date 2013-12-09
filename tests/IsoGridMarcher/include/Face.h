//
//  Face.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 09/12/13.
//
//

#ifndef IsoGridMarcher_Face_h
#define IsoGridMarcher_Face_h

class Face{
    
    
public:
    int ip0;
    int ip1;
    int ip2;
    
    Face(int p0 = 0, int p1 = 0, int p2 = 0) :
    ip0(p0),ip1(p1),ip2(p2){
        
    }
};


#endif
