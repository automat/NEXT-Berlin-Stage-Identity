//
//  GLHelper.h
//  Room_01
//
//  Created by Henryk Wollik on 10/02/14.
//
//

#ifndef Room_01_GLHelper_h
#define Room_01_GLHelper_h

#include "cinder/Vector.h"
#include <OpenGL/OpenGL.h>

using namespace ci;

namespace GLHelper {
    
    inline void DrawPlane(const Vec3f& pos, const Vec2f& size){
        float x = pos.x, y = pos.y, z = pos.z;
        float sizeX_2 = size.x * 0.5f;
        float sizeY_2 = size.y * 0.5f;
        GLfloat vertices[12] = {
            x-sizeX_2,y,z-sizeY_2,
            x-sizeX_2,y,z+sizeY_2,
            x+sizeX_2,y,z-sizeY_2,
            x+sizeX_2,y,z+sizeY_2,
        };
        
        static const GLfloat texcoords[8] = {
            0,0, 1,0, 0,1, 1,1
        };
        
        static const GLfloat normals[12] = {
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0
        };
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glNormalPointer(GL_FLOAT, 0, normals);
        glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    
    }
    
    /*--------------------------------------------------------------------------------------------------------------------*/
    // Material
    /*--------------------------------------------------------------------------------------------------------------------*/
    
    inline void SetMaterial(GLenum face,
                            const GLfloat* emission,
                            const GLfloat* ambient,
                            const GLfloat* diffuse,
                            const GLfloat* specular,
                            const GLfloat& shininess){
        glMaterialfv(face, GL_EMISSION, emission);
        glMaterialfv(face, GL_AMBIENT,  ambient);
        glMaterialfv(face, GL_DIFFUSE,  diffuse);
        glMaterialfv(face, GL_SPECULAR, specular);
        glMaterialf( face, GL_SHININESS,shininess);
    }
    
    
    /*--------------------------------------------------------------------------------------------------------------------*/
    // Light
    /*--------------------------------------------------------------------------------------------------------------------*/
    
    inline void SetLight(GLenum light,
                         const GLfloat* position,
                         const GLfloat* ambient,
                         const GLfloat* diffuse,
                         const GLfloat* specular,
                         const GLfloat& constAttentuation,
                         const GLfloat& linAttentuation,
                         const GLfloat& quadAttentuation){
        glLightfv(light, GL_POSITION,position);
        glLightfv(light, GL_AMBIENT, ambient);
        glLightfv(light, GL_DIFFUSE, diffuse);
        glLightfv(light, GL_SPECULAR, specular);
        glLightf( light, GL_CONSTANT_ATTENUATION, constAttentuation);
        glLightf( light, GL_LINEAR_ATTENUATION, linAttentuation);
        glLightf( light, GL_QUADRATIC_ATTENUATION, quadAttentuation);
    }
    
    /*--------------------------------------------------------------------------------------------------------------------*/
    // Color
    /*--------------------------------------------------------------------------------------------------------------------*/
    
    inline void SetColor(GLfloat k, GLfloat* color){
        color[0] = color[1] = color[2] = k;
        color[3] = 1.0;
    }
    
    inline void SetColor(GLfloat k, GLfloat a, GLfloat* color){
        color[0] = color[1] = color[2] = k;
        color[3] = a;
    }
    
    inline void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat* color){
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = 1.0;
    }
    
    inline void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat* color){
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
    }
    
    inline void SetColorR(GLfloat r, GLfloat* color){
        color[0] = r;
    }
    
    inline void SetColorG(GLfloat g, GLfloat* color){
        color[1] = g;
    }
    
    inline void SetColorB(GLfloat b, GLfloat* color){
        color[2] = b;
    }
    
    inline void SetColorA(GLfloat a, GLfloat* color){
        color[3] = a;
    }
    
    inline void SetColorRGB(GLfloat r, GLfloat g, GLfloat b, GLfloat* color){
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }
    
    
    
}



#endif
