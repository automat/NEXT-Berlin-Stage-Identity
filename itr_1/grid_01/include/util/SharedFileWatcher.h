//
//  SharedFileWatcher.h
//  grid_00
//
//  Created by Henryk Wollik on 29/03/14.
//
//

#ifndef grid_00_SharedFileWatcher_h
#define grid_00_SharedFileWatcher_h

#include <iostream>
#include "util/FileWatcher.h"

typedef std::shared_ptr<FileWatcher> SharedFileWatcherRef;

class SharedFileWatcher {
    SharedFileWatcher(){};
public:
    inline static SharedFileWatcherRef Get(){
        static SharedFileWatcherRef ref = std::make_shared<FileWatcher>();
        return ref;
    }
};


#endif
