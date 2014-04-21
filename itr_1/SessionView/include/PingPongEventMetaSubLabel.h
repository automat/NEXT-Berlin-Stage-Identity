//
//  PingPongEventMetaSubLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 22/04/14.
//
//

#ifndef SessionView_PingPongEventMetaSubLabel_h
#define SessionView_PingPongEventMetaSubLabel_h

#include "SubLabel.h"

namespace next {
     class PingPongEventMetaSubLabel {
         int mIndex;
         SubLabel mLabels[2];
        
    public:
         PingPongEventMetaSubLabel() : mIndex{}
         
         inline void draw(){
             mLabels[0].draw();
             mLabels[1].draw();
         }
         
         inline void update(){
             mLabels[0].update();
             mLabels[1].update();
         }
         
         inline void set(const string& type){
             mLabels[mIndex].setString(type);
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
