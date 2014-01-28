//
//  Scene.h
//  WindingCubes
//
//  Created by Henryk Wollik on 16/12/13.
//
//

#ifndef WindingCubes_Scene_h
#define WindingCubes_Scene_h
#include <vector>
#include "cinder/Vector.h"

struct Edge{
    ci::Vec3f* from;
    ci::Vec3f* to;
    Edge(){
        from = 0;
        to   = 0;
    }
};

class Scene {
private:
    std::vector<ci::Vec3f> mPoints;
    std::vector<Edge>      mEdges;
    
    
public:

    Scene();
    ~Scene();
    
    void draw(float time, float timeDelta, int tick);
    void update(float time, float timeDelta, int tick);

};


#endif
