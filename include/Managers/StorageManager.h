#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <cstdint>
#include "vector"
#include "string"

class StorageManager {
private:
    static StorageManager* _instance;

public:
    static StorageManager* getInstance();
    bool begin(bool format = true);
    bool format();
    size_t getFreeSpace();
    size_t getTotalSpace();

    bool exists(const char* path);
    size_t getFileSize(const char* path);
    std::vector<std::string> listFiles(const char* path);

    bool writeText(const char* path, const char* data, bool replace = true);
    std::string readText(const char* path);

    bool writeBinary(const char* path, const uint8_t* data, size_t size, bool replace = true);
    size_t readBinary(const char* path, uint8_t* buffer, size_t bufferSize);

    bool deleteFile(const char* path);
    bool renameFile(const char* path, const char* name);

    std::vector<std::string> listDir(const char* path);
    bool createDir(const char* path);
    bool renameDir(const char* path, const char* name);
    bool deleteDir(const char* path);
};


#endif