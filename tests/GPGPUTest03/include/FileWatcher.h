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
public:
	virtual const char* what() const throw(){return "File has been removed.";}
};

class FileWatcherNotAddedExc : public std::exception {
public:
	virtual const char* what() const throw(){return "File not added to watcher.";}
};

class FileWatcherNotExistExc : public std::exception {
public:
	virtual const char* what() const throw(){return "File does not exist.";}
};


class FileWatcher {
    struct File{
        string path;
        time_t timeModifiedNew = -1;
        time_t timeModifiedOld = -1;
    };
    
    map<string,File> mFilesToWatch;
    
    void getFileWriteTime(File& file){
        if (!filesystem::exists(file.path)) {
            throw FileWatcherRemovedExc();
        }
        
        file.timeModifiedOld = file.timeModifiedNew;
        file.timeModifiedNew = filesystem::last_write_time(file.path);
    }
    
public:
    FileWatcher(){}
    
    void addFile(const string& filePath){
        if(!filesystem::exists(filePath)){
            throw FileWatcherNotExistExc();
        }
        
        if (mFilesToWatch.count(filePath) == 1) { //check if key already exist
            return;
        }
        
        File file;
        file.path = filePath;

        mFilesToWatch[filePath] = file;
    }
    
    void removeFile(const string& filePath){
        if(mFilesToWatch.count(filePath) == 0){
            throw FileWatcherNotAddedExc();
        }
        mFilesToWatch.erase(mFilesToWatch.find(filePath));
    }
    
    bool filesDidChange(){
        for(map<string,File>::iterator itr = mFilesToWatch.begin();itr != mFilesToWatch.end();itr++){
            this->getFileWriteTime(itr->second);
            if(itr->second.timeModifiedNew != itr->second.timeModifiedOld){
                return true;
            }
        }
        return false;
    }
    
    bool fileDidChange(const string& filePath){
        if(mFilesToWatch.count(filePath) == 0){
            throw FileWatcherNotAddedExc();
        }
        File& file = mFilesToWatch[filePath];
        getFileWriteTime(file);
        return file.timeModifiedOld != -1 && file.timeModifiedNew != file.timeModifiedOld;
    }
};







#endif
