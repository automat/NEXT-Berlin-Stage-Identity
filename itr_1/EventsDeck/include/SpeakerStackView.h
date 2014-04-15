//
//  SpeakerStackView.h
//  SpeakerImageFilter
//
//  Created by Henryk Wollik on 15/04/14.
//
//

#ifndef SpeakerImageFilter_SpeakerStackView_h
#define SpeakerImageFilter_SpeakerStackView_h

#include <vector>
#include "SpeakerView.h"

using namespace std;
class SpeakerStackView {
    static const float sStackPadding;
    
    vector<SpeakerView*> mViews;
    size_t               mNumViews;
    int                  mViewTop;
    
    void deleteViews();
    
    void next();
    
public:
    SpeakerStackView(const vector<Speaker*>& data);
    ~SpeakerStackView();
    
    void reset(const vector<Speaker*>& data);
    
    void draw();
    void update();
    
};

#endif
