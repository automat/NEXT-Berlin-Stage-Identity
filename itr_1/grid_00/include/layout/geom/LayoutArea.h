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
#include <iostream>

using namespace ci;

class LayoutArea {
protected:
    Vec3f mA,mB,mC,mD;
    
public:
    LayoutArea(){};
    
    LayoutArea(const LayoutArea& area) :
    mA(area.mA),mB(area.mB),mC(area.mC),mD(area.mD){}
    
    
    LayoutArea(const Vec3f& tl, const Vec3f& tr, const Vec3f& bl, const Vec3f& br) :
    mA(tl),mB(tr),mC(bl),mD(br){}
    
    LayoutArea(float width, float height, float centered = false){
        if(centered){
            float width_2  = width * 0.5f,
            height_2 = height * 0.5f;
            mA = Vec3f(-width_2, 0,-height_2);
            mB = Vec3f( width_2, 0,-height_2);
            mC = Vec3f(-width_2, 0, height_2);
            mD = Vec3f( width_2, 0, height_2);
        } else {
            mA = Vec3f(0,     0, 0);
            mB = Vec3f(width, 0, 0);
            mC = Vec3f(0,     0, height);
            mD = Vec3f(width, 0, height);
        }
    }
    
    LayoutArea(const Vec3f& pos, float width = 1, float height = 1, bool centered = false){
        if(centered){
            float width_2  = width * 0.5f,
            height_2 = height * 0.5f;
            mA = Vec3f(pos.x - width_2, 0, pos.z - height_2);
            mB = Vec3f(pos.x + width_2, 0, pos.z - height_2);
            mC = Vec3f(pos.x - width_2, 0, pos.z + height_2);
            mD = Vec3f(pos.x + width_2, 0, pos.z + height_2);
        } else {
            mA = Vec3f(pos.x,         0, pos.z);
            mB = Vec3f(pos.x + width, 0, pos.z);
            mC = Vec3f(pos.x,         0, pos.z + height);
            mD = Vec3f(pos.x + width, 0, pos.z + height);
        }
    }
    
    inline LayoutArea& operator*=(const Matrix44f& rhs ){
        mA = rhs.transformPointAffine(mA);
        mB = rhs.transformPointAffine(mB);
        mC = rhs.transformPointAffine(mC);
        mD = rhs.transformPointAffine(mD);
        return *this;
    }
    
    inline LayoutArea& operator*=(float rhs){
        mA *= rhs;
        mB *= rhs;
        mC *= rhs;
        mD *= rhs;
        return *this;
    }
    
    //! assuming y = 0
    inline bool contains(const Vec3f& point) const{
        float areaABCD = (mA - mB).cross(mA - mC).length() * 0.5f +
        (mB - mD).cross(mB - mC).length() * 0.5f;
        float areaAPD  = (mA - point).cross(mA - mD).length() * 0.5f,
        areaDPC  = (mD - point).cross(mD - mC).length() * 0.5f,
        areaCPB  = (mC - point).cross(mC - mB).length() * 0.5f,
        areaPBA  = (point - mB).cross(point - mA).length() * 0.5f;
        
        return (areaAPD + areaDPC + areaCPB + areaPBA) <= areaABCD;
    }
    
    inline bool contains(const LayoutArea& area) const{
        return contains(area.mA) && contains(area.mB) && contains(area.mC) && contains(area.mD);
    }
    
    inline float getWidth() const{
        return (mB - mA).length();
    };
    
    inline float getHeight() const{
        return (mC - mA).length();
    }
    
    inline float getWidth(){
        return (mB - mA).length();
    }
    
    inline float getHeight(){
        return (mC - mA).length();
    }
    
    inline const Vec3f& getTL(){return mA;}
    inline const Vec3f& getTR(){return mB;}
    inline const Vec3f& getBL(){return mC;}
    inline const Vec3f& getBR(){return mD;}
    
    inline friend std::ostream& operator<<( std::ostream& lhs, const LayoutArea& rhs ){
		lhs << "[" << rhs.mA << "," << rhs.mB << "," << rhs.mC << "," << rhs.mD << "]";
		return lhs;
	}
};

#endif
