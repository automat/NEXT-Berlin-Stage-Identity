//
//  QuoteLayoutArea.h
//  QuoteLayout
//
//  Created by Henryk Wollik on 13/03/14.
//
//

#ifndef QuoteLayout_QuoteLayoutArea_h
#define QuoteLayout_QuoteLayoutArea_h

#include "cinder/Vector.h"
#include "cinder/Matrix44.h"
using namespace ci;

class QuoteLayoutArea {
    Vec3f mA,mB,mC,mD;
private:
    inline float area(const Vec3f& a, const Vec3f& b, const Vec3f& c){
        return (a-b).cross(a-c).length() * 0.5f;
    }
    
    inline float area(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& d){
        return area(a,b,c) + area(b,d,c);
    }
    
public:
    QuoteLayoutArea(const Vec3f& tl, const Vec3f& tr, const Vec3f& bl, const Vec3f& br) :
        mA(tl),mB(tr),mC(bl),mD(br){}
    
    QuoteLayoutArea(float width, float height) :
        mB(width,0,0),mC(0,0,height),mD(width,0,height){}
    
    inline QuoteLayoutArea& operator*=(const Matrix44f& rhs ){
        mA = rhs.transformPointAffine(mA);
        mB = rhs.transformPointAffine(mB);
        mC = rhs.transformPointAffine(mC);
        mD = rhs.transformPointAffine(mD);
        return *this;
    }
    
    //! assuming y = 0
    inline bool contains(const Vec3f& point){
        float areaABCD = (mA - mB).cross(mA - mC).length() * 0.5f +
                         (mB - mD).cross(mB - mC).length() * 0.5f;
        float areaAPD  = (mA - point).cross(mA - mD).length() * 0.5f,
              areaDPC  = (mD - point).cross(mD - mC).length() * 0.5f,
              areaCPB  = (mC - point).cross(mC - mB).length() * 0.5f,
              areaPBA  = (point - mB).cross(point - mA).length() * 0.5f;
        
        return (areaAPD + areaDPC + areaCPB + areaPBA) <= areaABCD;
    }
    
    inline const Vec3f& getTL(){return mA;}
    inline const Vec3f& getTR(){return mB;}
    inline const Vec3f& getBL(){return mC;}
    inline const Vec3f& getBR(){return mD;}
};



#endif