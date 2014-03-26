//
//  Utils.h
//  QuoteLayout
//
//  Created by Henryk Wollik on 17/03/14.
//
//

#ifndef QuoteLayout_Utils_h
#define QuoteLayout_Utils_h

#include <functional>
#include <algorithm>
#include <string>

namespace utils {
    using namespace std;
    inline void removeChar(string& str, const char& c){
        str.erase(remove(str.begin(), str.end(), c), str.end());
    }
    /*
    inline void removeCharFrontBack(string& str, const char& c){
        str.erase(str.begin(), find_if(str.begin(), str.end(), c));
    }*/
}

#endif
