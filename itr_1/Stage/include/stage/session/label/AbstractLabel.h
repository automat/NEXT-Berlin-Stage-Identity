//
//  AbstractLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_AbstractLabel_h
#define SessionView_AbstractLabel_h

#include <string>

#include "stage/session/AbstractAnimBase.h"

#include "Resources.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"

#include "Config.h"
#include "util/text/Textbox.h"


namespace next {
    using namespace std;
    using namespace ci;
    
    class AbstractLabel : public AbstractAnimBase{
    protected:
        next::TextBox* mTextBox;
        Vec2f mPos;
        
    public:
        AbstractLabel() : AbstractAnimBase(){
            mTextBox = new TextBox();
        };
        
        ~AbstractLabel() {
            delete mTextBox;
        }
        
        inline void setPosition(const Vec2f& pos){
            mPos = pos;
        }
    };
}



#endif
