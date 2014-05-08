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
#include <map>
#include <vector>
#include "data/session/Speaker.h"
#include "util/text/TextBoxTexture.h"

namespace next {
    using namespace std;
    class SpeakerLabel : public AbstractLabel {
        static map<string, TextBoxTexture> sMapNameTexture;
        static map<string, TextBoxTexture> sMapCompanyTexture;
        
        string      mKeyName;
        string      mKeyCompany;

        float       mScale;
        Matrix44f   mTransform;

        Anim<float> mOffsetStateName;
        Anim<float> mOffsetStateCompany;
        Anim<float> mAlphaStateName;
        Anim<float> mAlphaStateCompany;

        float    mTextBoxCompanyOffsetY;
    public:
        static void Map(map<uint32_t,next::Speaker>* speaker);
        
        SpeakerLabel();

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
