#include "util/Utils.h"

using namespace std;
namespace utils {
    void removeChar(string& str, const char& c){
        str.erase(remove(str.begin(), str.end(), c), str.end());
    }
    
    bool getIntersection(const Planef& plane, const Vec3f& p0, const Vec3f& p1, Vec3f* intersection){
        const Vec3f& normal = plane.getNormal();
        const Vec3f& point  = plane.getPoint();
        
        float t = normal.dot(point - p0) / normal.dot(p1 - p0);
        
        if(t > 0 && t < 1){
            *intersection = p0 + (p1 - p0) * t;
            return true;
        } else if( t == 0){
            *intersection = p0;
            return true;
        } else if( t == 1){
            *intersection = p1;
            return true;
        }
        return false;
    }

}

