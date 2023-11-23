#ifndef __DATA_RECEIVER_H
#define __DATA_RECEIVER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define DATA_RECEIVER_USE_TIMEOUT                   1
#define DATA_RECEIVER_USE_DEBUG                     1

#if DATA_RECEIVER_USE_TIMEOUT
#include <rtthread.h>
typedef void (*data_receiver_data_pool_callback_t)(void* arg);
#endif

/* For buf1, you can store some data prefixes, such as: command type, etc. For buf2, you should store real data.
 * If you don't need buf1 to store the data prefix, just write 0 by default */

typedef struct dataReceiver_struct{
    unsigned char *head;
    unsigned char headSize;
    unsigned char *coda;
    unsigned char codaSize;
    unsigned char *buf1Temp;
    unsigned char *buf2Temp;
    unsigned char *buf1;
    unsigned int buf1Size;
    unsigned int buf1len;
    unsigned char *buf2;
    unsigned int buf2Size;
    unsigned int buf2len;
    bool receiverFlag;
    /*
     * For dataPool, it is suitable for a large amount of unfixed length data.
     * If it is fixed length data, it is recommended to directly use receive instead of opening the dataPool!
     *
     *
     *
     * When the dataPool is enabled, there are two ways to parse the dataPool.
     * The first method is to call in the following format after you confirm that the data has been sent:
     * dataReceiver->Receive(dataReceiver, dataReceiver->dataPool, dataReceiver->dataPoolLen);
     *
     * And The second way is to enable the timeout mechanism. After the timeout mechanism is enabled,
     * it will be re timed every time a data is added into dataPool. When the time exceeds the set time,
     * the dataPool will be parsed and the callback function will be called (of course,
     * you need to first set the callback function through SetTimeOutCallback)
     * */
    unsigned char* dataPool;
    unsigned char* dataPoolTemp;
    unsigned int dataPoolLen;
#if DATA_RECEIVER_USE_TIMEOUT
    bool isTimeOut;
    unsigned int timeout;
    rt_timer_t timeoutTimer;
    data_receiver_data_pool_callback_t dataPoolCallback;
#endif

    void (*Receive)(struct dataReceiver_struct* receiver, unsigned char *p, unsigned int pSize);
    void (*EnableDataPool)(struct dataReceiver_struct* receiver);
    void (*DataPoolAdd)(struct dataReceiver_struct* receiver, unsigned char *p, unsigned int pSize);
    void (*Reset)(struct dataReceiver_struct* receiver);
#if DATA_RECEIVER_USE_TIMEOUT
    void (*SetTimeOutPeriod)(struct dataReceiver_struct* receiver, unsigned int timeout);
    void (*SetTimeOutEnable)(struct dataReceiver_struct* receiver, bool isEnable);
    void (*SetTimeOutCallback)(struct dataReceiver_struct* receiver, data_receiver_data_pool_callback_t callback);
#endif
}dataReceiver_t;


#ifdef __cplusplus
extern "C"{
#endif
void DataReceiver_Destroy(dataReceiver_t* receiver);
dataReceiver_t* DataReceiver_New(unsigned int buf1Size,
                                 unsigned int buf2Size,
                                 const unsigned char *head,
                                 unsigned char headSize,
                                 const unsigned char *coda,
                                 unsigned char codaSize);
unsigned short DataReceiver_FigureCRC16(unsigned char *p, unsigned int pSize);
#ifdef __cplusplus
}
#endif
#endif