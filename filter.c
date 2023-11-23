#include <malloc.h>
#include "filter.h"

#define MEDIAN_FILTER_WINDOWS_SIZE      5

typedef unsigned int(*pFilter_func_t)(struct filter_struct* pFilter);

static void* __filter_malloc(size_t size){
    return malloc(size);
}
static void __filter_free(void *p){
    free(p);
}


static unsigned int _filter_get(filterInternal_t* pFilter){
    for (int i = 0; i < pFilter->bufLen; i++) {
        pFilter->buf[i] = pFilter->getFunc();
    }
}
static uint32_t _median_filter(struct filter_struct* pFilter){

    filterInternal_t* filterInternal = (filterInternal_t*)pFilter;
    _filter_get(filterInternal);

    /* 创建一个临时数组储存数据 */
    unsigned int _windows[MEDIAN_FILTER_WINDOWS_SIZE];
    /* 遍历数据 */
    for (int i = 0; i < filterInternal->bufLen; ++i) {
        /* 计算窗口的开始和节数索引*/
        int startIndex = (i - (MEDIAN_FILTER_WINDOWS_SIZE / 2)) < 0 ? 0 : (i - (MEDIAN_FILTER_WINDOWS_SIZE / 2));
        int endIndex = (i + (MEDIAN_FILTER_WINDOWS_SIZE / 2)) >= filterInternal->bufLen ? (filterInternal->bufLen - 1) : (i + (MEDIAN_FILTER_WINDOWS_SIZE / 2));

        /* 填充窗口数据 */
        int _windowSize = endIndex - startIndex + 1;
        for (int j = 0; j < _windowSize; j++) {
            _windows[j] = filterInternal->buf[startIndex + j];
        }

        for (int m = 0; m < _windowSize - 1; m++) {
            for (int n = 0; n < _windowSize - m -1; n++) {
                if (_windows[n] > _windows[n + 1])
                {
                    /* 交换元素的位置 */
                    unsigned int  _temp = _windows[n];
                    _windows[n] = _windows[n + 1];
                    _windows[n + 1] = _temp;
                }
            }
        }
        /* 将中间位置的值作为滤波后的值输出 */
        filterInternal->buf[i] = _windows[_windowSize / 2];
    }
    return filterInternal->buf[filterInternal->bufLen / 2];
}
static uint32_t _none_filter(struct filter_struct* pFilter){

}
static pFilter_func_t filterFunc[FILTER_FUNC_MAX_NUM] = {
    _median_filter,
    _none_filter,
};
static int _filter_changeLan(struct filter_struct* pFilter,uint16_t len){
    filterInternal_t* filterInternal = (filterInternal_t*)pFilter;
    if (len == filterInternal->bufLen)
        return 0;
    /* 释放buf 重新申请内存的大小 */
    __filter_free(filterInternal->buf);
    filterInternal->buf = __filter_malloc(sizeof(unsigned int) * len);
    if (filterInternal->buf){
        filterInternal->bufLen = len;
        return 0;
    }
    __filter_free(filterInternal->buf);
    return -1;
}
static void _filter_change_type(struct filter_struct* pFilter, filterFuncType_t type){
    pFilter->Filter = filterFunc[type];
}
static void _set_get_func(struct filter_struct* pFilter,filter_get_func_t func){
    filterInternal_t* filterInternal = (filterInternal_t*)pFilter;
    filterInternal->getFunc = func;
}
void FilterDelete(filter_t* pFilter){
    filterInternal_t *filterInternal = (filterInternal_t*)pFilter;
    __filter_free(filterInternal->buf);
    __filter_free(filterInternal);
}
filter_t* FilterNew(short len){
    filterInternal_t* filterInternal = __filter_malloc(sizeof(filterInternal_t));
    if (!filterInternal)
        goto r1;
    filterInternal->buf = __filter_malloc(sizeof(unsigned int) * len);
    if (!filterInternal->buf)
        goto r2;
    filterInternal->bufLen = len;
    filterInternal->filter.ChangeLen = _filter_changeLan;
    filterInternal->filter.ChangeFilter = _filter_change_type;
    filterInternal->filter.SetGetFunc = _set_get_func;

    /* 定义过滤器的类型 */
    filterInternal->filter.Filter = _median_filter;
    return &(filterInternal->filter);
    r2:
    __filter_free(filterInternal->buf);
    r1:
    return NULL;

}