//
//  Slice.h
//  grid_00
//
//  Created by Henryk Wollik on 31/03/14.
//
//

#ifndef grid_00_Slice_h
#define grid_00_Slice_h

#include "cinder/Vector.h"

namespace next{
    using namespace ci;

    class PathSurface;

    class PathSlice {
        friend class PathSurface;

        PathSurface* mSurface;
        int   mIndex;
        int   mNumPoints;
        float mWidth;


    public:
        PathSlice(PathSurface* surface, int index, float width, int numPoints);

        const Vec3f& getStart() const;
        const Vec3f& getEnd() const;
        const Vec3f& getPoint(int index) const;

        //! get point on sliced path
        void getPointOn(float intrpl, Vec3f* out) const;

        //! get number of of slice points
        inline int getNumPoints() const{
            return mNumPoints;
        }

        //! get width of slice
        inline float getWidth() const{
            return mWidth;
        }

        //! get size of surface to the left
        int getSurfaceSize() const;

        //! get summed distance of slice
        float getLength() const;

        //! get summed distance of slice squared
        float getLengthSq() const;
    };
}

#endif
