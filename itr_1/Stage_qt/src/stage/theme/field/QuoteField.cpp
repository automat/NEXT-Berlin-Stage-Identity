#include "stage/theme/field/QuoteField.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "cinder/app/App.h"
#include "cinder/gl/Texture.h"

#include "Config.h"
#include "util/text/SharedTextureFont.h"


namespace next{
    using namespace boost::assign;

    QuoteField::QuoteField(const Vec3f& pos, int numPathSlices, const QuoteLine& quoteLine) :
    AbstractField(pos - Vec3f(quoteLine.getIndices().size(),0,0),numPathSlices,quoteLine.getIndices().size()){
        mSurfaceDisplacement = Vec3f(Rand::randInt(0, STAGE_GRID_NUM_CELLS_XY),0,Rand::randInt(0, STAGE_GRID_NUM_CELLS_XY));
        mSurfaceAmplitude    = QUOTE_FIELD_SURFACE_PATH_AMPLITUDE;
        mSurfaceDensity      = QUOTE_FIELD_SURFACE_PATH_DENSITY;
        mSurfaceOffsetSpeed  = QUOTE_FIELD_SURFACE_PATH_OFFSET_SPEED;

        mDiverUnitNumPoints = QUOTE_FIELD_DIVER_NUM_POINTS;
        mDiverHeightMin     = QUOTE_FIELD_DIVER_MIN_HEIGHT;
        mDiverHeightMax     = QUOTE_FIELD_DIVER_MAX_HEIGHT;
        mDiverOffsetXMin    = QUOTE_FIELD_DIVER_MIN_OFFSET_X;
        mDiverOffsetXMax    = QUOTE_FIELD_DIVER_MAX_OFFSET_X;
        mDiverOffsetYMin    = QUOTE_FIELD_DIVER_MIN_OFFSET_Y;
        mDiverOffsetYMax    = QUOTE_FIELD_DIVER_MAX_OFFSET_Y;
        mDiverSpeedMin      = QUOTE_FIELD_DIVER_MIN_SPEED;
        mDiverSpeedMax      = QUOTE_FIELD_DIVER_MAX_SPEED;
        mDiverLengthMin     = QUOTE_FIELD_DIVER_MIN_LENGTH;
        mDiverLengthMax     = QUOTE_FIELD_DIVER_MAX_LENGTH;

       // mBufferTexcoords.resize(m)
        
        //mMeshLayout.setStaticTexCoords2d();
        mMeshLayout.setStaticColorsRGB();
        mMeshLayout.setStaticTexCoords2d();
        reset(pos, numPathSlices, quoteLine);
    }


    void QuoteField::addMeshColors(){
        //
        //  Instead of using a custom attribute,
        //  the vertex color data is used to send a collection of custom data
        //  for each vertex to the shader
        //

        const Colorf data0(1,1,0);  // r = texture should be applied yes/no, g = how strong should the texture be merged
        const Colorf data1(1,0,0);  // for left / right top
        const Colorf data2(0,0,0);  // off data

        vector<Colorf> meshColors;

        int i,j;
        i = -1;
        while (++i < mNumDivers) {
            j = -1;
            while (++j < mDiverNumPoints) {
                meshColors += data2, data2;
                meshColors += data0, data0;
                meshColors += data2, data2;
                meshColors += data2, data2;;
            }
            meshColors += data2, data2;
            meshColors += data2, data2;
            meshColors += data2, data2;
            meshColors += data2, data2;
        }

        mMesh.bufferColorsRGB(meshColors);
    }


/*--------------------------------------------------------------------------------------------*/
//  Reset
/*--------------------------------------------------------------------------------------------*/

    void QuoteField::reset(const Vec3f &pos, int numPathSlices, const QuoteLine& quoteLine){
        mPos              = pos;
        mSurfaceNumSlices = numPathSlices;
        mSize             = quoteLine.getIndices().size();

        mTexcoordStart    = quoteLine.getTexcoords()[0];
        mTexcoordStep     = Vec2f((quoteLine.getTexcoords()[1].x - mTexcoordStart.x),
                                  (quoteLine.getTexcoords()[2].y - mTexcoordStart.y) / static_cast<float>(mSurfaceNumSlices));
        
        mTransform.setToIdentity();
        mTransform.translate(pos);

        reset_Internal();
        
        mBufferTexcoords.resize(mMesh.getNumVertices());
        
    }

/*--------------------------------------------------------------------------------------------*/
// Draw / Update
/*--------------------------------------------------------------------------------------------*/


