#include "SessionView.h"
#include "Event.h"

#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/Utilities.h"
#include <math.h>

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "Config.h"

#define CLAMP(a,b,c)({\
    MIN(MAX(b,a),c); \
})

namespace next {
    using namespace boost::assign;
    using namespace ci;
    using namespace ci::app;
    
    static const float sSlideLength(4);
    
    //  to be replaced with config
    static const float sTimeAnimateInOut(2.0f);
    
    static const float sTimeAnimateStartEnd(1.0f);

    typedef EaseOutCubic ViewStartEndEasing;
    typedef EaseOutCubic ViewInOutEasing;
    
    /*--------------------------------------------------------------------------------------------*/
    //  Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    SessionView::SessionView(Session* data) :
        AbstractAnimView(),
            mEventViewStep(0),
            mEventViewFront(0),
            mEventViewSlotBegin(0),
            mEventViewSlotFocus(2),
            mEventViewSlotIn(1),
            mEventViewSlotOut(3),
            mEventViewSlotEnd(4){
                
                float slideLength_2 = sSlideLength * 0.5f;
                mEventViewSlots[0] = Vec3f(0,0,-sSlideLength );
                mEventViewSlots[1] = Vec3f(0,0,-slideLength_2);
                mEventViewSlots[3] = Vec3f(0,0, slideLength_2);
                mEventViewSlots[4] = Vec3f(0,0, sSlideLength );
            
                mLabelTitle      = new SessionTitleLabel();
                mLabelEventTitle = new EventTitleLabel();
                
                reset(data);
    }
    
