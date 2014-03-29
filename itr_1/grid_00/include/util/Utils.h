//
//  Utils.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef QuoteLayout_Utils_h
#define QuoteLayout_Utils_h

#include <functional>
#include <algorithm>
#include <string>

#include "cinder/app/App.h"
#include "cinder/Camera.h"
#include "cinder/Plane.h"
#include "FrustumOrtho.h"
#include "layout/geom/LayoutArea.h"

namespace utils {
    using namespace std;
    using namespace ci;

    void removeChar(string& str, const char& c);
    bool getIntersection(const Planef& plane, const Vec3f& p0, const Vec3f& p1, Vec3f* intersection);
}

#endif

