//
//  ITNBSequence.h
//  PRDCB
//
//  Created by Henryk Wollik on 20/11/13.
//
//

#ifndef PRDCB_ITNBSequence_h
#define PRDCB_ITNBSequence_h

#include <vector>
#include <sys/types.h>
#include "PRDCB/geom/TNBFrame.h"

namespace pr {
    namespace geom{
        class ITNBSequence{
        protected:
            std::vector<TNBFrame> mFrames;
        public:
            
            virtual ~ITNBSequence(){}
            virtual const std::vector<TNBFrame>& getFrames() const = 0;
            virtual std::vector<TNBFrame>& getFrames() = 0;
            virtual size_t getNumFrames() = 0;
            virtual void recalcFrames() = 0;
        };
    }
}



#endif
