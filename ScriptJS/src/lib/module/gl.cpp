//
//  gl.cpp
//  ScriptJS
//
//  Created by Henryk Wollik on 10/02/14.
//
//

#include "gl.h"
#include "ScriptJS.h"
#include <OpenGL/gl.h>
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rect.h"

#include <string>

using namespace std;
using namespace ci;

class GLImpl{
public:
    
    void ClearColor (float r, float g, float b, float a) {glClearColor(r, g, b, a);}
    void Clear () {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}
    void Colorf (float r, float g, float b, float a) {glColor4f(r, g, b, a);}
    void Disable (int cap) { glDisable(cap);};
    void DisableClientState (int array) {glDisableClientState(array);}
    void DrawArrays (int mode, int first, int count) {glDrawArrays(mode, first, count);}
    void DrawElements (int mode, int count, int type, const float* indices) {glDrawElements(mode, count, type, indices);}
    void Enable (int cap) {glEnable(cap);}
    void EnableClientState (int array) {glEnableClientState(array);}
    void LightModelf (int pname, float param) {glLightModelf(pname, param);}
    void LightModelfv (int pname, const float* params) {glLightModelfv(pname, params);}
    void Lightf (int light, int pname, float param) {glLightf(light, pname, param);}
    void Lightfv(int light, int pname, const float* params) {glLightfv(light, pname, params);}
    void LineWidth (float width) {glLineWidth(width);}
    void LoadIdentity () {glLoadIdentity();}
    void LoadMatrixf (const float* m) {glLoadMatrixf(m);}
    void Materialf (int face, int pname, float param) {glMaterialf(face, pname, param);}
    void Materialfv (int face, int pname, const float* params) {glMaterialfv(face, pname, params);}
    void MatrixMode (int mode) {glMatrixMode(mode);}
    void NormalPointer (int type, int stride, const float* pointer) {glNormalPointer(type, stride, pointer);}
    // void Ortho
    void PointSize (float size) {glPointSize(size);}
    void PopMatrix () {glPopMatrix();}
    void PushMatrix () {glPushMatrix();}
    void Rotatef(float x, float y, float z) {
        static const float PI_180 = 180.0f / M_1_PI;
        glRotatef(x * PI_180, 1, 0, 0); glRotatef(y * PI_180, 0, 1, 0); glRotatef(z * PI_180, 0, 0, 1);
    }
    void Scalef (float x, float y, float z) {glScalef(x, y, z);}
    void TexCoordPointer (int size, int type, int stride, const float* pointer) {glTexCoordPointer(size, type, stride, pointer);}
    void Translatef (float x, float y, float z) {glTranslatef(x, y, z);}
    void VertexPointer (int size, int type, int stride, const float* pointer) {glVertexPointer(size, type, stride, pointer);}
    
    void Uniform1fv (int location, int count, const float* value) {glUniform1fv(location, count, value);}
    void Uniform2fv (int location, int count, const float* value) {glUniform2fv(location, count, value);}
    void Uniform3fv (int location, int count, const float* value) {glUniform3fv(location, count, value);}
    void Uniform4fv (int location, int count, const float* value) {glUniform4fv(location, count, value);}
    
    
    void Uniform1iv (int location, int count, const int* value) {glUniform1iv(location, count, value);}
    void Uniform2iv (int location, int count, const int* value) {glUniform2iv(location, count, value);}
    void Uniform3iv (int location, int count, const int* value) {glUniform3iv(location, count, value);}
    void Uniform4iv (int location, int count, const int* value) {glUniform4iv(location, count, value);}
    
    void UniformMatrix2fv (int location, int count, bool transpose, const float *value) {glUniformMatrix2fv(location, count, transpose, value);};
    void UniformMatrix3fv (int location, int count, bool transpose, const float *value) {glUniformMatrix3fv(location, count, transpose, value);};
    void UniformMatrix4fv (int location, int count, bool transpose, const float *value) {glUniformMatrix4fv(location, count, transpose, value);};

