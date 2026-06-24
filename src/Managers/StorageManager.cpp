#include "Managers/StorageManager.h"
#include <LittleFS.h>

StorageManager* StorageManager::_instance = nullptr;

StorageManager* StorageManager::getInstance() {
    if (_instance == nullptr) _instance = new StorageManager();
    return _instance;
}

bool StorageManager::begin(bool format){return LittleFS.begin(format);}
bool StorageManager::format(){return LittleFS.format();}

size_t StorageManager::getFreeSpace(){return LittleFS.totalBytes() - LittleFS.usedBytes();}
size_t StorageManager::getTotalSpace(){return LittleFS.totalBytes();}

bool StorageManager::exists(const char* path){return LittleFS.exists(path);}

size_t StorageManager::getFileSize(const char* path){
    File file = LittleFS.open(path, "r");
    if (!file) return 0;
    size_t size = file.size();
    file.close();
    return size;
}

std::vector<std::string> StorageManager::listFiles(const char* path){
    std::vector<std::string> fileNames;
    File root = LittleFS.open(path);
    if (!root || !root.isDirectory()) {
        return fileNames; 
    }

    File file = root.openNextFile();
    while (file) {
        if(!file.isDirectory()) fileNames.push_back(file.name());
        file = root.openNextFile();
    }
    root.close();
    return fileNames;
}

bool StorageManager::writeText(const char* path, const char* data, bool replace){
    File file = LittleFS.open(path, replace? FILE_WRITE : FILE_APPEND);
    if (!file) return false;
    
    size_t written = file.print(data);
    file.close();
    return written > 0;
}

std::string StorageManager::readText(const char* path){
    File file = LittleFS.open(path, "r");
    if (!file) return "";
    
    std::string content = file.readString().c_str();
    file.close();
    return content;
}

bool StorageManager::writeBinary(const char* path, const uint8_t* data, size_t size, bool replace){
    File file = LittleFS.open(path, replace ? FILE_WRITE : FILE_APPEND);
    if (!file) return false;
    
    size_t written = file.write(data, size);
    file.close();
    return written == size;
}

size_t StorageManager::readBinary(const char* path, uint8_t* buffer, size_t bufferSize){
    File file = LittleFS.open(path, "r");
    if (!file) return 0;
    
    size_t bytesRead = file.read(buffer, bufferSize);
    file.close();
    return bytesRead;
}

bool StorageManager::deleteFile(const char* path){return LittleFS.remove(path);}

bool StorageManager::renameFile(const char* path, const char* name){return LittleFS.rename(path, name);}

std::vector<std::string> StorageManager::listDir(const char* path){
    std::vector<std::string> fileNames;
    File root = LittleFS.open(path);
    if (!root || !root.isDirectory()) {
        return fileNames; 
    }

    File file = root.openNextFile();
    while (file) {
        fileNames.push_back(file.name());
        file = root.openNextFile();
    }
    root.close();
    return fileNames;
}

bool StorageManager::createDir(const char* path){return LittleFS.mkdir(path);}

bool StorageManager::renameDir(const char* path, const char* name){return LittleFS.rename(path, name);}

bool StorageManager::deleteDir(const char* path){return LittleFS.rmdir(path);}
