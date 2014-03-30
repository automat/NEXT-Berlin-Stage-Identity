//
//  Utils.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef QuoteLayout_Utils_h
#define QuoteLayout_Utils_h

#include <algorithm>
#include <string>

namespace utils {
    using namespace std;
    using namespace ci;

    inline void removeChar(string& str, const char& c){
        str.erase(remove(str.begin(), str.end(), c), str.end());
    }
}

#endif

