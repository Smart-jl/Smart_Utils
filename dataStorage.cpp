#include "dataStorage.h"
#include "typedefs.h"
/* 数据存储单元 */

/* 内部数据信息存储单元 */
typedef struct{
    dataStorage::dataStorageInfo_t info;
    openList list;
}dataStorageInfoInternal_t;

void *dataStorage::dataStorage_malloc(size_t size) {
    return rt_malloc(size);
}

void dataStorage::dataStorage_free(void *p) {
    rt_free(p);
}

dataStorage::dataStorage(const char *id) {
    this->ID = id;
    Ol_Administration()->Init(&this->listRoot);
}

dataStorage::~dataStorage() {

}
/* 添加数据块函数 */
void dataStorage::Add(const char *name, void* data, size_t size) {
    void* _p = dataStorage_malloc(size);
    if(!_p && size != 0)
        return;
    memcpy(_p, data, size);

    dataStorageInfoInternal_t* _infoInternal = (dataStorageInfoInternal_t*)dataStorage_malloc(sizeof(dataStorageInfoInternal_t));
    dataStorageInfo_t* _info = &_infoInternal->info;

    /* Init the chain */
    Ol_Administration()->Init(&_infoInternal->list);

    _info->name = name;
    _info->data = _p;
    _info->dataLen = size;

    /* Insert the chain */
    Ol_Administration()->InsertAfter(this->listRoot.prev, &_infoInternal->list);
}

void dataStorage::Remove(const char *name) {
    Remove(name, 1);
}

void dataStorage::Remove(const char *name, unsigned int num) {
    if(name == NULL)
        return;
    unsigned int _count = 0;
    dataStorageInfoInternal_t* _infoInternal = NULL;
    openList* _listTemp = &this->listRoot;
    openList* _listCmp = _listTemp;
    while(_listTemp->next != _listCmp){
        /* 通过链表地址获取结构体首地址 */
        _infoInternal = ol_ListEntry(_listTemp->next,dataStorageInfoInternal_t,list);
        if(_infoInternal != NULL){
            if(strcmp(name, _infoInternal->info.name) == 0){
                _count++;
                if(_count == num){
                    Ol_Administration()->Remove(&_infoInternal->list);
                    dataStorage_free(_infoInternal->info.data);
                    dataStorage_free(_infoInternal);
                    return;
                }
            }
        }
        _listTemp = _listTemp->next;
    }
}

unsigned int dataStorage::GetNum() {
    return GetNum(NULL);
}

unsigned int dataStorage::GetNum(const char *name) {
    unsigned int _count = 0;
    dataStorageInfoInternal_t* _infoInternal = NULL;
    openList* _listTemp = &this->listRoot;
    openList* _listCmp = _listTemp;
    while(_listTemp->next != _listCmp){
        /* 通过链表地址获取结构体首地址 */
        _infoInternal = ol_ListEntry(_listTemp->next,dataStorageInfoInternal_t,list);
        if(_infoInternal != NULL){
            if(name != NULL){
                if(strcmp(name, _infoInternal->info.name) == 0)
                    _count++;
            }else{
                _count++;
            }
        }
        _listTemp = _listTemp->next;
    }

    return _count;
}

dataStorage::dataStorageInfo_t *dataStorage::GetInfo(const char *name) {
    return GetInfo(name, 1);
}

dataStorage::dataStorageInfo_t *dataStorage::GetInfo(const char *name, unsigned int num) {
    if(name == NULL)
        return NULL;
    unsigned int _count = 0;
    dataStorageInfoInternal_t* _infoInternal = NULL;
    openList* _listTemp = &this->listRoot;
    openList* _listCmp = _listTemp;
    while(_listTemp->next != _listCmp){
        /* 通过链表地址获取结构体首地址 */
        _infoInternal = ol_ListEntry(_listTemp->next,dataStorageInfoInternal_t,list);
        if(_infoInternal != NULL){
            if(strcmp(name, _infoInternal->info.name) == 0){
                _count++;
                if(_count == num){
                    return (dataStorage::dataStorageInfo_t*)_infoInternal;
                }
            }
        }
        _listTemp = _listTemp->next;
    }

    return NULL;
}