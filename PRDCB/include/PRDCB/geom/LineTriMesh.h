//
//  Line3d.h
//  PRDCB
//
//  Created by Henryk Wollik on 19/11/13.
//
//

#ifndef PRDCB_Line3d_h
#define PRDCB_Line3d_h

#include "cinder/TriMesh.h"
#include "cinder/Vector.h"
#include <boost/function.hpp>
#include <vector>
#include <sys/types.h>

namespace pr {
    namespace geom{
        class LineTriMesh : public ci::TriMesh{
        private:
            std::vector<ci::Vec3f> mVerticesNormal;
            std::vector<ci::Vec3f> mPoints;
            size_t                 mNumSegments;
            bool                   mCloseCaps;
            float                  mRadius;
            
        public:

            /*---------------------------------------------------------------------------------------------------------*/
            
            LineTriMesh(const std::vector<ci::Vec3f>& points,
                        size_t numSegments,
                        float radius,
                        bool closeCaps);
            LineTriMesh();
            
            /*---------------------------------------------------------------------------------------------------------*/

            
            void setUnitRadius(float radius);
            void setRadius(size_t index, float radius);
            void setNumSegments(size_t num);
            void setCloseCaps(bool close);
            
            void setPoints(const std::vector<ci::Vec3f>& points);
            void setPoint(size_t index,float x, float y, float z);
            void setPoint(size_t index,const ci::Vec3f& point);
            
            void appendPoint(const ci::Vec3f& point);
            void appendPoint(float x, float y, float z);
            void appendPoints(const ci::Vec3f* points, size_t num);
            
            void applySliceFunc();
            void update();
            void clear();
            
            /*---------------------------------------------------------------------------------------------------------*/

            const std::vector<ci::Vec3f>& getPoints() const;
            void getPoint(size_t index, ci::Vec3f* point);
            std::vector<ci::Vec3f>& getPoints();
            
            
            size_t getNumPoints();
            size_t getNumSegments();
        };
    }
}



#endif
