//
//  GeomUtil.h
//  grid_00
//
//  Created by Henryk Wollik on 29/03/14.
//
//

#ifndef grid_00_GeomUtil_h
#define grid_00_GeomUtil_h

#include <vector>

#include "cinder/Plane.h"
#include "layout/geom/LayoutArea.h"

namespace next{
    namespace util {
        using namespace std;
        using namespace ci;

        bool getIntersection(const Planef& plane, const Vec3f& p0, const Vec3f& p1, Vec3f* intersection);
        void subdivide(vector<Vec3f>& vertices, vector<uint32_t>& indices, int numSubDivisions = 1, bool normalize = false);
        void randomSubdivide(vector<Vec3f>& vertices, vector<uint32_t>& indices, int numSubDivisions = 1, float threshold = 1, bool normalize = false);

        void genUniqueFaces(vector<Vec3f>& vertices, vector<uint32_t>& indices, vector<Vec3f>& normals);
    }
}



#endif
