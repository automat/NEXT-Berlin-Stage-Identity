//
//  Spline.cpp
//  PRDCB
//
//  Created by Henryk Wollik on 17/11/13.
//
//

#include "Spline.h"
#include "PRDCB/math/PRMath.h"

namespace pr {
    namespace geom{
        Spline::Spline() :
        mDetail(20),
        mTension(0),
        mBias(0),
        mVerticesUpdated(false),
        ITNBSequence(){}
        
        Spline::~Spline(){}
        
        /*----------------------------------------------------------------------------------*/
        
        void Spline::update(){
            if (mPoints.size() == 0)return;
            
            int detail_1 = (int)mDetail - 1;
            size_t len = mPoints.size() - 1;
            
            int i,j;
            float t;
            int index,index_1,index1,index2;

            i = -1;
            while (++i < len) {
                index = i;
                
                index1 = std::min((index + 1),(int)len);
                index2 = std::min((index + 2),(int)len);
                index_1= std::max(0,(index-1));
                
                j = -1;
                while (++j < detail_1) {
                    t = (float)j/(float)detail_1;
                    
                    math::hermiteIntrplf(mPoints[index_1],
                                         mPoints[index  ],
                                         mPoints[index1 ],
                                         mPoints[index2 ],
                                         t, mTension, mBias,
                                         &mVertices[i * detail_1 + j]);
                }
            }
            
            //temp, need to fix begin/end smoothing;
            mVertices.back().set(mPoints.back());
            mVerticesUpdated = false;
        }
        
        /*----------------------------------------------------------------------------------*/
        
        void Spline::recalcFrames(){
            if (mVerticesUpdated)return;
            
            size_t numFrames = mFrames.size();
            ci::Vec3f xAxis = ci::Vec3f::xAxis();
            ci::Vec3f yAxis = ci::Vec3f::yAxis();
            
            
            //calc tangents
            
            int i = -1;
            while (++i < numFrames - 1) {
                mFrames[i].tangent = mVertices[i+1] - mVertices[i];
                mFrames[i].tangent.normalize();
            }
            
            mFrames[numFrames - 1].tangent = mFrames[numFrames - 2].tangent;
            
            
            //calc initial frame
            
            pr::geom::TNBFrame& frameB = mFrames[0];
            frameB.binormal = frameB.tangent.cross( frameB.tangent.dot(yAxis) > 0.998f ?
                                                    xAxis :
                                                    yAxis );
            frameB.binormal.normalize();
            frameB.normal = frameB.binormal.cross(frameB.tangent);
            
            
            //calc rest
            
            float theta;
            ci::Vec3f b;
            i = 0;
            while (++i < numFrames) {
                pr::geom::TNBFrame& frameP = mFrames[i-1];
                pr::geom::TNBFrame& frameC = mFrames[i  ];
                /*
                frameC.normal   = frameP.normal;
                frameC.binormal = frameP.binormal;
                
                b = frameP.tangent.cross(frameC.tangent);
                if(b.length() > 0.0001){
                    b.normalize();
                    theta = acosf(pr::math::clampf(frameP.tangent.dot(frameC.tangent), -1.0f, 1.0f));
                    frameC.normal.rotate(b, theta);
                }
                
                frameC.binormal = frameC.tangent.cross(frameC.normal);
                */
                
                
                float dot = frameC.tangent.dot(yAxis);
                if (frameC.tangent.dot(yAxis) > 0.9998f) {
                    frameC.binormal = frameC.tangent.cross(xAxis);
                    frameC.binormal.normalize();
                    frameC.normal = frameC.binormal.cross(frameC.tangent*-1);
                    
                }
                else
                {
                    frameC.binormal = frameC.tangent.cross(yAxis);
                    frameC.binormal.normalize();
                    frameC.normal = frameC.binormal.cross(frameC.tangent);
                }
                 
                
                
               
                
            }
            
            mVerticesUpdated = true;
        }
       
        
        void Spline::clear(){
            mPoints.clear();
            mVertices.clear();
            mFrames.clear();
        }
        
