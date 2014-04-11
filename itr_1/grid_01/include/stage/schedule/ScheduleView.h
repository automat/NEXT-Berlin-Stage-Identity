//
//  ScheduleView.h
//  grid_01
//
//  Created by Henryk Wollik on 11/04/14.
//
//

#ifndef grid_01_ScheduleView_h
#define grid_01_ScheduleView_h

#include <vector>
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Material.h"

#include "util/FileWatcher.h"
#include "stage/grid/Index.h"

#include "stage/schedule/event/EventView.h"

#include "stage/AbstractView.h"

class ScheduleView : public AbstractView {
    
public:
    ScheduleView(Grid* grid, const LayoutArea& area);
    ~ScheduleView();
    
    void onConfigDidChange();
    void draw(const CameraOrtho& camera, bool useMaterialShader);
    void update();

    
};

#endif
