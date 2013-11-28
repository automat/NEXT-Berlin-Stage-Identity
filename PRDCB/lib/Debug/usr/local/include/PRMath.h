//
//  PRMath.h
//  PRDCB
//
//  Created by Henryk Wollik on 17/11/13.
//
//

#ifndef PRDCB_PRMath_h
#define PRDCB_PRMath_h

#include <math.h>
#include "cinder/Vector.h"

#ifndef P_PI
#define P_PI 3.14159265f
#endif

#ifndef P_2PI
#define P_2PI 6.28318530f
#endif

#ifndef P_PI2
#define P_PI2 1.57079632f
#endif

#ifndef P_PI4
#define P_PI4 0.78539816f
#endif

#ifndef P_EPSILON
#define P_EPSILON 0.0001f
#endif

namespace pr {
    namespace math{
        inline float lerpf(float a, float b, float v){
            return (a*(1-v))+(b*v);}
        
        inline void lerpf(float* a, float b, float v){
            *a = *a * (1.0f - v) + (b * v);
        }
        
        inline void lerp(const ci::Vec3f& v0, const ci::Vec3f& v1, float v, ci::Vec3f* vec){
            vec->set(lerpf(v0.x,v1.x,v),
                     lerpf(v0.y,v1.y,v),
                     lerpf(v0.z,v1.z,v));
        }
        
        inline float cosIntrplf(float a,float b,float v){
            float vi = (1.0f- cosf(v * P_PI)) * 0.5;
            return a * (1.0f - vi) + b * vi;
        }
        
        inline void cosIntrplf(float* a, float b, float v){
            float vi = (1.0f - cosf(v * P_PI)) * 0.5f;
            *a = *a * (1.0f - vi) + b * vi;
        }
        
        inline void cosIntrpl(const ci::Vec3f& v0,const ci::Vec3f& v1, float v, ci::Vec3f* vec){
            vec->set(cosIntrplf(v0.x,v1.x,v),
                     cosIntrplf(v0.y,v1.y,v),
                     cosIntrplf(v0.z,v1.z,v));
        }
        
        inline float cubicIntrplf(float a,float b,float c,float d,float v){
            float a0,b0,c0,d0,vv;
            
            vv = v * v;
            a0 = d - c - a + b;
            b0 = a - b - a0;
            c0 = c - a;
            d0 = b;
            
            return a0 * v * vv + b0 * vv + c0 * v + d0;
        }
        
        inline void cubicIntrplf(const ci::Vec3f& v0,
                                 const ci::Vec3f& v1,
                                 const ci::Vec3f& v2,
                                 const ci::Vec3f& v3,
                                 float v,
                                 ci::Vec3f* vec){
            vec->set(cubicIntrplf(v0.x, v1.x, v2.x, v3.x, v),
                     cubicIntrplf(v0.y, v1.y, v2.y, v3.y, v),
                     cubicIntrplf(v0.z, v1.z, v2.z, v3.z, v));
            
        }
        
        inline float hermiteIntrplf(float a,float b,float c,float d, float v, float tension, float bias){
            float v0, v1, v2, v3,
            a0, b0, c0, d0;
            
            tension = (1.0f - tension) * 0.5f;
            
            float biasp = 1 + bias,
            biasn = 1 - bias;
            
            v2  = v * v;
            v3  = v2 * v;
            
            v0  = (b - a) * biasp * tension;
            v0 += (c - b) * biasn * tension;
            v1  = (c - b) * biasp * tension;
            v1 += (d - c) * biasn * tension;
            
            a0  = 2 * v3 - 3 * v2 + 1;
            b0  = v3 - 2 * v2 + v;
            c0  = v3 - v2;
            d0  = -2 * v3 + 3 * v2;
            
            return a0 * b + b0 * v0 + c0 * v1 + d0 * c;
            
        }
        
        inline void hermiteIntrplf(const ci::Vec3f& v0,
                                    const ci::Vec3f& v1,
                                    const ci::Vec3f& v2,
                                    const ci::Vec3f& v3,
                                    float v, float tension,
                                    float bias, ci::Vec3f* vec){
            vec->set(hermiteIntrplf(v0.x, v1.x, v2.x, v3.x, v, tension, bias),
                     hermiteIntrplf(v0.y, v1.y, v2.y, v3.y, v, tension, bias),
                     hermiteIntrplf(v0.z, v1.z, v2.z, v3.z, v, tension, bias));
        }
        
        inline float constrain2f(float a,float b){
            return a > b ? b : a;
        }
        
        inline void constrain2f(float* a, float b){
            *a = *a > b ? b : *a;
        }
        
        inline float constrain3f(float a,float b, float c){
            return (a > c) ? c : (a < b) ? b : a;
        }
        
        inline void constrain3f(float*a, float b, float c){
            *a = (*a > c) ? c : (*a < b) ? b : *a;
        }
        
        inline float normalizef (float value,float start,float end){
            return (value - start) / (end - start);
        }
        
        inline void normalizef(float* value, float start, float end){
            *value = (*value - start) / (end - start);
        }
        
        inline float clampf(float value,float min,float max){
            return fmaxf(min, fminf(max, value));
        }
        
        inline void clampf(float* value, float min, float max){
            *value = fmaxf(min, fminf(max, *value));
        }
        
        inline float sawf(float n){
            return 2.0f * (n  - floorf((0.5f + n )));
        }
        
        inline float fracf(float n){
            return n - floorf(n);
        }
        
        inline float trif(float n){
            return 1.0f -4.0f* fabsf(0.5f - fracf(0.5f * n + 0.25f));
        }
        
        inline float rectf(float n){
            float a = fabsf(n);return (a > 0.5f) ? 0.0f : (a == 0.5f) ? 0.5f : (a < 0.5f) ? 1.0f : -1.0f;
        }

        inline float sgnf(float n){
            return n / fabsf(n);
        }
        
        inline float GreatestCommonDivisorf(float a, float b){
            return b == 0.0f ? a : GreatestCommonDivisorf(b, fmodf(a, b));
        }
        
        inline float isFloatEqualf(float a,float b){
            return fabsf(a-b) < P_EPSILON;
        }
        
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
        
        inline float catmullromf(float a, float b, float c, float d, float i){
            return a * ((-i + 2.0f) * i - 1.0f) * i * 0.5f +
            b * (((3.0f * i - 5.0f) * i) * i + 2.0f) * 0.5f +
            c * ((-3.0f * i + 4.0f) * i + 1) * i * 0.5f +
            d * ((i - 1.0f) * i * i) * 0.5f;
        }
        
        inline float fastInvSqrtf(float x){
            float xhalf = 0.5f*x;
            int i = *(int*)&x;
            i = 0x5f3759df - (i>>1);
            x = *(float*)&i;
            x = x*(1.5f - xhalf*x*x);
            return x;
        }
        
        inline float degrees(float radians){
            return radians * 57.2958f ;
        }
        
        inline float radians(float degree){
            return degree * 0.0174533f;
        }
        

        
    }
}

#endif
