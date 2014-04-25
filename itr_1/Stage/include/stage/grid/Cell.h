//
//  Cell.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Cell_h
#define grid_00_Cell_h

#include "cinder/Vector.h"

#include "layout/geom/LayoutArea.h"
#include "stage/grid/Index.h"

namespace next{
    using namespace ci;

    class Cell {
    protected:
        Index      mIndex;
        Vec3f      mPos;
        LayoutArea mArea;

        void debugDrawArea_Internal();

    public:
        Cell(const Index& index, const Vec3f& pos);

        void debugDrawArea();

        inline const LayoutArea& getArea() const{
            return mArea;
        }

        inline const Vec3f& getCenter() const{
            return mPos;
        }

        inline const Index& getIndex() const {
            return mIndex;
        }
    };
}




#endif
