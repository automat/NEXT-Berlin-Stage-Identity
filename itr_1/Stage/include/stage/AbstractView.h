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
#include "layout/geom/LayoutArea.h"
#include "stage/grid/Grid.h"

using namespace ci;

namespace next{
    class AbstractView {
    protected:
        LayoutArea mArea;
        Grid*      mGrid;


    public:
        AbstractView(Grid* grid, const LayoutArea& area) :
        mGrid(grid), mArea(area){}

        virtual void onConfigDidChange() = 0;
        virtual void draw(const CameraOrtho& cmaera, bool useMaterialShader) = 0;
        virtual void update() = 0;
    };
}



#endif
