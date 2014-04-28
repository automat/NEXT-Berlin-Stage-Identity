//
//  SpeakerLabel.h
//  SessionView
//
//  Created by Henryk Wollik on 22/04/14.
//
//

#ifndef SessionView_SpeakerLabel_h
#define SessionView_SpeakerLabel_h

#include "stage/session/label/AbstractLabel.h"
#include "cinder/Matrix.h"

namespace next {
    using namespace std;
    class SpeakerLabel : public AbstractLabel {
        static Font sFontName;
        static Font sFontCompany;

        float      mScale;
        Matrix44f  mTransform;

        Anim<float> mOffsetStateName;
        Anim<float> mOffsetStateCompany;
        Anim<float> mAlphaStateName;
        Anim<float> mAlphaStateCompany;

        TextBox* mTextBoxCompany;
        float    mTextBoxCompanyOffsetY;
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
