//
//  PingPongEventTitleLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 21/04/14.
//
//

#ifndef SessionView_PingPongEventTitleLabel_h
#define SessionView_PingPongEventTitleLabel_h

#include <string>
#include "EventTitleLabel.h"

namespace next {
    using namespace std;
    
    class PingPongEventTitleLabel {
        int mIndex;
        EventTitleLabel mLabels[2];
        
    public:
        PingPongEventTitleLabel() : mIndex(0){}
        
        inline void draw(){
            mLabels[0].draw();
            mLabels[1].draw();
        }
        
        inline void set(const string& title){
            mLabels[mIndex].set(title);
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
