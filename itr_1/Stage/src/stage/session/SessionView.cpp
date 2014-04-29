#include "stage/session/SessionView.h"

#include <OpenGL/OpenGL.h>
#include <math.h>
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"
#include "cinder/Utilities.h"
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "data/session/Event.h"

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
    //  Constructor / Destructor
    /*--------------------------------------------------------------------------------------------*/
    
    SessionView::SessionView(Session* data, map<uint32_t, Speaker>* speakerData) :
        AbstractAnimBase(),
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
                
                EventTitleLabel::Map(data->events);
                EventMetaLabel::Map( data->events);
                EventTypeLabel::Map( data->events);
                SpeakerLabel::Map(   speakerData);
                
                
                mLabelTitle              = new SessionTitleLabel();
                mLabelMeta               = new SessionMetaLabel();
                mPingPongLabelEventTitle = new PingPongEventTitleLabel();
                mLabelEventMeta          = new EventMetaLabel();
                mPingPongLabelSpeaker    = new PingPongSpeakerLabel();
                
                Vec3f offset(SESSION_VIEW_SPEAKER_SIZE.x * 0.5f + SESSION_VIEW_SPEAKER_NAME_LABEL_OFFSET,
                             SESSION_VIEW_SPEAKER_SIZE.y * -1,
                             SESSION_VIEW_SPEAKER_SIZE.x * 0.5f);
                
                mPingPongLabelSpeaker->setPosition(mEventViewSlots[2] + offset);
                
                reset(data);
    }
    
    SessionView::~SessionView(){
        deleteEventViews();
        delete mLabelTitle;
        delete mLabelMeta;
        delete mPingPongLabelEventTitle;
        delete mLabelEventMeta;
        delete mPingPongLabelSpeaker;
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

        mActive    = false;

        if(!mValid){
            return;
        }

        int i = -1;
        for(map<uint32_t, Event>::iterator itr = events->begin(); itr != events->end(); ++itr){
            mEventViews += new EventView(&((*itr).second));
            mEventViews.back()->mPositionState = mEventViewSlots[0];
            mEventViewsOffset += -(++i);
        }
        
        mLabelTitle->set(mData->title);
        mLabelMeta->set(mData->startHourString, mData->endHourString, mData->startTimeStamp, mData->clockImageRef);
    }
    
    void SessionView::resetEventViews(){
        if(!mValid){
            return;
        }
        
        mEventViewStep = 0;
        mIndexEventViews = 0;
        mEventViewFront = 0;
        
        for(vector<EventView*>::iterator itr = mEventViews.begin(); itr != mEventViews.end(); ++itr){
            (*itr)->mPositionState = mEventViewSlots[0];
            (*itr)->resetStack();
        }
    }

    /*--------------------------------------------------------------------------------------------*/
    //  Labels
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::turnOnSessionLabels(){
        mLabelMeta->on();
        mLabelTitle->on();
    }
    
    void SessionView::turnOffSessionLabels(){
        mLabelMeta->off();
        mLabelTitle->off();
    }
    
    void SessionView::updateSpeakerLabel(int index){
        mPingPongLabelSpeaker->hide();
        mPingPongLabelSpeaker->swap();
        
        uint32_t indexEvent = mIndexEventViews - 1;
        
        map<uint32_t, Event>::iterator itr_eventData = mData->events->begin();
        std::advance(itr_eventData, indexEvent);
        const Speaker* speakerData = itr_eventData->second.speakers[index];
        
        mPingPongLabelSpeaker->set(speakerData->name, speakerData->companyName);
        mPingPongLabelSpeaker->show();
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
    
    /*--------------------------------------------------------------------------------------------*/
    //  start / finish + callbacks
    /*--------------------------------------------------------------------------------------------*/
    
    void SessionView::onFinish(){
#ifdef SESSION_VIEW_DEBUG_STATE
        cout << "### Session View End ###" << endl;
#endif
        if(mCallback){
            mCallback();
        }
    }
    
    void SessionView::onStart(){
#ifdef SESSION_VIEW_DEBUG_STATE
        cout << "### Session View Start ###" << endl;
#endif
    }
    
    void SessionView::finish(){
        resetEventViews();
        delayCallback(SESSION_VIEW_ON_FINISH_DELAY, std::bind(&SessionView::onFinish,this));
        mActive = false;
    }
    
    // public fire!
    void SessionView::play(const std::function<void()>& callback){
        if(mActive){
            return;
        }
        mCallback = callback;
        turnOnSessionLabels();
        delayCallback(SESSION_VIEW_START_DELAY, std::bind(&SessionView::stepForward_2,this));
        
        mActive = true;
        onStart();
    }

    /*--------------------------------------------------------------------------------------------*/
    //  move event queue
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
            
            slot  = CLAMP(mEventViewsOffset[mEventViewFront] + mEventViewStep, 0, mEventViewSlotEnd);
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
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Update Event View state
    /*--------------------------------------------------------------------------------------------*/

    void SessionView::setViewState(EventView *view, int slot) {
        // view is last, target is last
        if((view == mEventViews.back() && slot == mEventViewSlotOut) ||
           (view == mEventViews.back() && slot == mEventViewSlotEnd && mNumEventViews == 1)){
            
            tween(&view->mPositionState,mEventViewSlots[mEventViewSlotEnd], // skip one, and proceed to last
                  SESSION_EVENT_ANIM_TIME_OFF,
                  ViewInOutEasing(),
                  NULL,std::bind(&SessionView::finish, this));
        
            view->unfocusOut();
            
            delayCallback(1.375f, std::bind(&SessionView::turnOffSessionLabels, this));
            
            mPingPongLabelEventTitle->off();
            mPingPongLabelSpeaker->off();
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
            
            mPingPongLabelEventTitle->set(data->title);
            
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
            
            mLabelEventMeta->set(data->type, mIndexEventViews);
            
            //
            //  trigger focus
            //
            if(view == mEventViews.front()){
                view->focusIn();
            } else {
                view->focusTop();
            }
            
            tween(&view->mPositionState, mEventViewSlots[slot],
                  SESSION_EVENT_ANIM_IN_OUT,
                  ViewInOutEasing(),
                  NULL, std::bind(&SessionView::focusView,this,view));
            
            mIndexEventViews++;

            updateSpeakerLabel();
            
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
    
    void SessionView::draw(){
        if(!mActive){
            return;
        }
        for (vector<EventView*>::const_iterator itr = mEventViews.begin(); itr != mEventViews.end(); itr++) {
            (*itr)->draw();
        }
        mPingPongLabelSpeaker->draw();
    }

    void SessionView::update(){
        if(!mActive){
            return;
        }
        mLabelMeta->update();
    }
    
    void SessionView::drawLabels(){
        if(!mActive){
            return;
        }
        mLabelTitle->draw();
        mLabelMeta->draw();
        mPingPongLabelEventTitle->draw();
        mLabelEventMeta->draw();
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
    
    /*--------------------------------------------------------------------------------------------*/
    //  Stage
    /*--------------------------------------------------------------------------------------------*/
    
    bool SessionView::isActive(){
        return mActive;
    }
}