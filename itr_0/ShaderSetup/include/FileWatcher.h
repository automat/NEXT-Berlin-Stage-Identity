//
//  BoostFileWatcher.h
//  GPGPUTest01
//
//  Created by Henryk Wollik on 29/01/14.
//
//

#ifndef GPGPUTest01_BoostFileWatcher_h
#define GPGPUTest01_BoostFileWatcher_h

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
    };
    
    map<string,File> mFilesToWatch;
    
    inline void getFileWriteTime(File& file){
        if (!filesystem::exists(file.path)) {
            throw FileWatcherRemovedExc(file.path);
        }
        
        file.timeModifiedOld = file.timeModifiedNew;
        file.timeModifiedNew = filesystem::last_write_time(file.path);
    }
    
public:
    FileWatcher(){}
    
    inline void addFile(const string& filePath){
        if(!filesystem::exists(filePath)){
            throw FileWatcherNotExistExc(filePath);
        }
        
        if (mFilesToWatch.count(filePath) == 1) { //check if key already exist
            return;
        }
        
        File file;
        file.path = filePath;

        mFilesToWatch[filePath] = file;
    }
    
    inline void removeFile(const string& filePath){
        if(mFilesToWatch.count(filePath) == 0){
            throw FileWatcherNotAddedExc(filePath);
        }
        mFilesToWatch.erase(mFilesToWatch.find(filePath));
    }
    
    inline bool filesDidChange(){
        for(map<string,File>::iterator itr = mFilesToWatch.begin();itr != mFilesToWatch.end();itr++){
            this->getFileWriteTime(itr->second);
            if(itr->second.timeModifiedNew != itr->second.timeModifiedOld){
                return true;
            }
        }
        return false;
    }
    
    inline bool fileDidChange(const string& filePath){
        if(mFilesToWatch.count(filePath) == 0){
            throw FileWatcherNotAddedExc(filePath);
        }
        File& file = mFilesToWatch[filePath];
        getFileWriteTime(file);
        return file.timeModifiedOld != -1 && file.timeModifiedNew != file.timeModifiedOld;
    }
};







#endif
