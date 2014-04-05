//
//  VboObj.h
//  grid_01
//
//  Created by Henryk Wollik on 05/04/14.
//
//

#ifndef grid_01_VboObj_h
#define grid_01_VboObj_h

#include "cinder/gl/Vbo.h"
#include "cinder/Vector.h"
#include <vector>
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

using namespace ci;
using namespace std;
using namespace boost::assign;

namespace debug {
    
    class VboObj {
        gl::VboMesh mVboMesh;
        
        
    public:
        VboObj(){
            gl::VboMesh::Layout layout;
            layout.setStaticIndices();
            layout.setStaticColorsRGB();
            layout.setDynamicPositions();
            
            int numIndices  = 18;
            int numVertices = 8;
            
            mVboMesh = gl::VboMesh(numVertices, numIndices, layout, GL_TRIANGLES);
            
            
            vector<uint32_t> indices;
            indices += 0,1,2;
            indices += 1,2,3;
            
            indices += 4,5,6;
            indices += 5,6,7;
            
            indices += 0,1,4;
            indices += 4,5,2;
            
            vector<Colorf> colors;
            colors += Colorf(1,0,0),Colorf(0,1,0),Colorf(0,0,1),Colorf(0,0,0);
            colors += Colorf(1,0,0),Colorf(0,1,0),Colorf(0,0,1),Colorf(0,0,0);
            
            mVboMesh.bufferIndices(indices);
            mVboMesh.bufferColorsRGB(colors);
            
            
            
        
        }
        
        inline void draw(){
            float size_2 = 0.5f;
            
            gl::VboMesh::VertexIter vbItr = mVboMesh.mapVertexBuffer();
            vbItr.setPosition(-size_2,-size_2,-size_2);++vbItr;
            vbItr.setPosition( size_2,-size_2,-size_2);++vbItr;
            vbItr.setPosition(-size_2,-size_2, size_2);++vbItr;
            vbItr.setPosition( size_2,-size_2, size_2);++vbItr;
            
            vbItr.setPosition(-size_2, size_2,-size_2);++vbItr;
            vbItr.setPosition( size_2, size_2,-size_2);++vbItr;
            vbItr.setPosition(-size_2, size_2, size_2);++vbItr;
            vbItr.setPosition( size_2, size_2, size_2);++vbItr;
            
            gl::draw(mVboMesh);
        }
        
        inline void update(){
            
            
            
        }
    };

    
}



#endif
