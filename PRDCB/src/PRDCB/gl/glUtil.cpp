//
//  glUtil.cpp
//  PRDCB
//
//  Created by Henryk Wollik on 18/11/13.
//
//

#include "glUtil.h"
#include <OpenGL/gl.h>
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Matrix44.h"
#include "PRDCB/math/PRMath.h"

namespace pr {
    namespace gl{
        void color1f(float k){
            glColor3f(k,k,k);
        }
        
        void color2f(float k, float a){
            glColor4f(k,k,k,a);
        }
        
        void color3f(float r, float g, float b){
            glColor3f(r,g,b);
        }
        
        void color4f(float r, float g, float b, float a){
            glColor4f(r,g,b,a);
        }
        
        void rotatef(float angle, float x, float y, float z){
            glRotatef(math::degrees(angle),x,y,z);
        }
        
        void rotatef(float anglex, float angley, float anglez){
            glRotatef(math::degrees(anglex), 1.0f, 0, 0);
            glRotatef(math::degrees(angley), 0, 1.0f, 0);
            glRotatef(math::degrees(anglez), 0, 0, 1.0f);
        }
        
        void drawAxes(float scale){
            glColor3f(1.0f,0.0f,0.0f);
            ci::gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() * scale,0.05f,0.025f );
            glColor3f(0, 1.0f, 0);
            ci::gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() * scale,0.05f,0.025f);
            glColor3f(0, 0, 1.0f);
            ci::gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() * scale,0.05f,0.025f);
        }
        
        
        
        void drawGrid(uint size, float unit){
            uint sizeVertices = (size + 1) * 12;
            GLfloat vertices[sizeVertices];
            
            int i = 0;
            float ui;
            float sh = (float)size * 0.5f;
            
            while(i < sizeVertices){
                ui = i / 12;
                vertices[i   ] = vertices[i+8 ] = -sh;
                vertices[i+1 ] = vertices[i+4 ] = vertices[i+7 ] = vertices[i+10] = 0;
                vertices[i+2 ] = vertices[i+5 ] = vertices[i+6 ] = vertices[i+9 ] =-sh + ui;
                vertices[i+3 ] = vertices[i+11] = sh;
                
                i += 12;
            }
            
            glPushMatrix();
            glScalef(unit,unit,unit);
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_LINES, 0, sizeVertices / 3);
            glDisableClientState(GL_VERTEX_ARRAY);
            
            glPopMatrix();
        }
        
        void drawGridCube(uint size, float unit){
            uint sizeVertices = (size + 1) * 12;
            GLfloat vertices[sizeVertices];
            
            int i = 0;
            float ui;
            float sh = (float)size * 0.5f * unit;
            
            while(i < sizeVertices){
                ui = i / 12 * unit;
                
                vertices[i   ] = vertices[i+8 ] = -sh ;
                vertices[i+1 ] = vertices[i+4 ] = vertices[i+7 ] = vertices[i+10] = 0;
                vertices[i+2 ] = vertices[i+5 ] = vertices[i+6 ] = vertices[i+9 ] =-sh + ui;
                vertices[i+3 ] = vertices[i+11] = sh ;
                
                i += 12;
            }
            
            uint sizeArr = sizeVertices / 3;
            
            glPushMatrix();
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, vertices);
            
            glPushMatrix();
            glTranslatef(0,-sh,0);
            glDrawArrays(GL_LINES, 0, sizeArr);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0, sh,0);
            glDrawArrays(GL_LINES, 0, sizeArr);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(-sh, 0, 0);
            rotatef(0.0f, 0.0f, P_PI2);
            glDrawArrays(GL_LINES, 0, sizeArr);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(sh, 0, 0);
            rotatef(0.0f, 0.0f, P_PI2);
            glDrawArrays(GL_LINES, 0, sizeArr);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0, 0, -sh);
            rotatef(0.0f, P_PI2, P_PI2);
            glDrawArrays(GL_LINES, 0, sizeArr);
            glPopMatrix();
            
            glPushMatrix();
            glTranslatef(0, 0, sh);
            rotatef(0.0f, P_PI2, P_PI2);
            glDrawArrays(GL_LINES, 0, sizeArr);
            glPopMatrix();
            
            glDisableClientState(GL_VERTEX_ARRAY);
            
            glPopMatrix();
        }
        
        
        void drawArrays(GLenum mode,
                        const std::vector<ci::Vec3f>* vertices,
                        const std::vector<ci::Vec3f>* normals,
                        const std::vector<ci::ColorAf>* colors,
                        const std::vector<ci::Vec2f>* texCoords,
                        int sizeVertices,
                        int sizeColors,
                        int sizeTexCoords,
                        uint   offset,
                        GLsizei length){
            
            if (vertices->size() == 0)return;
            
            bool dmnt = mode != GL_POINTS &&
                        mode != GL_LINES &&
                        mode != GL_LINE_STRIP &&
                        mode != GL_LINE_LOOP;
            
            bool na = !dmnt && normals != 0,
                 ca = colors  != 0,
                 ta = !dmnt && texCoords != 0;
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(sizeVertices, GL_FLOAT, 0, &((*vertices)[0].x));
            
            if(na){
                glEnableClientState(GL_NORMAL_ARRAY);
                glNormalPointer(GL_FLOAT, 0, &((*normals)[0].x));
            }
            
            if (ca) {
                glEnableClientState(GL_COLOR_ARRAY);
                glColorPointer(sizeColors, GL_FLOAT, 0, &((*colors)[0].r));
            }
            
            if(ta){
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glTexCoordPointer(sizeTexCoords, GL_FLOAT, 0, &((*texCoords)[0].x));
            }
            
            glDrawArrays(mode, offset, length);
            
            if(na)glDisableClientState(GL_NORMAL_ARRAY);
            if(ca)glDisableClientState(GL_COLOR_ARRAY);
            if(ta)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            
            glDisableClientState(GL_VERTEX_ARRAY);
        }
        
        void drawArrays(GLenum mode,
                        const std::vector<ci::Vec3f>* vertices,
                        const std::vector<ci::Vec3f>* normals,
                        const std::vector<ci::ColorAf>* colors,
                        const std::vector<ci::Vec2f>* texCoords,
                        uint offset,
                        GLsizei length){
            drawArrays(mode,
                       vertices,
                       normals,
                       colors,
                       texCoords,
                       3,4,2,
                       offset,length);
        }
        
        void drawArrays(GLenum mode,
                         const std::vector<ci::Vec3f>* vertices,
                         const std::vector<ci::Vec3f>* normals,
                         const std::vector<ci::ColorAf>* colors,
                         const std::vector<ci::Vec2f>* texCoords){
            drawArrays(mode,
                       vertices,
                       normals,
                       colors,
                       texCoords,
                       3,4,2,
                       0,(GLint)vertices->size());
        }
        
        void drawTNBFrames(const std::vector<ci::Vec3f>& vertices,
                           const std::vector<pr::geom::TNBFrame>& frames,
                           float scale){

            int offset = 6 + 8; //axes vertices + double rect vertices + pointer;
            int num = (int)vertices.size() * offset;
            
            ci::Vec3f* verts   = new ci::Vec3f[num];
            ci::Colorf* colors = new ci::Colorf[num];
            
            
            float pmin = -scale,
                  pmax = pmin * -1;
            
            ci::Matrix44f matrix;
            ci::Vec3f p0,p1,p2,p3;
            
            int index;
            int i,j;
            i = -1;
            while (++i < vertices.size()) {
                index = i*offset;
                
                const ci::Vec3f& vertex         = vertices[i];
                const pr::geom::TNBFrame& frame = frames[i];
                
                colors[index].set(1, 0, 0);
                colors[index+1].set(colors[index]);
                
                verts[index  ].set(vertex);
                verts[index+1].set(vertex + frame.tangent * scale);
                
                colors[index+2].set(0, 1, 0);
                colors[index+3].set(colors[index+2]);
                
                verts[index+2].set(vertex);
                verts[index+3].set(vertex + frame.normal * scale);
                
                colors[index+4].set(0, 0, 1);
                colors[index+5].set(colors[index+4]);
                
                verts[index+4].set(vertex);
                verts[index+5].set(vertex + frame.binormal * scale);
                
                //set rect colors
                
                colors[index+ 6].set(0.35f,0.35f,0.35f);
                j = -1;
                while (++j < 7) {
                    colors[index + 7 + j].set(colors[index + 6]);
                }
                
                //transform points
                
                frame.getRotationMatrix(&matrix);
                matrix.setTranslate(vertex);
                
                p0.set(0.0f, pmin, pmin);
                p1.set(0.0f, pmin, pmax);
                p2.set(0.0f, pmax, pmax);
                p3.set(0.0f, pmax, pmin);
                
                p0.set(matrix.transformPoint(p0));
                p1.set(matrix.transformPoint(p1));
                p2.set(matrix.transformPoint(p2));
                p3.set(matrix.transformPoint(p3));
                
                verts[index+ 6].set(p0);
                verts[index+ 7].set(p1);
                
                verts[index+ 8].set(p1);
                verts[index+ 9].set(p2);
                
                verts[index+10].set(p2);
                verts[index+11].set(p3);
                
                verts[index+12].set(p3);
                verts[index+13].set(p0);
            }
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3, GL_FLOAT,  0, &colors[0].r);
            glVertexPointer(3, GL_FLOAT, 0, &verts[0].x);
            glDrawArrays(GL_LINES, 0, num);
            glDisableClientState(GL_COLOR_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
            glDrawArrays(GL_POINTS, 0, (GLint)vertices.size());
            
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
            
            delete verts;
            delete colors;
        }
        
        void drawTNBFrame(const ci::Vec3f& vertex,
                          const pr::geom::TNBFrame& frame,
                          float scale){
            const int num = 14;
            ci::Vec3f  verts[num];
            ci::Colorf colors[num];
            
            float pmin = -scale,
                  pmax = pmin * -1;
            ci::Matrix44f matrix;
            ci::Vec3f p0,p1,p2,p3;
            
            colors[0].set(1,0,0);
            colors[1].set(colors[0]);
            verts[0].set(vertex);
            verts[1].set(vertex + frame.tangent * scale);
            
            colors[2].set(0,1,0);
            colors[3].set(colors[2]);
            verts[2].set(vertex);
            verts[3].set(vertex + frame.normal * scale);
            
            colors[4].set(0,0,1);
            colors[5].set(colors[4]);
            verts[4].set(vertex);
            verts[5].set(vertex + frame.binormal * scale);
            
            colors[6].set(0.35f, 0.35f, 0.35f);
            int j = -1;
            while (++j < 7) {
                colors[7 + j].set(colors[6]);
            }
            
            frame.getRotationMatrix(&matrix);
            matrix.setTranslate(vertex);
            
            p0.set(0.0f, pmin, pmin);
            p1.set(0.0f, pmin, pmax);
            p2.set(0.0f, pmax, pmax);
            p3.set(0.0f, pmax, pmin);
            
            p0.set(matrix.transformPoint(p0));
            p1.set(matrix.transformPoint(p1));
            p2.set(matrix.transformPoint(p2));
            p3.set(matrix.transformPoint(p3));
            
            verts[6].set(p0);
            verts[7].set(p1);
            
            verts[8].set(p1);
            verts[9].set(p2);
            
            verts[10].set(p2);
            verts[11].set(p3);
            
            verts[12].set(p3);
            verts[13].set(p0);
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3, GL_FLOAT,  0, &colors[0].r);
            glVertexPointer(3, GL_FLOAT, 0, &verts[0].x);
            glDrawArrays(GL_LINES, 0, num);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
            
            
        }

        
    }
}
