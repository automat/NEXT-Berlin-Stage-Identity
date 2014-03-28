//
//  Index.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Index_h
#define grid_00_Index_h

class Index{
private:
    int x,y;
public:
    Index(const Index& index) : x(index.x), y(index.y){}
    Index(int x = 0,int y = 0) : x(x), y(y){}
    
    inline const int& operator[](int n) const {
        assert( n >= 0 && n <= 1);
        return (&x)[n];
    }
};


#endif
