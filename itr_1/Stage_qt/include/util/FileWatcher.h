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
#include <memory>

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

typedef std::shared_ptr<class FileWatcher> FileWatcherRef;

class FileWatcher {
public:
   
private:
    struct File{
        string path;
        time_t timeModifiedNew;
        time_t timeModifiedOld;
        File();
        File(const string& path);
    };
    
    map<string,File> mFilesToWatch;
    void getFileWriteTime(File& file);
    FileWatcher(){}
    
public:
    
    void addFile(const string& filePath);
    void removeFile(const string& filePath);
    
    bool filesDidChange();
    bool fileDidChange(const string& filePath);
    
    bool hasFile(const string& filePath);
    
    void clear();
    
private:
public:
    
    inline static FileWatcherRef Get(){
        static std::mutex m;
        static std::weak_ptr<FileWatcher> cache;
        
        std::lock_guard<std::mutex> lg(m);
        std::shared_ptr<FileWatcher> shared = cache.lock();
        if(cache.expired()){
            shared.reset(new FileWatcher());
            cache = shared;
        }
        
        return shared;
    }
    
};


#endif
