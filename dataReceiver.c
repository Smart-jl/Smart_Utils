#include "dataReceiver.h"
#if DATA_RECEIVER_USE_DEBUG
#include "debug.h"
#include "delay.h"

#define TAG                     "[DataReceiver]"
#endif
#if DATA_RECEIVER_USE_TIMEOUT
static void __data_receiver_timeoutTimer_entry(void* arg){
    dataReceiver_t* receiver = (dataReceiver_t*)arg;
    if(receiver->dataPoolLen != 0){
        receiver->Receive(receiver, receiver->dataPool, receiver->dataPoolLen);
        receiver->dataPoolLen = 0;
#if DATA_RECEIVER_USE_DEBUG
        JL_LOGD(TAG, "Data-Pool receive");
#endif
    }
    if(receiver->receiverFlag == false){
        receiver->Reset(receiver);
#if DATA_RECEIVER_USE_DEBUG
        JL_LOGD(TAG, "Reset");
#endif
    }else{
        if(receiver->dataPoolCallback != NULL)
            receiver->dataPoolCallback((void*)receiver);
    }
}
#endif

static void* __data_receiver_malloc(size_t len){
    return rt_malloc(len);
}

static void __data_receiver_free(void* p){
    rt_free(p);
}

static void _receive(struct dataReceiver_struct* receiver, unsigned char *p, unsigned int pSize){
    unsigned int i,j,start = 0,end = pSize;

    if(receiver->buf1len == 0 && receiver->buf2len == 0) {
        for (i = 0; i < receiver->headSize; i++) {
            if (p[i] != receiver->head[i])
                break;
        }
        if(i != receiver->headSize){
            return;
        }
        receiver->receiverFlag = false;
        start = receiver->headSize;
    }
    for (i = start; i < pSize; i++) {
        if(p[i] == receiver->coda[0]){
            for(j = i; j < i + receiver->codaSize; j++){
                if(p[j] != receiver->coda[j-i])
                    break;
            }
            if(j == i + receiver->codaSize){
                end = i;
                receiver->receiverFlag = true;
                break;
            }
        }
    }
    if(receiver->buf1Size != 0){
        if(receiver->buf1Size != receiver->buf1len){
            for(j = start; j < end; j++){
                receiver->buf1[receiver->buf1len++] = p[j];
                start++;
                if(receiver->buf1len == receiver->buf1Size)
                    break;
            }
        }
    }
    for(j = start; j < end; j++){
        receiver->buf2[receiver->buf2len++] = p[j];
    }

#if DATA_RECEIVER_USE_TIMEOUT
    if(receiver->timeoutTimer != RT_NULL && receiver->isTimeOut && receiver->dataPool == NULL){
        rt_timer_stop(receiver->timeoutTimer);
        if(receiver->receiverFlag == false)
            rt_timer_start(receiver->timeoutTimer);
    }
#endif
}

static void _data_pool_add(struct dataReceiver_struct* receiver, unsigned char *p, unsigned int pSize){
    if(receiver->dataPool == NULL)
        return;
    for(int i = 0; i < pSize; i++){
        receiver->dataPool[receiver->dataPoolLen++] = p[i];
    }
#if DATA_RECEIVER_USE_TIMEOUT
    if(receiver->timeoutTimer != RT_NULL && receiver->isTimeOut){
        rt_timer_stop(receiver->timeoutTimer);
        if(receiver->receiverFlag == false)
            rt_timer_start(receiver->timeoutTimer);
    }
#endif
}

static void _reset(struct dataReceiver_struct* receiver){
    receiver->receiverFlag = 0;
    receiver->buf2len = 0;
    receiver->buf1len = 0;
    receiver->dataPoolLen = 0;
}

static void _enable_data_pool(struct dataReceiver_struct* receiver){
    receiver->dataPool = (unsigned char*)__data_receiver_malloc(receiver->buf2Size * 2);
    if(!receiver->dataPool)
        goto r1;
    receiver->dataPoolTemp = receiver->dataPool;

    receiver->dataPoolLen = 0;
    r1:
    return;
}

