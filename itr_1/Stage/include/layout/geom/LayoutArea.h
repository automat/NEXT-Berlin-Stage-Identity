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


namespace next{
    using namespace ci;

    class LayoutArea {
    protected:
        Vec3f mA,mB,mC,mD;

    public:
        LayoutArea(){};

        LayoutArea(const LayoutArea& area);
        LayoutArea(const Vec3f& tl, const Vec3f& tr, const Vec3f& bl, const Vec3f& br);
        LayoutArea(float width, float height, bool centered = false);
        LayoutArea(const Vec3f& pos, float width = 1, float height = 1, bool centered = false);

        LayoutArea& operator*=(const Matrix44f& rhs );
        LayoutArea& operator*=(float rhs);

        //! assuming y = 0
        bool contains(const Vec3f& point) const;
        bool contains(const LayoutArea& area) const;

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

        inline const Vec3f& getTL() const{return mA;}
        inline const Vec3f& getTR() const{return mB;}
        inline const Vec3f& getBL() const{return mC;}
        inline const Vec3f& getBR() const{return mD;}

        inline friend std::ostream& operator<<( std::ostream& lhs, const LayoutArea& rhs ){
            lhs << "[" << rhs.mA << "," << rhs.mB << "," << rhs.mC << "," << rhs.mD << "]";
            return lhs;
        }
    };

}

#endif
