//
//  glUtil.h
//  PRDCB
//
//  Created by Henryk Wollik on 18/11/13.
//
//

#ifndef PRDCB_glUtil_h
#define PRDCB_glUtil_h

#include <sys/types.h>
#include <OpenGL/gl.h>
#include <vector>
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/TriMesh.h"
#include "PRDCB/geom/TNBFrame.h"

namespace pr {
    namespace gl{
        void color1f(float k);
        void color2f(float k, float a);
        void color3f(float r, float g, float b);
        void color4f(float r, float g, float b, float a);
        void rotatef(float angle, float x, float y, float z);
        void rotatef(float anglex, float angley, float anglez);
        void drawAxes(float scale = 1.0f);
        
        void drawGrid(uint size, float unit = 1.0f);
        void drawGridCube(uint size, float unit = 1.0f);
        
        void drawArrays(GLenum mode,
                        const std::vector<ci::Vec3f>* vertices,
                        const std::vector<ci::Vec3f>* normals,
                        const std::vector<ci::Color>* colors,
                        const std::vector<ci::Vec2f>* texCoords);
        
        void drawArrays(GLenum mode,
                        const std::vector<ci::Vec3f>* vertices,
                        const std::vector<ci::Vec3f>* normals,
                        const std::vector<ci::Color>* colors,
                        const std::vector<ci::Vec2f>* texCoords,
                        uint offset,
                        GLsizei length);
        
        void drawArrays(GLenum mode,
                        const std::vector<ci::Vec3f>* vertices,
                        const std::vector<ci::Vec3f>* normals,
                        const std::vector<ci::ColorAf>* colors,
                        const std::vector<ci::Vec2f>* texCoords,
                        int sizeVertices,
                        int sizeColors,
                        int sizeTexCoords,
                        uint offset,
                        GLsizei length);
        
        void drawTNBFrames(const std::vector<ci::Vec3f>& vertices,
                           const std::vector<pr::geom::TNBFrame>& frames,
                           float scale = 0.025f);
        
        void drawTNBFrame(const ci::Vec3f& vertex,
                          const pr::geom::TNBFrame& frame,
                          float scale = 0.025f);
    }
}


#endif