#if DATA_RECEIVER_USE_TIMEOUT
static void _set_time_out_period(struct dataReceiver_struct* receiver, unsigned int timeout){
    if(receiver->timeoutTimer == RT_NULL){
        char tempName[32];
        sprintf(tempName, "data_receive-%p", receiver);
        receiver->timeoutTimer = rt_timer_create(tempName,
                                                  __data_receiver_timeoutTimer_entry,
                                                  receiver,
                                                  timeout,
                                                  RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
    }else{
        rt_timer_control(receiver->timeoutTimer, RT_TIMER_CTRL_SET_TIME, &timeout);
    }
}

static void _set_time_out_enable(struct dataReceiver_struct* receiver, bool isEnable){
    receiver->isTimeOut = isEnable;
    if(isEnable == false){
        rt_timer_stop(receiver->timeoutTimer);
    }
}
static void _set_time_out_callback(struct dataReceiver_struct* receiver, data_receiver_data_pool_callback_t callback){
    if(receiver->dataPool == NULL)
        return;
    receiver->dataPoolCallback = callback;
}
#endif

unsigned short DataReceiver_FigureCRC16(unsigned char *p, unsigned int pSize){
    unsigned int i;
    unsigned short crc = 0xFFFF;

    while(pSize--){
        crc ^= *p++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc = (crc >> 1);
        }
    }

    return crc;
}

void DataReceiver_Destroy(dataReceiver_t* receiver){
    if(!receiver)
        return;
    if(receiver->buf1Temp)
        __data_receiver_free(receiver->buf1Temp);
    if(receiver->buf2Temp)
        __data_receiver_free(receiver->buf2Temp);
    if(receiver->dataPoolTemp)
        __data_receiver_free(receiver->dataPoolTemp);
    __data_receiver_free(receiver->head);
    __data_receiver_free(receiver->coda);
#if DATA_RECEIVER_USE_TIMEOUT
    if(receiver->timeoutTimer != RT_NULL)
        rt_timer_delete(receiver->timeoutTimer);
#endif
    __data_receiver_free(receiver);
}

dataReceiver_t* DataReceiver_New(unsigned int buf1Size,
                                 unsigned int buf2Size,
                                 const unsigned char *head,
                                 unsigned char headSize,
                                 const unsigned char *coda,
                                 unsigned char codaSize){
    if(buf2Size == 0 || headSize == 0 || codaSize == 0)
        goto r1;

    dataReceiver_t *receiver = (dataReceiver_t*)__data_receiver_malloc(sizeof(dataReceiver_t));
    if(!receiver)
        goto r1;

    if(buf1Size != 0){
        receiver->buf1 = (unsigned char*)__data_receiver_malloc(buf1Size);
        if(!receiver->buf1)
            goto r2;
        receiver->buf1Temp = receiver->buf1;
    }else{
        receiver->buf1Temp = NULL;
    }

    receiver->buf2 = (unsigned char*)__data_receiver_malloc(buf2Size);
    if(!receiver->buf2)
        goto r3;
    receiver->buf2Temp = receiver->buf2;

    receiver->head = (unsigned char*)__data_receiver_malloc(headSize);
    if(!receiver->head)
        goto r4;

    receiver->coda = (unsigned char*)__data_receiver_malloc(codaSize);
    if(!receiver->coda)
        goto r5;

    receiver->dataPool = NULL;
    receiver->dataPoolTemp = NULL;

    receiver->buf1Size = buf1Size;
    receiver->buf2Size = buf2Size;
    receiver->headSize = headSize;
    receiver->codaSize = codaSize;
    receiver->buf2len = 0;
    receiver->buf1len = 0;
    receiver->dataPoolLen = 0;
    receiver->receiverFlag = false;
#if DATA_RECEIVER_USE_TIMEOUT
    receiver->isTimeOut = false;
    receiver->timeout = 0;
    receiver->timeoutTimer = RT_NULL;
    receiver->dataPoolCallback = NULL;
#endif

    memcpy(receiver->head, head, headSize);
    memcpy(receiver->coda, coda, codaSize);

    receiver->Receive = _receive;
    receiver->Reset = _reset;
    receiver->DataPoolAdd = _data_pool_add;
    receiver->EnableDataPool = _enable_data_pool;
#if DATA_RECEIVER_USE_TIMEOUT
    receiver->SetTimeOutPeriod = _set_time_out_period;
    receiver->SetTimeOutEnable = _set_time_out_enable;
    receiver->SetTimeOutCallback = _set_time_out_callback;
#endif
    return receiver;

    r5:
    __data_receiver_free(receiver->coda);
    r4:
    __data_receiver_free(receiver->head);
    r3:
    if(receiver->buf1)
        __data_receiver_free(receiver->buf1);
    r2:
    __data_receiver_free(receiver);
    r1:
    return NULL;
}