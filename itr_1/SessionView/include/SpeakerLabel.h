//
//  SpeakerLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 22/04/14.
//
//

#ifndef SessionView_SpeakerLabel_h
#define SessionView_SpeakerLabel_h

#include "AbstractLabel.h"
#include "cinder/Matrix.h"

namespace next {
    using namespace std;
    class SpeakerLabel : public AbstractLabel {
        string           mString;
        float            mScale;
        Matrix44f        mTransform;
        
        float mTextBoxNameHeight;
        
        TextBox* mTextBoxCompany;
        
    public:
        SpeakerLabel();
        ~SpeakerLabel();
        
        void draw();
        void set(const string& name, const string& company);
        
        void setPosition(const Vec3f& pos);
         
        void on();
        void off();
        
        void show();
        void hide();
    };
}


#endif
