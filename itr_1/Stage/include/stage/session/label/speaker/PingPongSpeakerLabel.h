//
//  PingPongSpeakerLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 23/04/14.
//
//

#ifndef SessionView_PingPongSpeakerLabel_h
#define SessionView_PingPongSpeakerLabel_h

#include "stage/session/label/speaker/SpeakerLabel.h"
#include "cinder/Vector.h"

namespace next {
    using namespace ci;
    
    class PingPongSpeakerLabel {
        int          mIndex;
        SpeakerLabel mLabels[2];
        
    public:
        PingPongSpeakerLabel() : mIndex(0){}
        
        inline void setPosition(const Vec3f& pos){
            mLabels[0].setPosition(pos);
            mLabels[1].setPosition(pos);
        }
        
        inline void draw(){
            glPushMatrix();
                mLabels[1-mIndex].draw();
                glTranslatef(0, 0.000125f, 0);
                mLabels[mIndex].draw();
            glPopMatrix();
        }
        
        inline void update(){
            mLabels[0].update();
            mLabels[1].update();
        }
        
        inline void set(const string& name, const string& company){
            mLabels[mIndex].set(name, company);
        }
        
        inline void show(){
            mLabels[mIndex].show();
        }
        
        inline void hide(){
            mLabels[mIndex].hide();
        }
        
        inline void on(){
            mLabels[mIndex].on();
        }
        
        inline void off(){
            mLabels[mIndex].off();
        }
        
        inline void swap(){
            mIndex = 1 - mIndex;
        }
    };
}

#endif