        /*----------------------------------------------------------------------------------*/
        
        
        void Spline::getPointOnComp(const std::vector<ci::Vec3f>& comp, float intrpl, ci::Vec3f *point){
            intrpl = std::max(0.0f, std::min(intrpl,1.0f));
            
            size_t compSize = comp.size();
            size_t len      = compSize - 1;
            uint index  = (uint)std::floorf((float)compSize * intrpl),
                 index1 = (uint)std::min((uint)index + 1,(uint)len);
            
            float localIntrpl    = fmodf(intrpl, 1.0f / (float)compSize) * (float)compSize,
                  localIntrplInv = 1.0f - localIntrpl;
            
            *point = comp[index] * localIntrplInv + comp[index1] * localIntrpl;
        }
        
        void Spline::getPointOnPoints(float intrpl, ci::Vec3f *point){
            this->getPointOnComp(mPoints, intrpl, point);
        }
        
        void Spline::getPointOnSpline(float intrpl, ci::Vec3f *point){
            this->getPointOnComp(mVertices, intrpl, point);
        }
        
        void Spline::getFrameOnSpline(float intrpl, pr::geom::TNBFrame *frame){
            intrpl = std::max(0.0f, std::min(intrpl,1.0f));
            
            size_t frameSize = mFrames.size();
            int len    = (int)frameSize - 1;
            int index  = (int)std::floorf(intrpl * frameSize);
            int index1 = std::min(index + 1, len);
 
            float localIntrpl    = fmodf(intrpl, 1.0f / (float)frameSize) * (float)frameSize;
            float localIntrplInv = 1.0f - localIntrpl;
            
            
            pr::geom::TNBFrame& iframe   = mFrames[index];
            pr::geom::TNBFrame& iframe1  = mFrames[index1];
            
            std::cout << iframe.tangent.dot(ci::Vec3f::yAxis()) << std::endl;
            

            *frame = iframe * localIntrplInv + iframe1 * localIntrpl;
        }
        
        
        /*----------------------------------------------------------------------------------*/
        
        
        const ci::Vec3f& Spline::getPoint(uint index) const{
            return mPoints[index];
        }
        
        void Spline::getPoint(uint index, ci::Vec3f* point){
            *point = mPoints[index];
        }
        
        void Spline::setPoints(const std::vector<ci::Vec3f> &points){
            mPoints = points;
            mVertices.resize((mPoints.size() - 1) * (mDetail - 1) + 1);
            mFrames.resize(mVertices.size());
        }
        
        void Spline::setPoint(uint index, const ci::Vec3f &point){
            mPoints[index].set(point);
        }
        
        void Spline::setPoint(uint index, float x, float y, float z){
            mPoints[index].set(x, y, z);
        }
        
        void Spline::setDetail(uint detail){
            mDetail = detail;
        }
        
        /*----------------------------------------------------------------------------------*/
        
        void Spline::setTension(float tension){
            mTension = tension;
        }
        
        void Spline::setBias(float bias){
            mBias = bias;
        }
        
        /*----------------------------------------------------------------------------------*/
        
        size_t Spline::getNumPoints(){
            return mPoints.size();
        }
        
        size_t Spline::getNumVertices(){
            return mVertices.size();
        }
        
        size_t Spline::getNumFrames(){
            return mFrames.size();
        }
        
        /*----------------------------------------------------------------------------------*/

        
        const std::vector<ci::Vec3f>& Spline::getPoints() const{
            return mPoints;
        }
        
        const std::vector<ci::Vec3f>& Spline::getVertices() const{
            return mVertices;
        }
        
        const std::vector<pr::geom::TNBFrame>& Spline::getFrames() const{
            return mFrames;
        }

        std::vector<ci::Vec3f>& Spline::getPoints(){
            return mPoints;
        }
        
        std::vector<ci::Vec3f>& Spline::getVertices(){
            return mVertices;
        }
        
        std::vector<pr::geom::TNBFrame>& Spline::getFrames(){
            return mFrames;
        }

    }
}
