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
        
        inline void update(){
            mLabels[0].update();
            mLabels[1].update();
        }
        
        inline void setString(const string& str){
            mLabels[mIndex].setString(str);
        }
        
        inline void swap(){
            mIndex = 1 - mIndex;
        }
        
    };
}


#endif
