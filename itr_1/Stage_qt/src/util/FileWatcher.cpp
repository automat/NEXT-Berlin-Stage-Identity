#include "util/FileWatcher.h"


FileWatcher::File::File() :
    timeModifiedNew(-1),
    timeModifiedOld(-1){}

FileWatcher::File::File(const string& path) :
    timeModifiedNew(-1),
    timeModifiedOld(-1),
    path(path){}


void FileWatcher::getFileWriteTime(FileWatcher::File &file){
    if (!filesystem::exists(file.path)) {
        throw FileWatcherRemovedExc(file.path);
    }
    
    file.timeModifiedOld = file.timeModifiedNew;
    file.timeModifiedNew = filesystem::last_write_time(file.path);
}

void FileWatcher::addFile(const string &filePath){
    if(!filesystem::exists(filePath)){
        throw FileWatcherNotExistExc(filePath);
    }
    
    if (this->hasFile(filePath)) { //check if key already exist
        return;
    }
    mFilesToWatch[filePath] = File(filePath);
}

void FileWatcher::removeFile(const string& filePath){
    if(!this->hasFile(filePath)){
        throw FileWatcherNotAddedExc(filePath);
    }
    mFilesToWatch.erase(mFilesToWatch.find(filePath));
}

bool FileWatcher::filesDidChange(){
    for(map<string,File>::iterator itr = mFilesToWatch.begin();itr != mFilesToWatch.end();itr++){
        this->getFileWriteTime(itr->second);
        if(itr->second.timeModifiedNew != itr->second.timeModifiedOld){
            return true;
        }
    }
    return false;
}

bool FileWatcher::fileDidChange(const string& filePath){
    if(mFilesToWatch.count(filePath) == 0){
        throw FileWatcherNotAddedExc(filePath);
    }
    File& file = mFilesToWatch[filePath];
    getFileWriteTime(file);
    return file.timeModifiedOld != -1 && file.timeModifiedNew != file.timeModifiedOld;
}

bool FileWatcher::hasFile(const string &filePath){
    return mFilesToWatch.count(filePath) == 1;
}

void FileWatcher::clear(){
    mFilesToWatch.clear();
}