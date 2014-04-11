#include "stage/schedule/ScheduleView.h"
#include "stage/schedule/person/Person.h"
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>
#include "cinder/Vector.h"

#include "Resources.h"
#include "cinder/app/App.h"

#include "util/DrawUtil.h"
#include "util/TextureFontTranscript.h"

using namespace boost::assign;

ScheduleView::ScheduleView(Grid* grid, const LayoutArea& area) :
    AbstractView(grid,area){
        mTexFontTranscript100 = TextureFontTranscript::GetSize48();
        
        mImageMap[RES_TEST_IMAGE_NAME] = gl::Texture(loadImage(static_cast<string>(RES_TEST_IMAGES_PATH) + RES_TEST_IMAGE_NAME));
        mTestPersons    += Person::Create(0, "", "Darth", "Vader", "Lord", "Siths", RES_TEST_IMAGE_NAME);
        mTestPersonStack = new PersonViewStack(mTestPersons);
    
}

ScheduleView::~ScheduleView(){
    while (!mTestPersons.empty()) delete mTestPersons.back(), mTestPersons.pop_back();
    delete mTestPersonStack;
}

void ScheduleView::drawStackLayer(){
    mTestPersonStack->draw();
}

void ScheduleView::drawInfoLayer(){
    gl::pushMatrices();
    gl::setMatricesWindow(app::getWindowSize());
    
    glColor3f(1,1,1);
    
    //gl::drawSolidCircle(Vec2f::zero(), 200);
    
    //mTexFontTranscript100->drawString("Darth Vader", Vec2f::zero());
    
    gl::enableAlphaTest();
    gl::enableAdditiveBlending();
    //utils::drawString(mTexFontTranscript100, "ABCDEFG", Vec2f(100,100));
    glPushMatrix();
    glTranslatef(100, 100, 0);
    TextureFontTranscript::drawString("ABCD", Vec2f::zero());
    glColor3f(223.0f/255.0f,17.0f/255.0f,101.0f/255.0f);
    TextureFontTranscript::drawString("ABCD", Vec2f(0,300), TextureFontTranscript::FontSize::Size_48);

    glColor3f(0,0,0);
    gl::drawSolidCircle(Vec2f::zero(), 100);
    
    
    glColor3f(1, 1, 1);
    

    
    glPopMatrix();
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
    
    gl::popMatrices();
}

void ScheduleView::onConfigDidChange(){
    
}

void ScheduleView::draw(const CameraOrtho& camera, bool useMaterialShader){
    drawStackLayer();
    drawInfoLayer();
    
    
}

void ScheduleView::update(){
    mTestPersonStack->update();
    
}