    void QuoteField::updateDivers(float value){
        const static Vec2f zero;
        Vec2f tex_0;   //upper tex coord
        Vec2f tex_1;   //lower tex coord

        float texcoordStartX = mTexcoordStart.x;
        float texcoordStartY = mTexcoordStart.y;
        float texcoordStepX  = mTexcoordStep.x;
        float texcoordStepY  = mTexcoordStep.y;

        int i,j,k;
        i = k = 0;
 
        //
        //  Moved from using dynamic texcoords to a static buffer,
        //  because vbomesh itr.setTexcoodX was messing up the overal mesh geomertry,
        //  although it stepped through correctly
        //
        
        
        //gl::VboMesh::VertexIter vbItr = mMesh.mapVertexBuffer();
        for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
            Diver* diver = *itr;
            
            diver->setOffset(diver->getOffsetXInitial() + value); //diver->getOffsetXInitial() + value);    //  diver->getOffsetInitial() + value);
            diver->update();            //  update diver position
            
            //
            //  Update mesh texcoords according to diver texcoords
            //

            const vector<float>& texcoords = diver->getTexcoords();
            tex_0.y = texcoordStartY + texcoordStepY  * static_cast<float>(i++);
            tex_1.y = tex_0.y + texcoordStepY;
            
            
            static const Vec2f zero;
            
            j = -1;
            while (++j < mDiverNumPoints) {
                tex_0.x = tex_1.x = texcoordStartX + texcoordStepX * texcoords[j];  //  get sliced hotizontal
                
                k+=2;
                mBufferTexcoords[k++] = tex_0;
                mBufferTexcoords[k++] = tex_1;
                
                mBufferTexcoords[k++] = tex_0;
                mBufferTexcoords[k++] = tex_1;
                mBufferTexcoords[k++] = tex_0;
                mBufferTexcoords[k++] = tex_1;
                
                /*
                vbItr.setTexCoord2d0(zero); ++vbItr;    //  skip bottom
                vbItr.setTexCoord2d0(zero); ++vbItr;

                vbItr.setTexCoord2d0(tex_0); ++vbItr;   //  top
                vbItr.setTexCoord2d0(tex_1); ++vbItr;

                vbItr.setTexCoord2d0(tex_0); ++vbItr;   //  top / bottom left
                vbItr.setTexCoord2d0(tex_1); ++vbItr;
                vbItr.setTexCoord2d0(tex_0); ++vbItr;   //  top / bottom right
                vbItr.setTexCoord2d0(tex_1); ++vbItr;
                 */
            }
            
            k += 8;
            /*
            vbItr.setTexCoord2d0(zero); ++vbItr;        //  skip front
            vbItr.setTexCoord2d0(zero); ++vbItr;
            vbItr.setTexCoord2d0(zero); ++vbItr;
            vbItr.setTexCoord2d0(zero); ++vbItr;
            
            vbItr.setTexCoord2d0(zero); ++vbItr;        //  skip back
            vbItr.setTexCoord2d0(zero); ++vbItr;
            vbItr.setTexCoord2d0(zero); ++vbItr;
            vbItr.setTexCoord2d0(zero); ++vbItr;
             */
            
            diver->updateTexcoords();   //  update diver texcooord
        
            
        }
    }

    void QuoteField::draw(){
        mMesh.bufferTexCoords2d(0, mBufferTexcoords);
        drawMesh();
        mMesh.unbindBuffers();
    }

    void QuoteField::update(Oscillator *osc, float t){
        mSurfaceOffset -= mSurfaceOffsetSpeed;
        updatePathSurface(osc, mSurfaceOffset);
        updateMesh();
    }


/*--------------------------------------------------------------------------------------------*/
//  Debug draw
/*--------------------------------------------------------------------------------------------*/

    void QuoteField::debugDrawArea(){
        glColor3f(1,0,1);
        debugDrawArea_Internal();
    }

    void QuoteField::debugDrawDiverIndices(const cinder::CameraOrtho &camera){
        glPushMatrix();
        glMultMatrixf(&mTransform[0]);
        for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
            (*itr)->debugDrawIndices(camera);
        }
        glPopMatrix();
    }

    void QuoteField::debugDrawIndices(const cinder::CameraOrtho &camera){
    }

}