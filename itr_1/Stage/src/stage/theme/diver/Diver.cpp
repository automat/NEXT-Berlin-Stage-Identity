#include "stage/theme/diver/Diver.h"

#include "Resources.h"
#include "cinder/Matrix44.h"
#include "cinder/Utilities.h"
#include "cinder/gl/TextureFont.h"

#include "util/text/SharedTextureFont.h"

namespace next{
    using namespace ci;

    Diver::Diver(PathSlice* pathSlice,
            int        numPoints,
            float      offset,
            float      speed,
            float      length,
            float      height){
        reset(pathSlice,
                numPoints,
                offset,
                speed,
                length,
                height);
    }

    void Diver::reset(PathSlice* pathSlice,
            int        numPoints,
            float      offset,
            float      speed,
            float      length,
            float      height){
        mPathSlice = pathSlice;
        mNumPoints = numPoints;
        mOffset    = mOffsetInitial = offset;
        mSpeed     = speed;
        mWidth     = pathSlice->getWidth();
        mLength    = length;
        mHeight    = height;
        mIsHidden  = false;

        mPathSurfaceSize = mPathSlice->getSurfaceSize();
        mPathLength      = mPathSurfaceSize;
        mPathLengthInv   = 1.0f / mPathLength;

        mIsOut = mIsOutPrev = false;
        mPoints.resize(mNumPoints);
        mTexcoords.resize(mNumPoints);

        mLengthStep = float(mLength) / float(mNumPoints-1);
    }

    void Diver::update(){
        if (mIsOut && mIsHidden) {
            return;
        }

        //  -1-0 : complete length outside before
        //   0-1 : start to end
        //   1-2 : complete length outside after
        
        if(mOffset >= 2.0f){
            mOffset = -1;
        }

        int i = -1;
        for(vector<Vec3f>::iterator itr = mPoints.begin(); itr != mPoints.end(); itr++){
            mPathSlice->getPointOn(mOffset - mLengthStep * float(++i), &(*itr));
        }
    }
    
    void Diver::setOffset(float offset){
        mOffset = offset;
    }
    
    void Diver::increaseOffset(float value){
        mOffset += value;
    }

    void Diver::updateTexcoords(){
        mTexcoords[0] = MIN(mLength,mOffset);
        int i = 0;
        while(++i < mTexcoords.size()){
            mTexcoords[i] = MAX(0,mTexcoords[i-1] - (mPoints[i-1].distance(mPoints[i]) / mPathLength)) ;
        }
    }

    void Diver::updateInOut(){
        mIsOutPrev = mIsOut;
        mIsOut = (mOffset >= (1.0f + mLength)) || (mOffset <= 0.0f);
    }

    void Diver::debugDraw(){
        if (isOut()){
            return;
        }

        float prevPointSize;
        glGetFloatv(GL_CURRENT_POINT_SIZE_APPLE, &prevPointSize);

        glPointSize(3);
        glColor4f(1, 1, 1, 1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
        glDrawArrays(GL_POINTS, 0, mPoints.size());

        Vec3f line[4];
        float pathWidth = mWidth * 0.5f;
        float height    = mHeight * 0.5f;

        glColor3f(0.75f, 0, 0.15f);
        int i = 0;
        while(i < mNumPoints){
            line[0].set(mPoints[i]);
            line[1].set(line[0]);
            line[0].y += height;
            line[1].y += height;
            line[0].x += pathWidth;
            line[1].x -= pathWidth;

            line[2].set(mPoints[i]);
            line[3].set(line[2]);
            line[2].y -= height;
            line[3].y -= height;
            line[2].x += pathWidth;
            line[3].x -= pathWidth;

            glVertexPointer(3, GL_FLOAT, 0, &line[0].x);
            glColor3f(0.75f,0,0.15f);
            glDrawArrays(GL_LINES, 0, 4);
            glColor3f(1, 1, 1);
            glDrawArrays(GL_POINTS, 0, 4);
            i++;
        }

        glDisableClientState(GL_VERTEX_ARRAY);
        glPointSize(prevPointSize);
    }

    void Diver::debugDrawIndices(const CameraOrtho &camera){
        static const float fontScale = 0.005f;

        Vec3f v;
        Vec3f w;
        Vec3f u;

        camera.getBillboardVectors(&w, &u);
        v = w.cross(u);

        const static Vec2f zero;
        const gl::TextureFontRef& sharedTextureFont = SharedTextureFont::Get();
        float fontDescent = sharedTextureFont->getDescent();

        Matrix44f mat;
        Matrix44f rot = Matrix44f::createRotationOnb(u,w,v);
        rot*= Matrix44f::createRotation(Vec3f::zAxis(), static_cast<float>(M_PI_2));
        rot*= Matrix44f::createScale(Vec3f(fontScale,fontScale,fontScale));

        gl::enableAlphaTest();
        gl::enableAlphaBlending();

        glColor3f(1,1,1);
        int i = -1;
        while(++i < mPoints.size()){
            mat.setToIdentity();
            mat *= Matrix44f::createTranslation(mPoints[i]);
            mat *= rot;

            string stringTexCoord = toString(mTexcoords[i]);
            Vec2f  stringSize = sharedTextureFont->measureString(stringTexCoord);

            glPushMatrix();
            glMultMatrixf(&mat[0]);
            glColor4f(0,0,0,0.75f);
            gl::drawSolidRect(Rectf(0,fontDescent,stringSize.x, stringSize.y * -1+fontDescent));
            glColor3f(1,1,1);
            sharedTextureFont->drawString(stringTexCoord, zero);
            glPopMatrix();
        }

        gl::disableAlphaBlending();
        gl::disableAlphaTest();
    }
}