    void Viewport(int x, int y, int width, int height){
        glViewport(x, y, width, height);
    }
    
    
    
    /*----------------------------------------------------------------------------------------------------------------------------*/
    //  Cinder convience
    /*----------------------------------------------------------------------------------------------------------------------------*/
    
    void DrawCoordinateFrame(float axisLength = 1.0f, float headLength = 0.2f, float headRadius = 0.05f){
        gl::drawCoordinateFrame(axisLength,headLength,headRadius);
    }
    
    void DrawLine(float x0, float y0, float z0, float x1, float y1, float z1){
        static Vec3f start;
        static Vec3f end;
        start.set(x0, y0, z0);
        end.set(x1, y1, z1);
        gl::drawLine(start, end);
    }
    
    void DrawCube(float x, float y, float z, float sx, float sy, float sz){
        static Vec3f center;
        static Vec3f size;
        center.set(x, y, z);
        size.set(sx, sy, sz);
        gl::drawCube(center, size);
    }
    
    void DrawColorCube(float x, float y, float z, float sx, float sy, float sz){
        static Vec3f center;
        static Vec3f size;
        center.set(x, y, z);
        size.set(sx, sy, sz);
        gl::drawColorCube(center, size);
    }
    
    void DrawStrokedCube(float x, float y, float z, float sx, float sy, float sz){
        static Vec3f center;
        static Vec3f size;
        center.set(x, y, z);
        size.set(sx, sy, sz);
        gl::drawStrokedCube(center, size);
    }
    
    void DrawSphere(float x, float y, float z, float radius, int segments = 12){
        static Vec3f center;
        center.set(x, y, z);
        gl::drawSphere(center, radius, segments);
    }
    
    void DrawSolidCircle(float x, float y, float z, float radius, int numSegments = 0){
        glPushMatrix();
        glTranslatef(x, y, z);
        gl::drawSolidCircle(Vec2f::zero(), radius, numSegments);
        glPopMatrix();
    }
    
    void DrawStrokedCircle(float x, float y, float z, float radius, int numSegments = 0){
        static const Vec2f zero(0,0);
        glPushMatrix();
        glTranslatef(x, y, z);
        gl::drawSolidCircle(zero, radius, numSegments);
        glPopMatrix();
    }
    
    void DrawSolidEllipse(float x, float y, float z, float radiusX, float radiusY, int numSegments = 0){
        static const Vec2f zero(0,0);
        glPushMatrix();
        glTranslatef(x, y, z);
        gl::drawSolidEllipse(zero, radiusX, radiusY, numSegments);
        glPopMatrix();
    }
    
    void DrawStrokedEllipse(float x, float y, float z, float radiusX, float radiusY, int numSegments = 0){
        static const Vec2f zero(0,0);
        glPushMatrix();
        glTranslatef(x, y, z);
        gl::drawStrokedEllipse(zero, radiusX, radiusY, numSegments);
        glPopMatrix();
    }
    
    void DrawSolidRect(float x, float y, float z, float width, float height){
        static Rectf rect;
        rect.set(0,0,width,height);
        glPushMatrix();
        glTranslatef(x,y,z);
        gl::drawSolidRect(rect);
        glPopMatrix();
    }
    
    void DrawStrokedRect(float x, float y, float z, float width, float height){
        static Rectf rect;
        rect.set(0,0,width,height);
        glPushMatrix();
        glTranslatef(x,y,z);
        gl::drawStrokedRect(rect);
        glPopMatrix();
    }
    
    void DrawVector(float x0, float y0, float z0, float x1, float y1, float z1, float headLength = 0.2f, float headRadius = 0.05f){
        static Vec3f start;
        static Vec3f end;
        start.set(x0, y0, z0);
        end.set(x1, y1, z1);
        gl::drawVector(start, end, headLength, headRadius);
    }
    
    
    
