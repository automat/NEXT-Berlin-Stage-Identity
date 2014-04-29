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
#include "data/session/Event.h"
#include "util/text/TextBoxTexture.h"


#include <map>
#include <vector>

namespace next {
    using namespace std;
    class EventTitleLabel : public AbstractLabel {
        static map<string, TextBoxTexture>   sMapTitleTexture;
        static map<string, vector<LineQuad>> sMapTitleLineQuads;
    public:
        static void Map(map<uint32_t,Event>* events);
    private:
        string           mString;
        
    public:
        EventTitleLabel();
        
        void draw();
        void set(const string& title);
        
        void on();
        void off();
        
        void show();
        void hide();
    };
}


#endif
