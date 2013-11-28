//
//  TNBFrame.h
//  PRDCB
//
//  Created by Henryk Wollik on 20/11/13.
//
//

#ifndef PRDCB_SplineFrame_h
#define PRDCB_SplineFrame_h

#include "cinder/Vector.h"
#include "cinder/Matrix44.h"

namespace pr {
    namespace geom{
        class TNBFrame{
        public:
            ci::Vec3f tangent;
            ci::Vec3f normal;
            ci::Vec3f binormal;
        
            TNBFrame() :
            tangent(1,0,0),
            normal(0,1,0),
            binormal(0,0,1){}
            
            TNBFrame(const TNBFrame& srcFrame) :
            tangent(srcFrame.tangent),
            normal(srcFrame.normal),
            binormal(srcFrame.binormal){}
            
            TNBFrame(const ci::Vec3f& srcTangent,
                     const ci::Vec3f& srcNormal,
                     const ci::Vec3f& srcBinormal) :
            tangent(srcTangent),
            normal(srcNormal),
            binormal(srcBinormal){}
            
            void getRotationMatrix(ci::Matrix44f* matrix) const{
               *matrix = ci::Matrix44f::createRotationOnb(tangent, normal, binormal);
            }
            
            void lerp(float intrpl, const TNBFrame& frame){
                float invIntrpl = 1.0f - intrpl;
                tangent  = tangent  * invIntrpl + frame.tangent  * intrpl;
                normal   = normal   * invIntrpl + frame.normal   * intrpl;
                binormal = binormal * invIntrpl + frame.binormal * intrpl;
            }
           
            bool operator==(const TNBFrame& rhs) const{
                return (rhs.tangent  == this->tangent &&
                        rhs.normal   == this->normal &&
                        rhs.binormal == this->binormal);
            }
            
            bool operator!=(const TNBFrame& rhs) const{
                return ! (*this == rhs);
            }
            
            const TNBFrame operator+(const TNBFrame& rhs) const{
                return TNBFrame(tangent  + rhs.tangent,
                                normal   + rhs.normal,
                                binormal + rhs.binormal);
            }
            
            const TNBFrame operator-(const TNBFrame& rhs) const{
                return TNBFrame(tangent  - rhs.tangent,
                                normal   - rhs.normal,
                                binormal - rhs.binormal);
            }
            
            const TNBFrame operator*(const TNBFrame& rhs) const{
                return TNBFrame(tangent  * rhs.tangent,
                                normal   * rhs.normal,
                                binormal * rhs.binormal);
            }
            
            const TNBFrame operator/(const TNBFrame& rhs) const{
                return TNBFrame(tangent  / rhs.tangent,
                                normal   / rhs.normal,
                                binormal / rhs.binormal);
            }
            
            const TNBFrame operator*(double rhs) const{
                return TNBFrame(tangent * rhs, normal * rhs, binormal * rhs);
            }
            
            const TNBFrame operator/(double rhs) const{
                return TNBFrame(tangent / rhs, normal / rhs, binormal / rhs);
            }
            
            
            TNBFrame& operator*=(double rhs){
                tangent *= rhs;
                normal  *= rhs;
                binormal*= rhs;
                return *this;
            }
            
            
            TNBFrame& operator/=(double rhs){
                tangent /= rhs;
                normal  /= rhs;
                binormal/= rhs;
                return *this;
            }
            
            TNBFrame& operator+=(const TNBFrame& rhs){
                tangent += rhs.tangent;
                normal  += rhs.normal;
                binormal+= rhs.binormal;
                return *this;
            }
            
            TNBFrame& operator-=(const TNBFrame& rhs){
                tangent -= rhs.tangent;
                normal  -= rhs.normal;
                binormal-= rhs.binormal;
                return *this;
            }
            
            TNBFrame operator-() const{
                return TNBFrame(-tangent,-normal,-binormal);
            }
            
            
            
            
            
        };
        
        
    }
}



#endif
