//
//  Spline.h
//  PRDCB
//
//  Created by Henryk Wollik on 17/11/13.
//
//

#ifndef PRDCB_Spline_h
#define PRDCB_Spline_h

#include <stdio.h>
#include <sys/types.h>
#include <vector>
#include "cinder/Vector.h"
#include "PRDCB/geom/ITNBSequence.h"
#include "PRDCB/geom/TNBFrame.h"

//Parallel Transport Frames http://www.cs.indiana.edu/pub/techreports/TR425.pdf
namespace pr {
    namespace geom{
        class Spline : public ITNBSequence{
        private:
            uint  mDetail;
            float mTension;
            float mBias;
            bool  mVerticesUpdated;
            
            void getPointOnComp(const std::vector<ci::Vec3f>& comp, float intrpl, ci::Vec3f *point);
            
        protected:
            std::vector<ci::Vec3f> mPoints;
            std::vector<ci::Vec3f> mVertices;
            
        public:
            Spline();
            ~Spline();
            
            void setDetail(uint detail);
            void setTension(float tension);
            void setBias(float bias);
            
            void setPoints(const std::vector<ci::Vec3f>& points);
            void setPoint(uint index, const ci::Vec3f& point);
            void setPoint(uint index, float x, float y, float z);
            
            void getPointOnPoints(float intrpl, ci::Vec3f* point);
            void getPointOnSpline(float intrpl, ci::Vec3f* point);
            void getFrameOnSpline(float intrpl, pr::geom::TNBFrame* frame);
            
            void update();
            void recalcFrames();
            
            void clear();
            
            const ci::Vec3f& getPoint(uint index) const;
            void  getPoint(uint index, ci::Vec3f* point);
            
            size_t getNumPoints();
            size_t getNumVertices();
            size_t getNumFrames();
            
            const std::vector<ci::Vec3f>& getPoints() const;
            const std::vector<ci::Vec3f>& getVertices() const;
            const std::vector<pr::geom::TNBFrame>& getFrames() const;

            std::vector<ci::Vec3f>& getPoints();
            std::vector<ci::Vec3f>& getVertices();
            std::vector<pr::geom::TNBFrame>& getFrames();
        };
    }
}



#endif
