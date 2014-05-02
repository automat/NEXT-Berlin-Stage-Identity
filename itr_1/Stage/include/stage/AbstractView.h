//
//  AbstractView.h
//  grid_01
//
//  Created by Henryk Wollik on 11/04/14.
//
//

#ifndef grid_01_AbstractView_h
#define grid_01_AbstractView_h

#include "cinder/Camera.h"
#include "util/LayoutArea.h"
#include "stage/grid/Grid.h"

using namespace ci;

namespace next{
    class AbstractView {
     public:
        virtual void onConfigDidChange() = 0;
    };
}



#endif