    /*----------------------------------------------------------------------------------------------------------------------------*/
    //  Helper
    /*----------------------------------------------------------------------------------------------------------------------------*/
    
    void DrawPlane(float x, float y, float z,
                          float sx, float sy){
        float sizeX_2 = sx * 0.5f;
        float sizeY_2 = sy * 0.5f;
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

    
    
    
    
};


struct GLConstant{
    const char* name;
    uint32_t    value;
};

struct GLFunction{
    const char* name;
    void (*func)(const FunctionCallbackInfo<Value>& args);
};

GLConstant glconstants[] = {
    /* AccumOp */
    {"GL_ACCUM", GL_ACCUM},
    {"GL_LOAD", GL_LOAD},
    {"GL_RETURN", GL_RETURN},
    {"GL_MULT", GL_MULT},
    {"GL_ADD", GL_ADD},
    /* AccumOp */
    {"GL_NEVER",GL_NEVER},
    {"GL_LESS",GL_LESS},
    {"GL_EQUAL", GL_EQUAL},
    {"GL_LEQUAL", GL_LEQUAL},
    {"GL_GREATER", GL_GREATER},
    {"GL_NOTEQUAL", GL_NOTEQUAL},
    {"GL_GEQUAL", GL_GEQUAL},
    {"GL_ALWAYS", GL_ALWAYS},
    /* AttribMask */
    {"GL_CURRENT_BIT", GL_CURRENT_BIT},
    {"GL_POINT_BIT", GL_POINT_BIT},
    
};

class GLWrap : public scriptjs::ObjectWrap{
    static Persistent<Function> constructor;
    
    GLImpl mImpl;
    
    static GLImpl& Impl(const FunctionCallbackInfo<Value>& args){
        GLWrap* wrap = ObjectWrap::Unwrap<GLWrap>(args.This());
        return wrap->mImpl;
    }
    
    /*----------------------------------------------------------------------------------------------------------------------------*/
    //  OpenGL
    /*----------------------------------------------------------------------------------------------------------------------------*/
    
