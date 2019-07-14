

#ifndef _NET_MANAGE_H_
#define _NET_MANAGE_H_

#include "app_freertos.h"
#include "hal.h"

#define NET_QUEUE_DEFAULT_LENGTH    1024
#define __HTTPS_ENABLE__            1
typedef enum
{
    NET_CONNECTION_REQ,
    NET_SEND_DATA_REQ,
    NET_RECEIVE_DATA_REQ,
    NET_MAX_TYPE
}net_opt_type_t;

typedef struct 
{
    int32_t len;
    //uint8_t data[0];
    uint8_t data[NET_QUEUE_DEFAULT_LENGTH];
}msg_t;

void create_net_manage_task(void);
bool send_msg_to_server(void *msg);
#endif //_NET_MANAGE_H_