    SessionView::~SessionView(){
        deleteEventViews();
        delete mLabelTitle;
        delete mLabelEventTitle;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Handle Data
    /*--------------------------------------------------------------------------------------------*/
 
    void SessionView::deleteEventViews(){
        while(!mEventViews.empty()) delete mEventViews.back(), mEventViews.pop_back();
    }
    
    void SessionView::reset(Session* data){
        deleteEventViews();
        
        map<uint32_t, Event>* events = data->events;
        
        mNumEventViews = events->size();
        mEventViewStep = mEventViewFront = mEventViewBack = 0;
        mValid         = mNumEventViews != 0;
        mEventViewStep = 0;

        mAnimating = false;

        if(!mValid){
            return;
        }

        int i = -1;
        for(map<uint32_t, Event>::iterator itr = events->begin(); itr != events->end(); ++itr){
            mEventViews += new EventView(&((*itr).second));
            mEventViews.back()->mPositionState = mEventViewSlots[0];
            mEventViewsOffset += -(++i);
        }
        
        mLabelTitle->setString(data->title);
       
        start();
    }

    /*--------------------------------------------------------------------------------------------*/
    //  On start / end
    /*--------------------------------------------------------------------------------------------*/

    void SessionView::onFinish(){
#ifdef SESSION_VIEW_DEBUG_STATE
        cout << "### Session View End ###" << endl;
#endif
    }

    void SessionView::onStart(){
#ifdef SESSION_VIEW_DEBUG_STATE
        cout << "### Session View Start ###" << endl;
#endif
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Animation states
    /*--------------------------------------------------------------------------------------------*/

    void SessionView::stepForward_1(){moveViews();}
    void SessionView::stepForward_2(){moveViews(2);}

    void SessionView::moveViews(int count) {
        if(!mValid ||
            mEventViewFront >= mNumEventViews ||
            mEventViewFront <= -1){
            return;
        }

        int slot;
        EventView* front;

        int i = -1;
        while(++i < count){
            mEventViewStep++;
            
            slot = CLAMP(mEventViewsOffset[mEventViewFront] + mEventViewStep, 0, mEventViewSlotEnd);
            front = mEventViews[mEventViewFront];
            setViewState(front, slot);
    
            int j= mEventViewFront;
            while (++j < mNumEventViews) {
                slot = CLAMP(mEventViewsOffset[j] + mEventViewStep, 0, mEventViewSlotEnd);
       
                if(slot == mEventViewSlotEnd){
                    mEventViewFront++;
                }
                setViewState(mEventViews[j], slot);
            }
        }
        
#ifdef SESSION_VIEW_DEBUG_STATE
        cout << "Front: " << mEventViewFront << endl;
#endif
    }


    void SessionView::start(){
        onStart();
        if(mNumEventViews < 2){
            stepForward_2();
            return;
        }
        stepForward_1();
        
        mLabelEventTitle->setString((*mEventViews.begin())->getData()->title);
    }
    
    void SessionView::focusView(next::EventView *view){
        if(mNumEventViews < 2){
            view->stackSpeaker(std::bind(&SessionView::stepForward_2, this));
        } else {
            view->stackSpeaker(std::bind(&SessionView::stepForward_1, this));
        }
    }

    void SessionView::setViewState(EventView *view, int slot) {
        // view is last, target slot last
        if(view == mEventViews.back() && slot == mEventViewSlotEnd){
            tween(&view->mPositionState,mEventViewSlots[slot],sTimeAnimateInOut,ViewInOutEasing(),
                  NULL,std::bind(&SessionView::onFinish, this));
            return;
        }
        
        // view is first, target slot is in postion
        if(view == mEventViews.front() && slot == mEventViewSlotIn){
            tween(&view->mPositionState, mEventViewSlots[slot], sTimeAnimateInOut, ViewInOutEasing(),
                  NULL, std::bind(&SessionView::stepForward_1, this));
            return;
        }
        
        // target slot is event focus
        if(slot == mEventViewSlotFocus){
            mLabelEventTitle->setString(view->getData()->title);
            view->focusTop();
            tween(&view->mPositionState, mEventViewSlots[slot], sTimeAnimateInOut, ViewInOutEasing(),
                  NULL, std::bind(&SessionView::focusView,this,view));
            return;
        }
        
        // target slot is out position
        if(slot == mEventViewSlotOut){
            view->unfocus();
            
            if(view == mEventViews.back()){
               tween(&view->mPositionState,mEventViewSlots[slot],sTimeAnimateInOut,ViewInOutEasing(),
                      NULL,std::bind(&SessionView::stepForward_1, this));
                return;
            }
        }
        
        tween(&view->mPositionState, mEventViewSlots[slot], sTimeAnimateInOut, ViewInOutEasing());
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::drawLabels(){
        mLabelTitle->draw();
        mLabelEventTitle->draw();
        
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update events
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::draw(){
        for (vector<EventView*>::const_iterator itr = mEventViews.begin(); itr != mEventViews.end(); itr++) {
            (*itr)->draw();
        }
    }
    
    void SessionView::update(){
        for (vector<EventView*>::const_iterator itr = mEventViews.begin(); itr != mEventViews.end(); itr++) {
            (*itr)->update();
        }
    }
    
    void SessionView::debugDraw(){
        const static float fontScale = 0.0125f;
        Vec3f pos;
        glColor3f(1, 1, 1);
        
        gl::enableAlphaTest();
        gl::enableAlphaBlending();
        int i = -1;
        while (++i< mNumEventViews) {
            pos = mEventViews[i]->mPositionState();
            glPushMatrix();
            glTranslatef(pos.x - 1, pos.y, pos.z);
            glRotatef(90, 0, 0, 0);
            glScalef(fontScale, fontScale, fontScale);
            gl::drawString(toString(i),Vec2f::zero());
            glPopMatrix();
            
        }
        gl::disableAlphaBlending();
        gl::disableAlphaTest();
        
        glColor3f(1,0,0);
        gl::drawLine(mEventViewSlots[0], mEventViewSlots[1]);
        gl::drawLine(mEventViewSlots[1], mEventViewSlots[2]);
        gl::drawSphere(mEventViewSlots[0], 0.025f);
        gl::drawSphere(mEventViewSlots[1], 0.025f);
        glColor3f(1,0,1);
        gl::drawLine(mEventViewSlots[2], mEventViewSlots[3]);
        gl::drawLine(mEventViewSlots[3], mEventViewSlots[4]);
        gl::drawSphere(mEventViewSlots[3], 0.025f);
        gl::drawSphere(mEventViewSlots[4], 0.025f);
        
    }
}