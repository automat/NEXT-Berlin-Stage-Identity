#include "layout/geom/LayoutArea.h"

namespace next{

    LayoutArea::LayoutArea(const LayoutArea& area)  :
    mA(area.mA),mB(area.mB),mC(area.mC),mD(area.mD){}

    LayoutArea::LayoutArea(const Vec3f& tl, const Vec3f& tr, const Vec3f& bl, const Vec3f& br) :
    mA(tl),mB(tr),mC(bl),mD(br){}

    LayoutArea::LayoutArea(float width, float height, bool centered){
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

    LayoutArea::LayoutArea(const Vec3f& pos, float width, float height, bool centered){
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


    LayoutArea& LayoutArea::operator*=(const Matrix44f& rhs ){
        mA = rhs.transformPointAffine(mA);
        mB = rhs.transformPointAffine(mB);
        mC = rhs.transformPointAffine(mC);
        mD = rhs.transformPointAffine(mD);
        return *this;
    }

    LayoutArea& LayoutArea::operator*=(float rhs){
        mA *= rhs;
        mB *= rhs;
        mC *= rhs;
        mD *= rhs;
        return *this;
    }

    bool LayoutArea::contains(const Vec3f& point) const{
        float areaABCD = (mA - mB).cross(mA - mC).length() * 0.5f +
                (mB - mD).cross(mB - mC).length() * 0.5f;
        float areaAPD  = (mA - point).cross(mA - mD).length() * 0.5f,
                areaDPC  = (mD - point).cross(mD - mC).length() * 0.5f,
                areaCPB  = (mC - point).cross(mC - mB).length() * 0.5f,
                areaPBA  = (point - mB).cross(point - mA).length() * 0.5f;

        return (areaAPD + areaDPC + areaCPB + areaPBA) <= areaABCD;
    }

    bool LayoutArea::contains(const LayoutArea& area) const{
        return contains(area.mA) && contains(area.mB) && contains(area.mC) && contains(area.mD);
    }
}
