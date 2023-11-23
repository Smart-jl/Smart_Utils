#ifndef __DATA_STORAGE_H
#define __DATA_STORAGE_H

#include <stdlib.h>
#include <string.h>
#include "openList.h"

class dataStorage{
public:
    typedef struct{
        const char* name;
        void* data;
        unsigned int dataLen;
    }dataStorageInfo_t;
public:
    dataStorage(const char* id);
    ~dataStorage();
    void Add(const char* name, void* data, size_t size);
    void Remove(const char* name);
    void Remove(const char* name, unsigned int num);
    unsigned int GetNum();
    unsigned int GetNum(const char* name);
    dataStorageInfo_t* GetInfo(const char* name);
    dataStorageInfo_t* GetInfo(const char* name, unsigned int num);
public:
    const char* ID;
private:
    void* dataStorage_malloc(size_t size);
    void dataStorage_free(void* p);
private:
    openList listRoot;
};

#endif
