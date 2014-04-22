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
    
    typedef EaseOutCubic ViewStartEndEasing;
    typedef EaseInOutQuad ViewInOutEasing;
    
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
                mEventViewSlots[0] = Vec3f(SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET,0,-SESSION_VIEW_SLIDE_LENGTH - SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET);
                mEventViewSlots[1] = Vec3f(SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET,0,-SESSION_VIEW_SLIDE_LENGTH * 0.5f - SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET);
                mEventViewSlots[2] = Vec3f(SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET,0,-SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET);
                mEventViewSlots[3] = Vec3f(SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET,0, SESSION_VIEW_SLIDE_LENGTH * 0.5f - SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET);
                mEventViewSlots[4] = Vec3f(SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET,0, SESSION_VIEW_SLIDE_LENGTH - SESSION_VIEW_SLIDE_SCREEN_CENTER_OFFSET);
            
                mSpeakerLabelPos = mEventViewSlots[2];
                
                mLabelTitle              = new SessionTitleLabel();
                mLabelMeta               = new SessionMetaLabel();
                mPingPongLabelEventTitle = new PingPongEventTitleLabel();
                mLabelEventMeta          = new EventMetaLabel();
                mLabelSpeaker            = new SpeakerLabel();
                
                mLabelSpeaker->setPosition(mEventViewSlots[2] +
                                           Vec3f(SESSION_VIEW_SPEAKER_SIZE.x * 0.5f + SESSION_VIEW_SPEAKER_NAME_LABEL_OFFSET,
                                                 SESSION_VIEW_SPEAKER_SIZE.y * -1,
                                                 SESSION_VIEW_SPEAKER_SIZE.x * 0.5f));
                
                reset(data);
    }
    
    SessionView::~SessionView(){
        deleteEventViews();
        delete mLabelTitle;
        delete mLabelMeta;
        delete mPingPongLabelEventTitle;
        delete mLabelEventMeta;
        delete mLabelSpeaker;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Handle Data
    /*--------------------------------------------------------------------------------------------*/
 
    void SessionView::deleteEventViews(){
        while(!mEventViews.empty()) delete mEventViews.back(), mEventViews.pop_back();
    }
    
    void SessionView::reset(Session* data){
        deleteEventViews();
        
        mData = data;
        map<uint32_t, Event>* events = mData->events;
        
        mNumEventViews   = events->size();
        mEventViewStep   = mEventViewFront = mEventViewBack = 0;
        mValid           = mNumEventViews != 0;
        mIndexEventViews = 0;

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
        
        mLabelTitle->setString(mData->title);
        mLabelMeta->set(mData->startHourString, mData->endHourString, mData->startTimeStamp);
    }
    
    void SessionView::resetEventViews(){
        if(!mValid){
            return;
        }
        
        mEventViewStep = 0;
        mIndexEventViews = 0;
        mEventViewFront = 0;
        mAnimating = false;
        
        for(vector<EventView*>::iterator itr = mEventViews.begin(); itr != mEventViews.end(); ++itr){
            (*itr)->mPositionState = mEventViewSlots[0];
            (*itr)->resetStack();
        }
    }

    /*--------------------------------------------------------------------------------------------*/
    //  On start / end
    /*--------------------------------------------------------------------------------------------*/

    void SessionView::onFinish(){
#ifdef SESSION_VIEW_DEBUG_STATE
        cout << "### Session View End ###" << endl;
#endif
        start();
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
    
    
    void SessionView::finish(){
        mLabelMeta->off();
        mLabelTitle->off();
        resetEventViews();
        delayCallback(1.0f, std::bind(&SessionView::onFinish,this));
    }
    
    void SessionView::start(){
        if(mAnimating){
            return;
        }
        mLabelMeta->on();
        mLabelTitle->on();
        delayCallback(0.5f, std::bind(&SessionView::stepForward_2,this));
      
        mAnimating = true;
        onStart();
    }
    
    void SessionView::updateSpeakerLabel(int index){
        //mLabelSpeaker->set(toString(mEventViews[mIndexEventViews]->getStackIndex()), "Microsoft");
        cout << index << endl;
    }
    
    void SessionView::focusView(next::EventView *view){
        if(mNumEventViews < 2){
            view->stackSpeaker(std::bind(&SessionView::updateSpeakerLabel, this, std::placeholders::_1),
                               std::bind(&SessionView::stepForward_2, this));
        } else {
            view->stackSpeaker(std::bind(&SessionView::updateSpeakerLabel, this, std::placeholders::_1),
                               std::bind(&SessionView::stepForward_1, this));
        }
    }

    void SessionView::setViewState(EventView *view, int slot) {
        // view is last, target is last
        if(view == mEventViews.back() && slot == mEventViewSlotOut){
            tween(&view->mPositionState,mEventViewSlots[mEventViewSlotEnd], // skip one, and proceed to last
                  SESSION_EVENT_ANIM_TIME_OFF,
                  ViewInOutEasing(),
                  NULL,std::bind(&SessionView::finish, this));
            mPingPongLabelEventTitle->off();
            mLabelEventMeta->off();
            return;
        }
        
        // view is first, target slot is in postion
        if(view == mEventViews.front() && slot == mEventViewSlotIn){
            tween(&view->mPositionState, mEventViewSlots[slot],
                  SESSION_EVENT_ANIM_IN_OUT,
                  ViewInOutEasing(),
                  NULL, std::bind(&SessionView::stepForward_1, this));
            return;
        }
        
        // target slot is event focus
        if(slot == mEventViewSlotFocus){
            
            if(mIndexEventViews != 0){
                //
                //  Hide previous event label
                //
                mPingPongLabelEventTitle->hide();
                mPingPongLabelEventTitle->swap();
            }
            
            //
            //  Set data & show next event label
            //
            map<uint32_t, Event>::iterator eventData = mData->events->begin();
            std::advance(eventData, mIndexEventViews);
            Event* data = &eventData->second;
            
            mPingPongLabelEventTitle->setString(data->title);
            
            if(mIndexEventViews == 0){
                //
                //  turn label on
                //
                mPingPongLabelEventTitle->on();
                mLabelEventMeta->on();
            } else {
                
                //
                //  show next label
                //
                mPingPongLabelEventTitle->show();
            }
            
            mLabelEventMeta->set(data->type, toString(mIndexEventViews + 1) + " / " + toString(mNumEventViews));
            
            //
            //  trigger focus
            //
            view->focusTop();
            tween(&view->mPositionState, mEventViewSlots[slot],
                  SESSION_EVENT_ANIM_IN_OUT,
                  ViewInOutEasing(),
                  NULL, std::bind(&SessionView::focusView,this,view));
            
            mIndexEventViews++;
            return;
        }
        
        // target slot is out position
        if(slot == mEventViewSlotOut){
            view->unfocus();
            
            if(view == mEventViews.back()){
               tween(&view->mPositionState,mEventViewSlots[slot],
                     SESSION_EVENT_ANIM_IN_OUT,
                     ViewInOutEasing(),
                     NULL,std::bind(&SessionView::stepForward_1, this));
                return;
            }
        }
        
        tween(&view->mPositionState, mEventViewSlots[slot],
              SESSION_EVENT_ANIM_IN_OUT,
              ViewInOutEasing());
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::drawLabels(){
        gl::disableDepthRead();
        mLabelTitle->draw();
        mLabelMeta->draw();
        mPingPongLabelEventTitle->draw();
        mLabelEventMeta->draw();
        gl::enableDepthRead();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update events
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::draw(){
        for (vector<EventView*>::const_iterator itr = mEventViews.begin(); itr != mEventViews.end(); itr++) {
            (*itr)->draw();
        }
        
        glPushMatrix();
        mLabelSpeaker->draw();
        glPopMatrix();
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