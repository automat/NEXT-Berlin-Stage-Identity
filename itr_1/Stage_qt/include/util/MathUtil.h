//
//  MathUtil.h
//  Stage
//
//  Created by Henryk Wollik on 26/04/14.
//
//

#ifndef Stage_MathUtil_h
#define Stage_MathUtil_h

namespace next {
    namespace util{
        inline float stepSmoothf(float n){
            return n * n * (3.0f - 2.0f * n);
        }
        
        inline float stepSmoothSquaredf(float n){
            float s = stepSmoothf(n);
            return s * s;
        }
        
        inline float stepSmoothInvSquaredf(float n){
            float s1 = 1.0f- stepSmoothf(n);
            return 1.0f - s1 * s1;
        }
        
        inline float stepSmoothCubedf(float n){
            float s = stepSmoothf(n);
            return s * s * s * s;
        }
        
        inline float stepSmoothInvCubedf(float n){
            float s1 = 1.0f - n;
            return 1.0f - s1 * s1 * s1 * s1;
        }
        
        inline float stepSquaredf(float n){
            return n * n;
        }
        
        inline float stepInvSquaredf(float n){
            float s1 = 1.0f - n;
            return 1.0f - s1 * s1;
        }
        
        inline float stepCubedf(float n){
            return n * n * n * n;
        }
        
        inline float stepInvCubedf(float n){
            float s1 = 1.0f - n;
            return 1.0f - s1 * s1 * s1 * s1;
        }
    }
}


#endif
