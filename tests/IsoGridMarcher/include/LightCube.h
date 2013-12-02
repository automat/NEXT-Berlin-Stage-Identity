//
//  GridCube.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 28/11/13.
//
//

#ifndef IsoGridMarcher_GridCube_h
#define IsoGridMarcher_GridCube_h

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "LightBulb.h"

class LightCube {
private:
    //top
    LightBulb* mBulb0;
    LightBulb* mBulb1;
    LightBulb* mBulb2;
    LightBulb* mBulb3;
    
    //bottom
    LightBulb* mBulb4;
    LightBulb* mBulb5;
    LightBulb* mBulb6;
    LightBulb* mBulb7;
    
public:
    LightCube(){
        mBulb0 = 0;
        mBulb1 = 0;
        mBulb2 = 0;
        mBulb3 = 0;
        
        mBulb4 = 0;
        mBulb5 = 0;
        mBulb6 = 0;
        mBulb7 = 0;
    }
    
    void setTop(LightBulb* bulb0, LightBulb* bulb1, LightBulb* bulb2, LightBulb* bulb3){
        mBulb0 = bulb0;
        mBulb1 = bulb1;
        mBulb2 = bulb2;
        mBulb3 = bulb3;
    }
    
    void setBottom(LightBulb* bulb0, LightBulb* bulb1, LightBulb* bulb2, LightBulb* bulb3){
        mBulb4 = bulb0;
        mBulb5 = bulb1;
        mBulb6 = bulb2;
        mBulb7 = bulb3;
    }
    
    void switchOn(){
    /*
        if(mBulb0)mBulb0->switchOn();
        
        if(mBulb1)mBulb1->switchOn();
        if(mBulb2)mBulb2->switchOn();
        if(mBulb3)mBulb3->switchOn();
        */
        if(mBulb4)mBulb4->switchOn();
        /*
        if(mBulb5)mBulb5->switchOn();
        if(mBulb6)mBulb6->switchOn();
        if(mBulb7)mBulb7->switchOn();
         */
    }
    
    void switchOff(){
        
       // if(mBulb0)mBulb0->switchOff();
        /*
        if(mBulb1)mBulb1->switchOff();
        if(mBulb2)mBulb2->switchOff();
        if(mBulb3)mBulb3->switchOff();
        */
        
       // if(mBulb4)mBulb4->switchOff();
       // if(mBulb5)mBulb5->switchOff();
       // if(mBulb6)mBulb6->switchOff();
       //  if(mBulb7)mBulb7->switchOff();
    }
    
    
    

};

#endif
