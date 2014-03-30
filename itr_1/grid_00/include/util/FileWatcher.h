//
//  FileWatcher.h
//  grid_00
//
//  Created by Henryk Wollik on 29/03/14.
//
//

#ifndef grid_00_FileWatcher_h
#define grid_00_FileWatcher_h

#include <boost/filesystem.hpp>
#include <string>
#include <map>
#include <exception>

using namespace std;
using namespace boost;

class FileWatcherRemovedExc : public std::exception {
    std::string mMessage;
public:
    FileWatcherRemovedExc(const std::string& msg) : mMessage("File has been removed: " + msg){};
	inline virtual const char* what() const throw(){return mMessage.c_str();}
};

class FileWatcherNotAddedExc : public std::exception {
    std::string mMessage;
public:
    FileWatcherNotAddedExc(const std::string& msg) : mMessage("File not added to watcher: " + msg){};
	inline virtual const char* what() const throw(){return mMessage.c_str();}
};

class FileWatcherNotExistExc : public std::exception {
    std::string mMessage;
public:
	FileWatcherNotExistExc( const std::string &msg) : mMessage("File does not exist: " + msg){};
    inline virtual const char* what() const throw(){return mMessage.c_str();}
};


class FileWatcher {
    struct File{
        string path;
        time_t timeModifiedNew = -1;
        time_t timeModifiedOld = -1;
        File(){};
        File(const string& path) : path(path){}
    };
    
    map<string,File> mFilesToWatch;
    
    void getFileWriteTime(File& file);
    
public:
    FileWatcher(){}
    
    void addFile(const string& filePath);
    void removeFile(const string& filePath);
    
    bool filesDidChange();
    bool fileDidChange(const string& filePath);
    
    inline bool hasFile(const string& filePath){
        return mFilesToWatch.count(filePath) == 1;
    }
    
    inline void clear(){
        mFilesToWatch.clear();
    }
};


#endif
