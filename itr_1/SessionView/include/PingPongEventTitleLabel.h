//
//  PingPongEventTitleLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 21/04/14.
//
//

#ifndef SessionView_PingPongEventTitleLabel_h
#define SessionView_PingPongEventTitleLabel_h

#include "EventTitleLabel.h"
#include <string>

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
        
        inline void setString(const string& str){
            mLabels[mIndex].setString(str);
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
