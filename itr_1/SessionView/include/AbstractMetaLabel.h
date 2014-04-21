//
//  AbstractMetaLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 21/04/14.
//
//

#ifndef SessionView_AbstractMetaLabel_h
#define SessionView_AbstractMetaLabel_h

#include "AbstractLabel.h"
#include "TextBox.h"

namespace next {
    class AbstractMetaLabel : public AbstractLabel {
    protected:
        float mTextBoxFrontWidth;
        next::TextBox* mTextBoxSub;
        
        
        
    public:
        AbstractMetaLabel() :
            AbstractLabel() ,
            mTextBoxFrontWidth(0){
        }
        
        ~AbstractMetaLabel() {
            delete mTextBoxSub;
        }
    };
    
}



#endif
