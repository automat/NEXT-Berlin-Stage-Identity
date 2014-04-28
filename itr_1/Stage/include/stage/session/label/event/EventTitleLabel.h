//
//  EventTitleLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 20/04/14.
//
//

#ifndef SessionView_EventTitleLabel_h
#define SessionView_EventTitleLabel_h

#include "stage/session/label/AbstractLabel.h"
#include "stage/session/label/LineQuad.h"

#include <vector>

namespace next {
    using namespace std;
    class EventTitleLabel : public AbstractLabel {
        static Font sFontDefault;
        static Font sFontExceed;
        
        TextBox*         mTextBoxExceed;
        
        bool             mDidExceedNumLineMax;
        string           mString;
        vector<LineQuad> mLineQuads;
        void genQuads();
        
    public:
        EventTitleLabel();
        ~EventTitleLabel();
        
        void draw();
        void set(const string& title);
        
        void on();
        void off();
        
        void show();
        void hide();
    };
}


#endif
