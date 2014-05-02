//
//  SessionView.h
//  EventsDeck
//
//  Created by Henryk Wollik on 16/04/14.
//
//

#ifndef EventsDeck_SessionView_h
#define EventsDeck_SessionView_h

#include "stage/session/AbstractAnimBase.h"
#include "data/session/Session.h"
#include "stage/session/EventView.h"

#include "stage/session/label/session/SessionMetaLabel.h"
#include "stage/session/label/session/SessionTitleLabel.h"
#include "stage/session/label/event/EventMetaLabel.h"
#include "stage/session/label/event/PingPongEventTitleLabel.h"
#include "stage/session/label/speaker/PingPongSpeakerLabel.h"

#include "cinder/Vector.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"

namespace next {
    using namespace ci;
    
    class SessionView : public AbstractAnimBase{
        Session* mData;
        
        bool mValid;    //  is valid, data > 0
        bool mActive;
        
        std::function<void()> mCallback;
        
        int                mNumEventViews;
        int                mIndexEventViews;
        vector<EventView*> mEventViews;
        vector<int>        mEventViewsOffset;
        int                mEventViewStep;
        int                mEventViewFront;
        int                mEventViewBack;

        Vec3f     mEventViewSlots[5];
        const int mEventViewSlotBegin;
        const int mEventViewSlotEnd;
        const int mEventViewSlotFocus;
        const int mEventViewSlotOut;
        const int mEventViewSlotIn;
        
        Vec3f mSpeakerLabelPos;
        
        gl::Light*   mLight;
        gl::Material mMaterial;

        //! clear all views
        void deleteEventViews();
        //! move all views in steps and direction
        void moveViews(int count = 1);
        //!
        void stepForward_1();
        void stepForward_2();

        //! update view state
        void setViewState(EventView* view, int slot);
        //! focus view
        void focusView(EventView* view);
        //! on all events shown
        void onFinish();
        //! on begin
        void onStart();

        void finish();

        void resetEventViews();
        
        void turnOnSessionLabels();
        void turnOffSessionLabels();
        
        SessionTitleLabel*         mLabelTitle;
        SessionMetaLabel*          mLabelMeta;
        PingPongEventTitleLabel*   mPingPongLabelEventTitle;
        EventMetaLabel*            mLabelEventMeta;
        PingPongSpeakerLabel*      mPingPongLabelSpeaker;
        
        void updateSpeakerLabel(int index = 0);
        
        void loadLightProperties();
        
    public:
        SessionView(Session* data, map<uint32_t, Speaker>* speakerData);
        ~SessionView();
        
        void reset(Session* data);  //  clearStates session with new data
        void play(const std::function<void()>& callback);               //  start animation


        void draw(const CameraOrtho& camera);
        void drawLabels();
        void drawLabelsSpeaker();
        
        void debugDraw();
        void update();

        bool isActive();
        
        void onConfigDidChange();
    };
}

#endif
