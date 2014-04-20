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

#include "AbstractAnimView.h"
#include "Textbox.h"
#include "Resources.h"
#include "cinder/app/App.h"
#include "Config.h"
#include "cinder/Vector.h"

namespace next {
    using namespace std;
    using namespace ci;
    
    class AbstractLabel : public AbstractAnimView{
    protected:
        next::TextBox* mTextBox;
        Vec2f mPos;
        
    public:
        AbstractLabel() : AbstractAnimView(){
            mTextBox = new TextBox();
        };
        
        ~AbstractLabel() {
            delete mTextBox;
        }
        
        inline void setString(const string& str){
            mTextBox->setString(str);
        }
        
        inline void setPosition(const Vec2f& pos){
            mPos = pos;
        }
        
    };
}



#endif