    static void ClearColor(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc == 0){
            
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        float r,g,b,k,a;
        switch (argc) {
            case 1:
                k = args[0]->NumberValue();
                glClearColor(k, k, k, 1.0f);
                //Impl(args).ClearColor(k, k, k, 1.0f);
            break;
            case 2:
                k = args[0]->NumberValue();
                a = args[1]->NumberValue();
                glClearColor(k, k, k, a);
                Impl(args).ClearColor(k, k, k, a);
            case 3:
                r = args[0]->NumberValue();
                g = args[1]->NumberValue();
                b = args[2]->NumberValue();
                glClearColor(r, g, b, 1.0f);
                Impl(args).ClearColor(r, g, b, 1.0f);
            break;
            case 4:
                r = args[0]->NumberValue();
                g = args[1]->NumberValue();
                b = args[2]->NumberValue();
                a = args[3]->NumberValue();
                Impl(args).ClearColor(r, g, b, a);
            break;
        }
        
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void Clear(const FunctionCallbackInfo<Value>& args){
        Impl(args).Clear();
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void Colorf(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc == 0 || argc> 4){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        float r,g,b,k,a;
        switch (argc) {
            case 1:
            k = args[0]->NumberValue();
            Impl(args).Colorf(k, k, k, 1.0f);
            break;
            case 2:
            k = args[0]->NumberValue();
            a = args[1]->NumberValue();
            Impl(args).Colorf(k, k, k, a);
            case 3:
            r = args[0]->NumberValue();
            g = args[1]->NumberValue();
            b = args[2]->NumberValue();
            Impl(args).Colorf(r, g, b, 1.0f);
            break;
            case 4:
            r = args[0]->NumberValue();
            g = args[1]->NumberValue();
            b = args[2]->NumberValue();
            a = args[3]->NumberValue();
            Impl(args).Colorf(r, g, b, a);
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    /*
    static void Disable(const FunctionCallbackInfo<Value>& args){
        if (args.Length() != 1) {
            GLWRAP_RETURN_UNDEFINED;
        }
        Impl(args).Disable(args[0]->Int32Value());
        GLWRAP_RETURN_UNDEFINED;
    }
    
    static void DisableClientState(const FunctionCallbackInfo<Value>& args){
        if (args.Length() != 1) {
            GLWRAP_RETURN_UNDEFINED;
        }
        Impl(args).Disable(args[0]->Int32Value());
        GLWRAP_RETURN_UNDEFINED;
    }
    
    static void DrawArrays(const FunctionCallbackInfo<Value>& args){
        if(args.Length() != 3){
            GLWRAP_RETURN_UNDEFINED;
        }
        Impl(args).DrawArrays(args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value());
    }
    
    static void DrawElements(const FunctionCallbackInfo<Value>& args){
        if(args.Length() != 4){
            GLWRAP_RETURN_UNDEFINED;
        }
     
        Impl(args).DrawElements(args[0]->Int32Value(),
                                args[1]->Int32Value(),
                                args[2]->Int32Value(),
                                args[3]->)
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
     */
   
    static void PopMatrix(const FunctionCallbackInfo<Value>& args){
        Impl(args).PopMatrix();
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void PushMatrix(const FunctionCallbackInfo<Value>& args){
        Impl(args).PushMatrix();
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void Rotatef(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc != 3){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        Impl(args).Rotatef(args[0]->NumberValue(),
                           args[1]->NumberValue(),
                           args[2]->NumberValue());
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void Scalef(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if (argc == 0) {
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        float s;
        switch (argc) {
            case 1:
                s = args[0]->NumberValue();
                Impl(args).Scalef(s, s, s);
            break;
            case 2:
                Impl(args).Scalef(args[0]->NumberValue(),
                                  args[1]->NumberValue(),
                                  1.0);
            break;
            case 3:
                Impl(args).Scalef(args[0]->NumberValue(),
                                  args[1]->NumberValue(),
                                  args[2]->NumberValue());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void Translatef(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if (argc == 0) {
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 1:
                Impl(args).Translatef(args[0]->NumberValue(), 0, 0);
            break;
            case 2:
                Impl(args).Translatef(args[0]->NumberValue(),
                                      args[1]->NumberValue(),
                                      0);
            break;
            case 3:
                Impl(args).Translatef(args[0]->NumberValue(),
                                      args[1]->NumberValue(),
                                      args[2]->NumberValue());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
   
    
    /*----------------------------------------------------------------------------------------------------------------------------*/
    // Cinder convinience
    /*----------------------------------------------------------------------------------------------------------------------------*/
    
    static void DrawCoordinateFrame(const FunctionCallbackInfo<Value>& args){
        switch (args.Length()) {
            case 0:
                Impl(args).DrawCoordinateFrame();
            break;
            case 1:
                Impl(args).DrawCoordinateFrame(args[0]->NumberValue());
            break;
            case 2:
                Impl(args).DrawCoordinateFrame(args[0]->NumberValue(),
                                               args[1]->NumberValue());
            break;
            case 3:
                Impl(args).DrawCoordinateFrame(args[0]->NumberValue(),
                                               args[1]->NumberValue(),
                                               args[2]->NumberValue());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawLine(const FunctionCallbackInfo<Value>& args){
        if (args.Length() != 6) {
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        Impl(args).DrawLine(args[0]->NumberValue(),
                            args[1]->NumberValue(),
                            args[2]->NumberValue(),
                            args[3]->NumberValue(),
                            args[4]->NumberValue(),
                            args[5]->NumberValue());
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawCube(const FunctionCallbackInfo<Value>& args){
        if(args.Length() != 6){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        Impl(args).DrawCube(args[0]->NumberValue(),
                            args[1]->NumberValue(),
                            args[2]->NumberValue(),
                            args[3]->NumberValue(),
                            args[4]->NumberValue(),
                            args[5]->NumberValue());
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawColorCube(const FunctionCallbackInfo<Value>& args){
        if(args.Length() != 6){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        Impl(args).DrawColorCube(args[0]->NumberValue(),
                                 args[1]->NumberValue(),
                                 args[2]->NumberValue(),
                                 args[3]->NumberValue(),
                                 args[4]->NumberValue(),
                                 args[5]->NumberValue());
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawStrokedCube(const FunctionCallbackInfo<Value>& args){
        if(args.Length() != 6){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        Impl(args).DrawStrokedCube(args[0]->NumberValue(),
                                   args[1]->NumberValue(),
                                   args[2]->NumberValue(),
                                   args[3]->NumberValue(),
                                   args[4]->NumberValue(),
                                   args[5]->NumberValue());
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawSphere(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc < 3 || argc > 5){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 3:
                Impl(args).DrawSphere(args[0]->NumberValue(),
                                      args[1]->NumberValue(),
                                      args[2]->NumberValue(),
                                      1.0f);
            break;
            case 4:
                Impl(args).DrawSphere(args[0]->NumberValue(),
                                      args[1]->NumberValue(),
                                      args[2]->NumberValue(),
                                      args[3]->NumberValue());
            break;
            case 5:
                Impl(args).DrawSphere(args[0]->NumberValue(),
                                      args[1]->NumberValue(),
                                      args[2]->NumberValue(),
                                      args[3]->NumberValue(),
                                      args[4]->Int32Value());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawSolidCircle(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc < 3 || argc > 5){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 3:
                Impl(args).DrawSolidCircle(args[0]->NumberValue(),
                                           args[1]->NumberValue(),
                                           args[2]->NumberValue(),
                                           0.5f);
            break;
            case 4:
                Impl(args).DrawSolidCircle(args[0]->NumberValue(),
                                           args[1]->NumberValue(),
                                           args[2]->NumberValue(),
                                           args[3]->NumberValue());
            break;
            case 5:
                Impl(args).DrawSolidCircle(args[0]->NumberValue(),
                                           args[1]->NumberValue(),
                                           args[2]->NumberValue(),
                                           args[3]->NumberValue(),
                                           args[4]->Int32Value());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawStrokedCircle(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc < 3 || argc > 5){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 3:
            Impl(args).DrawStrokedCircle(args[0]->NumberValue(),
                                         args[1]->NumberValue(),
                                         args[2]->NumberValue(),
                                         0.5f);
            break;
            case 4:
            Impl(args).DrawStrokedCircle(args[0]->NumberValue(),
                                         args[1]->NumberValue(),
                                         args[2]->NumberValue(),
                                         args[3]->NumberValue());
            break;
            case 5:
            Impl(args).DrawStrokedCircle(args[0]->NumberValue(),
                                         args[1]->NumberValue(),
                                         args[2]->NumberValue(),
                                         args[3]->NumberValue(),
                                         args[4]->Int32Value());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawSolidEllipse(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc < 3 || argc == 4 || argc > 6){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 3:
            Impl(args).DrawSolidEllipse(args[0]->NumberValue(),
                                        args[1]->NumberValue(),
                                        args[2]->NumberValue(),
                                        0.5f,
                                        0.5f);
            break;
            case 5:
            Impl(args).DrawSolidEllipse(args[0]->NumberValue(),
                                        args[1]->NumberValue(),
                                        args[2]->NumberValue(),
                                        args[3]->NumberValue(),
                                        args[4]->NumberValue());
            break;
            case 6:
            Impl(args).DrawSolidEllipse(args[0]->NumberValue(),
                                        args[1]->NumberValue(),
                                        args[2]->NumberValue(),
                                        args[3]->NumberValue(),
                                        args[4]->NumberValue(),
                                        args[5]->Int32Value());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawStrokedEllipse(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc < 3 || argc == 4 || argc > 6){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 3:
                Impl(args).DrawStrokedEllipse(args[0]->NumberValue(),
                                              args[1]->NumberValue(),
                                              args[2]->NumberValue(),
                                              0.5f,
                                              0.5f);
            break;
            case 5:
                Impl(args).DrawStrokedEllipse(args[0]->NumberValue(),
                                              args[1]->NumberValue(),
                                              args[2]->NumberValue(),
                                              args[3]->NumberValue(),
                                              args[4]->NumberValue());
            break;
            case 6:
                Impl(args).DrawStrokedEllipse(args[0]->NumberValue(),
                                              args[1]->NumberValue(),
                                              args[2]->NumberValue(),
                                              args[3]->NumberValue(),
                                              args[4]->NumberValue(),
                                              args[5]->Int32Value());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawSolidRect(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc < 3 || argc > 5){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 3:
                Impl(args).DrawSolidRect(args[0]->NumberValue(),
                                         args[1]->NumberValue(),
                                         args[2]->NumberValue(),
                                         1,
                                         1);
            break;
            case 4:
                Impl(args).DrawSolidRect(args[0]->NumberValue(),
                                         args[1]->NumberValue(),
                                         args[2]->NumberValue(),
                                         args[3]->NumberValue(),
                                         args[3]->NumberValue());
            break;
            case 5:
                Impl(args).DrawSolidRect(args[0]->NumberValue(),
                                         args[1]->NumberValue(),
                                         args[2]->NumberValue(),
                                         args[3]->NumberValue(),
                                         args[4]->NumberValue());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawStrokedRect(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc < 3 || argc > 5){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 3:
                Impl(args).DrawStrokedRect(args[0]->NumberValue(),
                                           args[1]->NumberValue(),
                                           args[2]->NumberValue(),
                                           1,
                                           1);
            break;
            case 4:
                Impl(args).DrawStrokedRect(args[0]->NumberValue(),
                                           args[1]->NumberValue(),
                                           args[2]->NumberValue(),
                                           args[3]->NumberValue(),
                                           args[3]->NumberValue());
            break;
            case 5:
                Impl(args).DrawStrokedRect(args[0]->NumberValue(),
                                           args[1]->NumberValue(),
                                           args[2]->NumberValue(),
                                           args[3]->NumberValue(),
                                           args[4]->NumberValue());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    static void DrawVector(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if(argc < 6 || argc > 8){
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 6:
                Impl(args).DrawVector(args[0]->NumberValue(),
                                      args[1]->NumberValue(),
                                      args[2]->NumberValue(),
                                      args[3]->NumberValue(),
                                      args[4]->NumberValue(),
                                      args[5]->NumberValue());
            break;
            case 7:
                Impl(args).DrawVector(args[0]->NumberValue(),
                                      args[1]->NumberValue(),
                                      args[2]->NumberValue(),
                                      args[3]->NumberValue(),
                                      args[4]->NumberValue(),
                                      args[5]->NumberValue(),
                                      args[6]->NumberValue());
            break;
            case 8:
                Impl(args).DrawVector(args[0]->NumberValue(),
                                      args[1]->NumberValue(),
                                      args[2]->NumberValue(),
                                      args[3]->NumberValue(),
                                      args[4]->NumberValue(),
                                      args[5]->NumberValue(),
                                      args[6]->NumberValue(),
                                      args[7]->NumberValue());
            break;
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    
    /*----------------------------------------------------------------------------------------------------------------------------*/
    // Helper
    /*----------------------------------------------------------------------------------------------------------------------------*/
    
    static void DrawPlane(const FunctionCallbackInfo<Value>& args){
        int argc = args.Length();
        if (argc != 0 || argc != 3 || argc != 5) {
            SCRIPTJS_MODULE_RETURN_UNDEFINED;
            return;
        }
        switch (argc) {
            case 0:
                Impl(args).DrawPlane(0,0,0,1,1);
            break;
            case 3:
                Impl(args).DrawPlane(args[0]->NumberValue(),
                                     args[1]->NumberValue(),
                                     args[2]->NumberValue(),
                                     1, 1);
            break;
            case 5:
                Impl(args).DrawPlane(args[0]->NumberValue(),
                                     args[1]->NumberValue(),
                                     args[2]->NumberValue(),
                                     args[3]->NumberValue(),
                                     args[4]->NumberValue());
        }
        SCRIPTJS_MODULE_RETURN_UNDEFINED;
    }
    

    
    
    
public:
    // create js constructor
    static void Init(Handle<Object> exports){
        Isolate* isolate = Isolate::GetCurrent();
        
        const char* className = "GL";
        
        //Prepare constructor template
        Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
        tpl->SetClassName(String::NewFromUtf8(isolate, className));
        tpl->InstanceTemplate()->SetInternalFieldCount(1);
        
        // OpenGL
        SET_PROTOTYPE_METHOD(tpl, "clearColor", ClearColor);
        SET_PROTOTYPE_METHOD(tpl, "clear", Clear);
        SET_PROTOTYPE_METHOD(tpl, "colorf", Colorf);
        SET_PROTOTYPE_METHOD(tpl, "popMatrix", PopMatrix);
        SET_PROTOTYPE_METHOD(tpl, "pushMatrix", PushMatrix);
        SET_PROTOTYPE_METHOD(tpl, "rotatef", Rotatef);
        SET_PROTOTYPE_METHOD(tpl, "scalef", Scalef);
        SET_PROTOTYPE_METHOD(tpl, "translatef", Translatef);
        
        // Cinder
        SET_PROTOTYPE_METHOD(tpl, "drawCoordinateFrame", DrawCoordinateFrame);
        SET_PROTOTYPE_METHOD(tpl, "drawLine", DrawLine);
        SET_PROTOTYPE_METHOD(tpl, "drawCube", DrawCube);
        SET_PROTOTYPE_METHOD(tpl, "drawColorCube", DrawColorCube);
        SET_PROTOTYPE_METHOD(tpl, "drawStrokedCube", DrawStrokedCube);
        SET_PROTOTYPE_METHOD(tpl, "drawSphere", DrawSphere);
        SET_PROTOTYPE_METHOD(tpl, "drawSolidCircle", DrawSolidCircle);
        SET_PROTOTYPE_METHOD(tpl, "drawStrokedCicle", DrawStrokedCircle);
        SET_PROTOTYPE_METHOD(tpl, "drawSolidEllipse", DrawSolidEllipse);
        SET_PROTOTYPE_METHOD(tpl, "drawStrokedEllipse", DrawStrokedEllipse);
        SET_PROTOTYPE_METHOD(tpl, "drawSolidRect", DrawSolidRect);
        SET_PROTOTYPE_METHOD(tpl, "drawStrokedRect", DrawStrokedRect);
        SET_PROTOTYPE_METHOD(tpl, "drawVector", DrawVector);
        
        //Helper
        SET_PROTOTYPE_METHOD(tpl, "drawPlane", DrawPlane);
        
        
     
        
        GLWrap::constructor.Reset(isolate,tpl->GetFunction());
        exports->Set(String::NewFromUtf8(isolate, className), tpl->GetFunction());
    }
    
    // create js constructor
    static void New(const FunctionCallbackInfo<Value>& args){
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope scope(isolate);
        
        if (args.IsConstructCall()) {
            // Invoked as constructor: `new MyObject(...)`
            GLWrap* obj = new GLWrap();
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            // Invoked as plain function `MyObject(...)`, turn into construct call.
            Local<Function> cons = Local<Function>::New(isolate, GLWrap::constructor);
            args.GetReturnValue().Set(cons->NewInstance());
        }
    }
    
};

//init static
Persistent<Function> GLWrap::constructor;

void GL::Initialize(v8::Handle<v8::Object> exports){
    GLWrap::Init(exports);
}

